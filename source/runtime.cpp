#include "runtime.hpp"

#include "dr_wav.h"
#include "sampler.hpp"

#include <miniaudio.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

namespace simple_sampler::runtime {
namespace {
    class DriverTickClock {
    public:
        DriverTickClock(engine::DriverRuntime& driver, const usize sample_rate)
            : driver_(driver),
              samples_per_tick_(std::max<usize>(1, (sample_rate * driver_tick_interval_us) / 1'000'000)),
              samples_until_tick_(samples_per_tick_) {}

        void tick() {
            --samples_until_tick_;
            if (samples_until_tick_ == 0) {
                driver_.tick();
                samples_until_tick_ = samples_per_tick_;
            }
        }

    private:
        engine::DriverRuntime& driver_;
        usize samples_per_tick_;
        usize samples_until_tick_;
    };

    class WavWriter {
    public:
        WavWriter(const char* filename, const engine::AudioConfig config) {
            drwav_data_format format;
            format.container = drwav_container_riff;
            format.format = DR_WAVE_FORMAT_IEEE_FLOAT;
            format.channels = static_cast<drwav_uint32>(config.channel_count);
            format.sampleRate = static_cast<drwav_uint32>(config.sample_rate);
            format.bitsPerSample = 32;

            initialized_ = drwav_init_file_write(&wav_, filename, &format, nullptr);
        }

        WavWriter(const WavWriter&) = delete;
        WavWriter& operator=(const WavWriter&) = delete;

        ~WavWriter() {
            if (initialized_) {
                drwav_uninit(&wav_);
            }
        }

        [[nodiscard]] explicit operator bool() const {
            return initialized_;
        }

        void write_frames(const f32* frames, const usize frame_count) {
            drwav_write_pcm_frames(&wav_, frame_count, frames);
        }

    private:
        drwav wav_ {};
        bool initialized_ = false;
    };

    [[nodiscard]] bool write_frame_to_buffer(
        std::vector<f32>& buffer,
        const usize frame,
        const engine::StereoFrame samples,
        const usize channel_count
    ) {
        if (channel_count == 1) {
            buffer[frame] = (samples.left + samples.right) * 0.5f;
            return true;
        }

        if (channel_count == 2) {
            buffer[frame * channel_count + 0] = samples.left;
            buffer[frame * channel_count + 1] = samples.right;
            return true;
        }

        return false;
    }

    [[nodiscard]] bool should_stop_rendering(
        const engine::ProcessResult& result,
        const engine::SamplerStatus& sampler_status,
        const engine::DriverRuntime& driver
    ) {
        return result.finished || (driver.is_finished() && !sampler_status.has_active_voices);
    }

    struct PlaybackContext {
        Sampler& sampler;
        engine::DriverRuntime& driver;
        engine::AudioConfig config;
        DriverTickClock driver_clock;
        bool command_queue_overflowed = false;

        PlaybackContext(Sampler& sampler, engine::DriverRuntime& driver, const engine::AudioConfig config)
            : sampler(sampler),
              driver(driver),
              config(config),
              driver_clock(driver, config.sample_rate) {}
    };

    class PlaybackDevice {
    public:
        explicit PlaybackDevice(PlaybackContext& context) {
            ma_device_config config = ma_device_config_init(ma_device_type_playback);
            config.playback.format = ma_format_f32;
            config.playback.channels = static_cast<ma_uint32>(context.config.channel_count);
            config.sampleRate = static_cast<ma_uint32>(context.config.sample_rate);
            config.dataCallback = &PlaybackDevice::data_callback;
            config.pUserData = &context;

            initialized_ = ma_device_init(nullptr, &config, &device_) == MA_SUCCESS;
        }

        PlaybackDevice(const PlaybackDevice&) = delete;
        PlaybackDevice& operator=(const PlaybackDevice&) = delete;

        ~PlaybackDevice() {
            if (started_) {
                ma_device_stop(&device_);
            }

            if (initialized_) {
                ma_device_uninit(&device_);
            }
        }

        [[nodiscard]] bool initialized() const {
            return initialized_;
        }

        [[nodiscard]] bool start() {
            if (!initialized_) {
                return false;
            }

            started_ = ma_device_start(&device_) == MA_SUCCESS;
            return started_;
        }

    private:
        //! TODO: This is still hardcoded to output f32 samples, fix this to be able to handle other formats as well.
        static void data_callback(ma_device* device, void* output, const void*, ma_uint32 frame_count) {
            auto* context = static_cast<PlaybackContext*>(device->pUserData);
            f32* output_f32 = static_cast<f32*>(output);

            if (!context) {
                std::fill_n(output_f32, frame_count * engine::default_channel_count, 0.0f);
                return;
            }

            const usize channel_count = context->config.channel_count;
            if (channel_count != 1 && channel_count != 2) {
                std::fill_n(output_f32, frame_count * channel_count, 0.0f);
                return;
            }

            for (usize frame = 0; frame < frame_count; ++frame) {
                context->driver_clock.tick();
                const engine::ProcessResult result = context->sampler.process_frame();
                context->command_queue_overflowed |= context->sampler.status().command_queue_overflowed;

                if (channel_count == 1) {
                    output_f32[frame] = (result.samples.left + result.samples.right) * 0.5f;
                } else {
                    output_f32[frame * channel_count + 0] = result.samples.left;
                    output_f32[frame * channel_count + 1] = result.samples.right;
                }
            }
        }

        ma_device device_ {};
        bool initialized_ = false;
        bool started_ = false;
    };
}

std::optional<ProgramOptions> parse_args(const int argc, char* argv[]) {
    ProgramOptions options;

    if (argc <= 1) {
        return options;
    }

    const std::string arg1 = argv[1];
    if (arg1 == "--render") {
        options.mode = ProgramMode::Render;
        if (argc > 2) {
            options.output_filename = argv[2];
        }
        return options;
    }

    return std::nullopt;
}

int render_to_file(const std::string& filename, Sampler& sampler, engine::DriverRuntime& driver, const engine::AudioConfig config) {
    if (config.channel_count != 1 && config.channel_count != 2) {
        std::cerr << "Unsupported output channel count: " << config.channel_count << std::endl;
        return -1;
    }

    std::cout << "Rendering to WAV file: " << filename << std::endl;

    WavWriter wav(filename.c_str(), config);
    if (!wav) {
        std::cerr << "Failed to initialize output WAV file." << std::endl;
        return -1;
    }

    const u32 max_duration_seconds = 10 * 60;
    const usize max_frames = max_duration_seconds * config.sample_rate;
    const usize chunk_size = 1024;

    std::vector<f32> chunk_buffer(chunk_size * config.channel_count);
    usize total_frames_rendered = 0;

    DriverTickClock driver_clock(driver, config.sample_rate);
    bool finished = false;

    std::cout << "Processing..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    while (total_frames_rendered < max_frames && !finished) {
        const usize frames_to_process = std::min(chunk_size, max_frames - total_frames_rendered);

        for (usize i = 0; i < frames_to_process; ++i) {
            driver_clock.tick();

            const engine::ProcessResult result = sampler.process_frame();
            const engine::SamplerStatus sampler_status = sampler.status();
            if (sampler_status.command_queue_overflowed) {
                std::cerr << "Audio command queue overflowed during render." << std::endl;
                return -1;
            }

            if (!write_frame_to_buffer(chunk_buffer, i, result.samples, config.channel_count)) {
                return -1;
            }

            if (should_stop_rendering(result, sampler_status, driver)) {
                finished = true;
            }
        }

        wav.write_frames(chunk_buffer.data(), frames_to_process);
        total_frames_rendered += frames_to_process;

        if (finished) break;

        if (total_frames_rendered % (config.sample_rate * 5) == 0) {
            std::cout << "Rendered " << (total_frames_rendered / config.sample_rate) << "s..." << std::endl;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Rendering complete!" << std::endl;
    std::cout << "Audio length: " << (static_cast<double>(total_frames_rendered) / static_cast<double>(config.sample_rate)) << "s" << std::endl;
    std::cout << "Time taken:   " << elapsed.count() << "s" << std::endl;
    std::cout << "Speed:        " << ((static_cast<double>(total_frames_rendered) / static_cast<double>(config.sample_rate)) / elapsed.count()) << "x realtime" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    return 0;
}

int run_realtime_playback(Sampler& sampler, engine::DriverRuntime& driver, const engine::AudioConfig config) {
    if (config.channel_count != 1 && config.channel_count != 2) {
        std::cerr << "Unsupported playback channel count: " << config.channel_count << std::endl;
        return -1;
    }

    PlaybackContext context(sampler, driver, config);
    PlaybackDevice device(context);
    if (!device.initialized()) {
        std::cerr << "Failed to initialize playback device." << std::endl;
        return -1;
    }

    if (!device.start()) {
        std::cerr << "Failed to start playback device." << std::endl;
        return -1;
    }

    std::cout << "Playback started. Press Enter to quit..." << std::endl;
    std::cin.get();

    if (context.command_queue_overflowed) {
        std::cerr << "Audio command queue overflowed during playback." << std::endl;
        return -1;
    }

    return 0;
}

} // namespace simple_sampler::runtime
