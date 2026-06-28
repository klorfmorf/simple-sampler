#pragma once

#include "audio_types.hpp"
#include "sample.hpp"
#include "types.hpp"

class Voice {
public:
    void start(simple_sampler::engine::SampleHandle sample);
    void stop();
    void release();

    void set_pitch(const f64 pitch);
    void set_volume(const f64 volume);
    void set_volume_ramp(const f64 volume, const usize duration_us, f64 output_sample_rate);
    void set_pan(const f64 pan);
    void set_dry_wet_mix(const f64 dry_wet_mix);

    [[nodiscard]] bool is_active() const {
        return active_;
    }

    [[nodiscard]] simple_sampler::engine::SampleHandle sample_handle() const {
        return sample_;
    }

    simple_sampler::engine::VoiceMixFrame process_stereo(const Sample& sample, f64 output_sample_rate);
    simple_sampler::engine::VoiceMixFrame process_silence();

private:
    simple_sampler::engine::SampleHandle sample_;

    f64 playback_position_ = 0.0;
    f64 playback_speed_ = 0.0;

    f64 pitch_ = 1.0;

    f64 volume_ = 1.0;
    f64 volume_target_ = 1.0;
    f64 volume_multiplier_ = 0.0;
    usize volume_ramp_samples_remaining_ = 0;

    f64 pan_ = 0.5;

    f64 dry_wet_mix_ = 0.0;

    bool active_ = false;
    bool playing_ = false;
};
