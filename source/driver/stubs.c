#include "driver.h"

#include <string.h>

static AudioDriverCallbacks g_audio_callbacks;

void audio_driver_set_callbacks(const AudioDriverCallbacks *callbacks) {
    if (callbacks == NULL) {
        memset(&g_audio_callbacks, 0, sizeof(g_audio_callbacks));
        return;
    }

    g_audio_callbacks = *callbacks;
}

void alSynStartVoice(size_t voice, const char* sample) {
    if (g_audio_callbacks.start_voice != NULL) {
        g_audio_callbacks.start_voice(g_audio_callbacks.user_data, voice, sample);
    }
}

void alSynStopVoice(size_t voice) {
    if (g_audio_callbacks.stop_voice != NULL) {
        g_audio_callbacks.stop_voice(g_audio_callbacks.user_data, voice);
    }
}

void alSynSetPitch(size_t voice, f32 pitch) {
    if (g_audio_callbacks.set_pitch != NULL) {
        g_audio_callbacks.set_pitch(g_audio_callbacks.user_data, voice, pitch);
    }
}

void alSynSetVol(size_t voice, s16 volume, s32 duration) {
    if (g_audio_callbacks.set_volume_ramp != NULL) {
        g_audio_callbacks.set_volume_ramp(g_audio_callbacks.user_data, voice, volume, duration);
    }
}

void alSynSetPan(size_t voice, u8 pan) {
    if (g_audio_callbacks.set_pan != NULL) {
        g_audio_callbacks.set_pan(g_audio_callbacks.user_data, voice, pan);
    }
}

void alSynSetFXMix(size_t voice, u8 mix) {
    if (g_audio_callbacks.set_dry_wet_mix != NULL) {
        g_audio_callbacks.set_dry_wet_mix(g_audio_callbacks.user_data, voice, mix);
    }
}

void alSynSetFXParam(s16 index, void *parameters) {
    if (g_audio_callbacks.set_reverb_parameter != NULL && parameters != NULL) {
        g_audio_callbacks.set_reverb_parameter(g_audio_callbacks.user_data, index, *((s32*)parameters));
    }
}

void start_fade_out(s32 duration) {
    if (g_audio_callbacks.set_master_volume_ramp != NULL) {
        g_audio_callbacks.set_master_volume_ramp(g_audio_callbacks.user_data, 0.0f, duration);
    }
}

void reset_fade() {
    if (g_audio_callbacks.set_master_volume_ramp != NULL) {
        g_audio_callbacks.set_master_volume_ramp(g_audio_callbacks.user_data, 1.0f, 0);
    }
}

void reset_reverb() {
    if (g_audio_callbacks.reset_reverb != NULL) {
        g_audio_callbacks.reset_reverb(g_audio_callbacks.user_data);
    }
}
