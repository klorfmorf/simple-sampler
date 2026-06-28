#pragma once

#include "audio_types.hpp"
#include "reverb.hpp"
#include "sample_registry.hpp"
#include "voice.hpp"

#include <vector>

namespace simple_sampler::engine {

class SamplerEngine final {
public:
    explicit SamplerEngine(SampleRegistry& samples, AudioConfig config = {});

    void apply(const AudioCommand& command);
    [[nodiscard]] ProcessResult process_frame();
    [[nodiscard]] SamplerStatus status() const;

private:
    void set_master_volume_ramp(f64 volume, isize duration_us);
    void reset_reverb();
    void start_voice(VoiceId voice, SampleHandle sample);
    void stop_voice(VoiceId voice);
    void release_voice(VoiceId voice);
    void set_voice_pitch(VoiceId voice, f64 pitch);
    void set_voice_volume_ramp(VoiceId voice, f64 volume, isize duration_us);
    void set_pan(VoiceId voice, f64 pan);
    void set_dry_wet_mix(VoiceId voice, f64 dry_wet_mix);
    void set_reverb_parameter(usize index, i32 value);

    [[nodiscard]] Voice* voice_at(VoiceId voice);

    void process_stereo(ProcessResult& result);

    SampleRegistry& samples_;
    AudioConfig config_;
    std::vector<Voice> voices_;

    Reverb reverb_;
    std::vector<i32> modified_reverb_parameters_;

    f32 master_volume_ = 1.0f;
    isize volume_change_samples_ = 0;
    f32 volume_change_per_sample_ = 0.0f;
    f32 volume_change_target_ = 0.0f;
};

} // namespace simple_sampler::engine
