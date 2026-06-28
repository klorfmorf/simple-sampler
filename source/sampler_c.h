#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Opaque type */
typedef struct Sampler Sampler;

/* Lifetime */
Sampler* sampler_create(void);
void     sampler_destroy(const Sampler* s);

/* Samples */
bool sampler_load_sample(Sampler* s, const char* id, const char* filepath);

/* Effects */
void sampler_start_fade_out(Sampler* s, ptrdiff_t duration);
void sampler_reset_fade(Sampler* s);
void sampler_reset_reverb(Sampler* s);

/* Voices */
void sampler_start_voice(Sampler* s, const size_t index, const char* sample_id);
void sampler_stop_voice(Sampler* s, const size_t index);
void sampler_release_voice(Sampler* s, const size_t index);

/* Parameters */
void sampler_set_voice_pitch(Sampler* s, const size_t index, const double pitch);
void sampler_set_voice_volume(Sampler* s, const size_t index, const double volume);
void sampler_set_voice_volume_ramp(Sampler* s, const size_t index, const double volume, const ptrdiff_t duration_us);
void sampler_set_pan(Sampler* s, const size_t index, const double pan);
void sampler_set_dry_wet_mix(Sampler* s, const size_t index, const double dry_wet_mix);
void sampler_set_reverb_parameter(Sampler* s, const size_t index, const int32_t value);

#ifdef __cplusplus
}
#endif