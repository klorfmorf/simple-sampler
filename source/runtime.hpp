#pragma once

#include "audio_types.hpp"
#include "types.hpp"

#include <optional>
#include <string>

class Sampler;

namespace simple_sampler::runtime {

inline constexpr usize driver_tick_interval_us = 5'000;

enum class ProgramMode {
    Playback,
    Render,
};

struct ProgramOptions {
    ProgramMode mode = ProgramMode::Playback;
    std::string output_filename = "output.wav";
};

[[nodiscard]] std::optional<ProgramOptions> parse_args(int argc, char* argv[]);

int render_to_file(const std::string& filename, Sampler& sampler, engine::DriverRuntime& driver, engine::AudioConfig config);
int run_realtime_playback(Sampler& sampler, engine::DriverRuntime& driver, engine::AudioConfig config);

} // namespace simple_sampler::runtime
