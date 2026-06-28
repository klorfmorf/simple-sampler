#pragma once

#include "audio_types.hpp"

#include <array>

namespace simple_sampler::engine {

[[nodiscard]] constexpr i32 aligned_ms_samples(const f64 milliseconds, const f64 sample_rate) {
    return static_cast<i32>(milliseconds * (sample_rate / 1'000.0)) & ~0x7;
}

inline constexpr std::array<i32, 10> default_reverb_parameters = {
    1,
    aligned_ms_samples(300.0, static_cast<f64>(default_sample_rate)),
    aligned_ms_samples(0.0, static_cast<f64>(default_sample_rate)),
    aligned_ms_samples(255.0, static_cast<f64>(default_sample_rate)), /* 179 for early versions, 255 for later versions */
    12800, /* 12000, 12800 for later versions */
    0,
    0x7FFF,
    0,
    0,
    0,
};

inline constexpr ReverbPreset default_reverb_preset {
    std::span<const i32>(default_reverb_parameters),
};

} // namespace simple_sampler::engine
