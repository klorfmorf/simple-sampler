#include "voice.hpp"

#include <cmath>
#include <numbers>

namespace {
    constexpr f64 volume_epsilon = 1e-6;
}

void Voice::start(simple_sampler::engine::SampleHandle sample) {
    sample_ = sample;
    playback_position_ = 0.0;
    active_ = true;
    playing_ = true;
}

void Voice::stop() {
    active_ = false;
    playing_ = false;
    sample_ = {};
}

void Voice::release() {
    stop(); // Placeholder
}

void Voice::set_pitch(const f64 pitch) {
    pitch_ = pitch;
}

void Voice::set_volume(const f64 volume) {
    volume_ = volume;
}

void Voice::set_volume_ramp(const f64 volume, const usize duration_us, const f64 output_sample_rate) {
    if (duration_us == 0) {
        volume_ = volume;
        volume_ramp_samples_remaining_ = 0;
        return;
    }

    const usize ramp_samples = static_cast<usize>((static_cast<f64>(duration_us) * output_sample_rate / 1'000'000.0));
    if (ramp_samples == 0) {
        volume_ = volume;
        return;
    }

    volume_target_ = volume;
    volume_ramp_samples_remaining_ = ramp_samples;

    const f64 current_volume = (volume_ <= volume_epsilon) ? volume_epsilon : volume_;
    const f64 target_volume = (volume_target_ <= volume_epsilon) ? volume_epsilon : volume_target_;

    volume_multiplier_ = std::pow(target_volume / current_volume, 1.0 / static_cast<f64>(ramp_samples));

    if (std::isnan(volume_multiplier_) || std::isinf(volume_multiplier_)) {
        volume_ = volume_target_;
        volume_ramp_samples_remaining_ = 0;
        volume_multiplier_ = 1.0;
    }
}

void Voice::set_pan(const f64 pan) {
    pan_ = pan;
}

void Voice::set_dry_wet_mix(const f64 dry_wet_mix) {
    dry_wet_mix_ = dry_wet_mix;
}

simple_sampler::engine::VoiceMixFrame Voice::process_silence() {
    if (volume_ramp_samples_remaining_ > 0) {
        volume_ *= volume_multiplier_;
        --volume_ramp_samples_remaining_;

        if (volume_ramp_samples_remaining_ == 0) {
            volume_ = volume_target_;
            volume_multiplier_ = 1.0;
        }
    }

    return {};
}

simple_sampler::engine::VoiceMixFrame Voice::process_stereo(const Sample& sample, const f64 output_sample_rate) {
    simple_sampler::engine::VoiceMixFrame output;

    if (!active_ || !sample_.valid()) {
        return process_silence();
    }

    process_silence();

    f32 raw_l = sample.get_sample(playback_position_, playback_speed_, 0);
    f32 raw_r = raw_l;

    if (sample.get_channel_count() > 1) {
        raw_r = sample.get_sample(playback_position_, playback_speed_, 1);
    }

    raw_l *= static_cast<f32>(volume_);
    raw_r *= static_cast<f32>(volume_);

    const f64 mix_angle    = dry_wet_mix_ * (std::numbers::pi / 2.0);
    const f64 mix_dry_gain = std::cos(mix_angle);
    const f64 mix_wet_gain = std::sin(mix_angle);

    const f64 pan_angle      = pan_ * (std::numbers::pi / 2.0);
    const f64 pan_gain_left  = std::cos(pan_angle);
    const f64 pan_gain_right = std::sin(pan_angle);

    output.dry.left  = raw_l * static_cast<f32>(mix_dry_gain * pan_gain_left);
    output.dry.right = raw_r * static_cast<f32>(mix_dry_gain * pan_gain_right);

    output.wet.left  = raw_l * static_cast<f32>(mix_wet_gain);
    output.wet.right = raw_r * static_cast<f32>(mix_wet_gain);

    playback_speed_     = pitch_ * (static_cast<f64>(sample.get_sample_rate()) / output_sample_rate);
    playback_position_ += playback_speed_;

    if (sample.get_loop_enabled()) {
        const f64 loop_end_boundary = static_cast<f64>(sample.get_loop_end()) + 1.0;
        if (playback_position_ >= loop_end_boundary) {
            const f64 loop_len_f = static_cast<f64>(sample.get_loop_end() - sample.get_loop_start() + 1);
            playback_position_  -= loop_len_f;
        }
    } else {
        if (playback_position_ >= static_cast<f64>(sample.get_frame_count())) {
            active_ = false;
        }
    }

    return output;
}
