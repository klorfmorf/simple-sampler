#pragma once

#include "types.hpp"

#include <span>

namespace simple_sampler::engine {

inline constexpr usize default_sample_rate = 48'000;
inline constexpr usize default_channel_count = 2;
inline constexpr usize default_voice_count = 64;
inline constexpr usize default_max_sample_count = 1'024;

struct AudioConfig {
    usize sample_rate = default_sample_rate;
    usize channel_count = default_channel_count;
    usize voice_count = default_voice_count;
    usize max_sample_count = default_max_sample_count;
};

struct StereoFrame {
    f32 left = 0.0f;
    f32 right = 0.0f;

    constexpr StereoFrame& operator+=(const StereoFrame& other) noexcept {
        left += other.left;
        right += other.right;
        return *this;
    }

    [[nodiscard]] constexpr StereoFrame scaled(const f32 gain) const noexcept {
        return {left * gain, right * gain};
    }
};

struct VoiceMixFrame {
    StereoFrame dry;
    StereoFrame wet;
};

struct ProcessResult {
    StereoFrame samples;
    bool finished = false;
    bool has_samples = true;
};

struct SamplerStatus {
    bool has_active_voices = false;
    bool master_volume_is_zero = false;
    bool command_queue_overflowed = false;

    [[nodiscard]] constexpr bool finished() const noexcept {
        return master_volume_is_zero;
    }
};

struct VoiceId {
    usize value = 0;
};

struct SampleHandle {
    static constexpr usize invalid_value = static_cast<usize>(-1);

    usize value = invalid_value;

    [[nodiscard]] constexpr bool valid() const noexcept {
        return value != invalid_value;
    }
};

struct LoopRegion {
    usize start = 0;
    usize end = 0;

    [[nodiscard]] constexpr usize length() const noexcept {
        return end >= start ? (end - start + 1) : 0;
    }
};

struct ReverbPreset {
    std::span<const i32> parameters;
};

enum class AudioCommandType {
    StartVoice,
    StopVoice,
    ReleaseVoice,
    SetPitch,
    SetVolumeRamp,
    SetPan,
    SetDryWetMix,
    SetReverbParameter,
    SetMasterVolumeRamp,
    ResetReverb,
};

struct AudioCommand {
    AudioCommandType type = AudioCommandType::StopVoice;
    VoiceId voice {};
    SampleHandle sample {};
    f64 value = 0.0;
    isize duration_us = 0;
    usize index = 0;
    i32 int_value = 0;

    [[nodiscard]] static constexpr AudioCommand start_voice(const VoiceId voice, const SampleHandle sample) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::StartVoice;
        command.voice = voice;
        command.sample = sample;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand stop_voice(const VoiceId voice) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::StopVoice;
        command.voice = voice;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand release_voice(const VoiceId voice) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::ReleaseVoice;
        command.voice = voice;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_pitch(const VoiceId voice, const f64 pitch) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetPitch;
        command.voice = voice;
        command.value = pitch;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_volume_ramp(const VoiceId voice, const f64 volume, const isize duration_us) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetVolumeRamp;
        command.voice = voice;
        command.value = volume;
        command.duration_us = duration_us;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_pan(const VoiceId voice, const f64 pan) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetPan;
        command.voice = voice;
        command.value = pan;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_dry_wet_mix(const VoiceId voice, const f64 dry_wet_mix) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetDryWetMix;
        command.voice = voice;
        command.value = dry_wet_mix;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_reverb_parameter(const usize index, const i32 value) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetReverbParameter;
        command.index = index;
        command.int_value = value;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand set_master_volume_ramp(const f64 volume, const isize duration_us) noexcept {
        AudioCommand command;
        command.type = AudioCommandType::SetMasterVolumeRamp;
        command.value = volume;
        command.duration_us = duration_us;
        return command;
    }

    [[nodiscard]] static constexpr AudioCommand reset_reverb() noexcept {
        AudioCommand command;
        command.type = AudioCommandType::ResetReverb;
        return command;
    }
};

class AudioCommandSink {
public:
    virtual ~AudioCommandSink() = default;

    virtual bool submit(const AudioCommand& command) = 0;
};

class DriverRuntime {
public:
    virtual ~DriverRuntime() = default;

    virtual bool initialize(AudioCommandSink& sink) = 0;
    virtual void tick() = 0;
    [[nodiscard]] virtual bool is_finished() const = 0;
};

enum class Output {
    Monoaural,
    Stereo,
};

} // namespace simple_sampler::engine
