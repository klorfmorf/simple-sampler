#include "legacy_driver_runtime.hpp"
#include "runtime.hpp"
#include "sampler.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    const auto options = simple_sampler::runtime::parse_args(argc, argv);
    if (!options) {
        std::cerr << "Usage: Sampler [--render [filename.wav]]" << std::endl;
        return 1;
    }

    simple_sampler::engine::AudioConfig config;
    Sampler sampler(config);
    simple_sampler::runtime::LegacyDriverRuntime driver(sampler);

    if (!driver.initialize(sampler)) {
        std::cerr << "Error: Failed to initialize legacy audio driver." << std::endl;
        return -1;
    }

    switch (options->mode) {
    case simple_sampler::runtime::ProgramMode::Render:
        return simple_sampler::runtime::render_to_file(options->output_filename, sampler, driver, config);

    case simple_sampler::runtime::ProgramMode::Playback:
        return simple_sampler::runtime::run_realtime_playback(sampler, driver, config);
    }

    return 1;
}
