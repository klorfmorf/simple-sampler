#include "sampler_engine.hpp"

#include "reverb_presets.hpp"

#include <algorithm>
#include <ranges>

namespace simple_sampler::engine {
namespace {
    [[nodiscard]] f32 clamp_audio(const f32 value) {
        return std::clamp(value, -1.0f, 1.0f);
    }
}

SamplerEngine::SamplerEngine(SampleRegistry& samples, AudioConfig config)
    : samples_(samples),
      config_(config),
      voices_(config_.voice_count) {
    reset_reverb();
}

void SamplerEngine::apply(const AudioCommand& command) {
    switch (command.type) {
    case AudioCommandType::StartVoice:
        start_voice(command.voice, command.sample);
        break;
    case AudioCommandType::StopVoice:
        stop_voice(command.voice);
        break;
    case AudioCommandType::ReleaseVoice:
        release_voice(command.voice);
        break;
    case AudioCommandType::SetPitch:
        set_voice_pitch(command.voice, command.value);
        break;
    case AudioCommandType::SetVolumeRamp:
        set_voice_volume_ramp(command.voice, command.value, command.duration_us);
        break;
    case AudioCommandType::SetPan:
        set_pan(command.voice, command.value);
        break;
    case AudioCommandType::SetDryWetMix:
        set_dry_wet_mix(command.voice, command.value);
        break;
    case AudioCommandType::SetReverbParameter:
        set_reverb_parameter(command.index, command.int_value);
        break;
    case AudioCommandType::SetMasterVolumeRamp:
        set_master_volume_ramp(command.value, command.duration_us);
        break;
    case AudioCommandType::ResetReverb:
        reset_reverb();
        break;
    }
}

void SamplerEngine::set_master_volume_ramp(const f64 volume, const isize duration_us) {
    if (duration_us <= 0) {
        master_volume_ = static_cast<f32>(volume);
        volume_change_per_sample_ = 0.0f;
        volume_change_samples_ = 0;
        volume_change_target_ = static_cast<f32>(volume);
        return;
    }

    volume_change_target_ = static_cast<f32>(volume);
    volume_change_samples_ = static_cast<isize>((static_cast<f64>(duration_us) / 1'000'000.0) * static_cast<f64>(config_.sample_rate));
    if (volume_change_samples_ <= 0) {
        master_volume_ = volume_change_target_;
        volume_change_per_sample_ = 0.0f;
        volume_change_samples_ = 0;
        return;
    }

    volume_change_per_sample_ = (volume_change_target_ - master_volume_) / static_cast<f32>(volume_change_samples_);
}

void SamplerEngine::reset_reverb() {
    reverb_.reset();
    reverb_.configure(default_reverb_preset.parameters, static_cast<f64>(config_.sample_rate));
    modified_reverb_parameters_.assign(default_reverb_preset.parameters.begin(), default_reverb_preset.parameters.end());
}

void SamplerEngine::start_voice(const VoiceId voice, const SampleHandle sample) {
    Voice* target_voice = voice_at(voice);
    if (!target_voice || !samples_.sample_at(sample)) {
        return;
    }

    target_voice->start(sample);
}

void SamplerEngine::stop_voice(const VoiceId voice) {
    if (Voice* target_voice = voice_at(voice)) {
        target_voice->stop();
    }
}

void SamplerEngine::release_voice(const VoiceId voice) {
    if (Voice* target_voice = voice_at(voice)) {
        target_voice->release();
    }
}

void SamplerEngine::set_voice_pitch(const VoiceId voice, const f64 pitch) {
    if (Voice* target_voice = voice_at(voice)) {
        target_voice->set_pitch(pitch);
    }
}

void SamplerEngine::set_voice_volume_ramp(const VoiceId voice, const f64 volume, const isize duration_us) {
    if (duration_us < 0) {
        return;
    }

    if (Voice* target_voice = voice_at(voice)) {
        target_voice->set_volume_ramp(volume, static_cast<usize>(duration_us), static_cast<f64>(config_.sample_rate));
    }
}

void SamplerEngine::set_pan(const VoiceId voice, const f64 pan) {
    if (Voice* target_voice = voice_at(voice)) {
        target_voice->set_pan(pan);
    }
}

void SamplerEngine::set_dry_wet_mix(const VoiceId voice, const f64 dry_wet_mix) {
    if (Voice* target_voice = voice_at(voice)) {
        target_voice->set_dry_wet_mix(dry_wet_mix);
    }
}

void SamplerEngine::set_reverb_parameter(const usize index, i32 value) {
    if (index >= modified_reverb_parameters_.size()) {
        return;
    }

    if (index == 3) {
        value = aligned_ms_samples(static_cast<f64>(value) / 40.0, static_cast<f64>(config_.sample_rate));
    }

    modified_reverb_parameters_[index] = value;
    reverb_.configure(modified_reverb_parameters_, static_cast<f64>(config_.sample_rate));
}

ProcessResult SamplerEngine::process_frame() {
    ProcessResult result;

    if (volume_change_samples_) {
        --volume_change_samples_;
        master_volume_ += volume_change_per_sample_;

        if (volume_change_samples_ == 0) {
            master_volume_ = volume_change_target_;
            volume_change_per_sample_ = 0.0f;
        }
    }

    process_stereo(result);
    result.finished = status().finished();
    return result;
}

SamplerStatus SamplerEngine::status() const {
    const bool has_active_voices = std::ranges::any_of(voices_, [](const Voice& voice) {
        return voice.is_active();
    });

    return {
        .has_active_voices = has_active_voices,
        .master_volume_is_zero = master_volume_ == 0.0f,
        .command_queue_overflowed = false,
    };
}

Voice* SamplerEngine::voice_at(const VoiceId voice) {
    if (voice.value >= voices_.size()) {
        return nullptr;
    }

    return &voices_[voice.value];
}

void SamplerEngine::process_stereo(ProcessResult& result) {
    StereoFrame master_dry_samples;
    StereoFrame master_wet_samples;

    for (auto& voice : voices_) {
        const Sample* sample = samples_.sample_at(voice.sample_handle());
        const VoiceMixFrame voice_samples = sample
            ? voice.process_stereo(*sample, static_cast<f64>(config_.sample_rate))
            : voice.process_silence();

        master_dry_samples += voice_samples.dry;
        master_wet_samples += voice_samples.wet;
    }

    master_dry_samples.left  = clamp_audio(master_dry_samples.left  * master_volume_);
    master_dry_samples.right = clamp_audio(master_dry_samples.right * master_volume_);

    master_wet_samples.left  = clamp_audio(master_wet_samples.left  * master_volume_);
    master_wet_samples.right = clamp_audio(master_wet_samples.right * master_volume_);

    f64 wet_sample_left  = 0.0;
    f64 wet_sample_right = 0.0;
    reverb_.process(master_wet_samples.left, master_wet_samples.right, wet_sample_left, wet_sample_right);

    result.samples.left  = clamp_audio(master_dry_samples.left  + static_cast<f32>(wet_sample_left));
    result.samples.right = clamp_audio(master_dry_samples.right + static_cast<f32>(wet_sample_right));
}

} // namespace simple_sampler::engine
