#include "legacy_driver_runtime.hpp"

#include "driver/driver.h"
#include "sampler.hpp"

#include <iostream>
#include <string>

namespace simple_sampler::runtime {

LegacyDriverRuntime::LegacyDriverRuntime(Sampler& sampler)
    : sampler_(sampler) {}

LegacyDriverRuntime::~LegacyDriverRuntime() {
    audio_driver_set_callbacks(nullptr);
}

bool LegacyDriverRuntime::initialize(engine::AudioCommandSink& sink) {
    sink_ = &sink;

    AudioDriverCallbacks callbacks {};
    callbacks.user_data = this;
    callbacks.start_voice = &LegacyDriverRuntime::start_voice_callback;
    callbacks.stop_voice = &LegacyDriverRuntime::stop_voice_callback;
    callbacks.set_pitch = &LegacyDriverRuntime::set_pitch_callback;
    callbacks.set_volume_ramp = &LegacyDriverRuntime::set_volume_ramp_callback;
    callbacks.set_pan = &LegacyDriverRuntime::set_pan_callback;
    callbacks.set_dry_wet_mix = &LegacyDriverRuntime::set_dry_wet_mix_callback;
    callbacks.set_reverb_parameter = &LegacyDriverRuntime::set_reverb_parameter_callback;
    callbacks.set_master_volume_ramp = &LegacyDriverRuntime::set_master_volume_ramp_callback;
    callbacks.reset_reverb = &LegacyDriverRuntime::reset_reverb_callback;
    callbacks.log = &LegacyDriverRuntime::log_callback;

    audio_driver_set_callbacks(&callbacks);

    //! TODO: This is still too coupled with the driver implementation. Ideally this would be a clean modular API interface for drivers where you just tell a driver that you do not know the implementation details of that it should initialize.
    external_initialize_player(nullptr);
    initialized_ = true;
    return true;
}

void LegacyDriverRuntime::tick() {
    if (initialized_) {
        external_player_handler(nullptr);
    }
}

bool LegacyDriverRuntime::is_finished() const {
    return initialized_ && external_is_finished() != 0;
}

bool LegacyDriverRuntime::submit(const engine::AudioCommand& command) {
    if (!sink_) {
        return false;
    }

    const bool accepted = sink_->submit(command);
    if (!accepted) {
        std::cerr << "Audio command queue overflow." << std::endl;
    }

    return accepted;
}

void LegacyDriverRuntime::start_voice(const usize voice, const char* sample) {
    if (!sample) {
        return;
    }

    const std::string sample_id = sample;
    const auto handle = sampler_.load_or_find_sample(sample_id, sample_id);
    if (!handle) {
        std::cerr << "Failed to load sample: " << sample_id << std::endl;
        return;
    }

    submit(engine::AudioCommand::start_voice({voice}, *handle));
}

void LegacyDriverRuntime::stop_voice(const usize voice) {
    submit(engine::AudioCommand::stop_voice({voice}));
}

void LegacyDriverRuntime::set_pitch(const usize voice, const f32 pitch) {
    submit(engine::AudioCommand::set_pitch({voice}, pitch));
}

void LegacyDriverRuntime::set_volume_ramp(const usize voice, const i16 volume, const i32 duration) {
    //! TODO: This looks ugly, especially the variable names, just consolidate this calculate without changing behavior.
    const i32 volume_new = (static_cast<i32>(volume) * static_cast<i32>(volume)) >> 15;
    const i16 volume_new_final = static_cast<i16>(volume_new);
    submit(engine::AudioCommand::set_volume_ramp({voice}, static_cast<f64>(volume_new_final) / 32767.0, duration));
}

void LegacyDriverRuntime::set_pan(const usize voice, const u8 pan) {
    submit(engine::AudioCommand::set_pan({voice}, static_cast<f64>(pan) / 127.0));
}

void LegacyDriverRuntime::set_dry_wet_mix(const usize voice, const u8 mix) {
    submit(engine::AudioCommand::set_dry_wet_mix({voice}, static_cast<f64>(mix) / 127.0));
}

void LegacyDriverRuntime::set_reverb_parameter(const i16 index, const i32 value) {
    if (index < 0) {
        return;
    }

    submit(engine::AudioCommand::set_reverb_parameter(static_cast<usize>(index), value));
}

void LegacyDriverRuntime::set_master_volume_ramp(const f32 volume, const i32 duration) {
    submit(engine::AudioCommand::set_master_volume_ramp(volume, duration));
}

void LegacyDriverRuntime::reset_reverb() {
    submit(engine::AudioCommand::reset_reverb());
}

void LegacyDriverRuntime::log(const char* message) const {
    if (message) {
        std::cerr << message << std::endl;
    }
}

void LegacyDriverRuntime::start_voice_callback(void* user_data, const usize voice, const char* sample) {
    static_cast<LegacyDriverRuntime*>(user_data)->start_voice(voice, sample);
}

void LegacyDriverRuntime::stop_voice_callback(void* user_data, const usize voice) {
    static_cast<LegacyDriverRuntime*>(user_data)->stop_voice(voice);
}

void LegacyDriverRuntime::set_pitch_callback(void* user_data, const usize voice, const f32 pitch) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_pitch(voice, pitch);
}

void LegacyDriverRuntime::set_volume_ramp_callback(void* user_data, const usize voice, const i16 volume, const i32 duration) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_volume_ramp(voice, volume, duration);
}

void LegacyDriverRuntime::set_pan_callback(void* user_data, const usize voice, const u8 pan) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_pan(voice, pan);
}

void LegacyDriverRuntime::set_dry_wet_mix_callback(void* user_data, const usize voice, const u8 mix) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_dry_wet_mix(voice, mix);
}

void LegacyDriverRuntime::set_reverb_parameter_callback(void* user_data, const i16 index, const i32 value) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_reverb_parameter(index, value);
}

void LegacyDriverRuntime::set_master_volume_ramp_callback(void* user_data, const f32 volume, const i32 duration) {
    static_cast<LegacyDriverRuntime*>(user_data)->set_master_volume_ramp(volume, duration);
}

void LegacyDriverRuntime::reset_reverb_callback(void* user_data) {
    static_cast<LegacyDriverRuntime*>(user_data)->reset_reverb();
}

void LegacyDriverRuntime::log_callback(void* user_data, const char* message) {
    static_cast<LegacyDriverRuntime*>(user_data)->log(message);
}

} // namespace simple_sampler::runtime
