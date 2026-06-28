#include "sampler_c.h"
#include "sampler.hpp"

extern "C" {

Sampler* sampler_create(void) {
    return new Sampler();
}

void sampler_destroy(const Sampler* s) {
    delete s;
}

bool sampler_load_sample(Sampler* s, const char* id, const char* filepath) {
    return s->load_sample(id, filepath);
}

void sampler_start_fade_out(Sampler* s, const ptrdiff_t duration) {
    s->start_fade_out(duration);
}

void sampler_reset_fade(Sampler* s) {
    s->reset_fade();
}

void sampler_reset_reverb(Sampler* s) {
    s->reset_reverb();
}

void sampler_start_voice(Sampler* s, const size_t index, const char* sample_id) {
    return s->start_voice(index, sample_id);
}

void sampler_stop_voice(Sampler* s, const size_t index) {
    s->stop_voice(index);
}

void sampler_release_voice(Sampler* s, const size_t index) {
    s->release_voice(index);
}

void sampler_set_voice_pitch(Sampler* s, const size_t index, const double pitch) {
    s->set_voice_pitch(index, pitch);
}

void sampler_set_voice_volume(Sampler* s, const size_t index, const double volume) {
    s->set_voice_volume(index, volume);
}

void sampler_set_voice_volume_ramp(Sampler* s, const size_t index, const double volume, const ptrdiff_t duration_us) {
    s->set_voice_volume_ramp(index, volume, duration_us);
}

void sampler_set_pan(Sampler* s, const size_t index, const double pan) {
    s->set_pan(index, pan);
}

void sampler_set_dry_wet_mix(Sampler* s, const size_t index, const double dry_wet_mix) {
    s->set_dry_wet_mix(index, dry_wet_mix);
}

void sampler_set_reverb_parameter(Sampler* s, const size_t index, const int32_t value)  {
    s->set_reverb_parameter(index, value);
}

} // extern "C"