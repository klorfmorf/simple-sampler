#pragma once

#include "audio_types.hpp"

class Sampler;

namespace simple_sampler::runtime {

class LegacyDriverRuntime final : public engine::DriverRuntime {
public:
    explicit LegacyDriverRuntime(Sampler& sampler);
    ~LegacyDriverRuntime() override;

    bool initialize(engine::AudioCommandSink& sink) override;
    void tick() override;
    [[nodiscard]] bool is_finished() const override;

private:
    bool submit(const engine::AudioCommand& command);

    void start_voice(usize voice, const char* sample);
    void stop_voice(usize voice);
    void set_pitch(usize voice, f32 pitch);
    void set_volume_ramp(usize voice, i16 volume, i32 duration);
    void set_pan(usize voice, u8 pan);
    void set_dry_wet_mix(usize voice, u8 mix);
    void set_reverb_parameter(i16 index, i32 value);
    void set_master_volume_ramp(f32 volume, i32 duration);
    void reset_reverb();
    void log(const char* message) const;

    static void start_voice_callback(void* user_data, usize voice, const char* sample);
    static void stop_voice_callback(void* user_data, usize voice);
    static void set_pitch_callback(void* user_data, usize voice, f32 pitch);
    static void set_volume_ramp_callback(void* user_data, usize voice, i16 volume, i32 duration);
    static void set_pan_callback(void* user_data, usize voice, u8 pan);
    static void set_dry_wet_mix_callback(void* user_data, usize voice, u8 mix);
    static void set_reverb_parameter_callback(void* user_data, i16 index, i32 value);
    static void set_master_volume_ramp_callback(void* user_data, f32 volume, i32 duration);
    static void reset_reverb_callback(void* user_data);
    static void log_callback(void* user_data, const char* message);

    Sampler& sampler_;
    engine::AudioCommandSink* sink_ = nullptr;
    bool initialized_ = false;
};

} // namespace simple_sampler::runtime
