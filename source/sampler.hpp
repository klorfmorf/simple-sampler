#pragma once

#include "types.hpp"
#include "audio_types.hpp"

#include <array>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>

namespace simple_sampler::engine {
class SampleRegistry;
class SamplerEngine;
}

class Sampler : public simple_sampler::engine::AudioCommandSink {
public:
    explicit Sampler(simple_sampler::engine::AudioConfig config);
    Sampler();
    ~Sampler();

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;

    Sampler(Sampler&&) noexcept;
    Sampler& operator=(Sampler&&) noexcept;

    bool load_sample(const std::string& id, const std::filesystem::path& file_path);
    std::optional<simple_sampler::engine::SampleHandle> load_or_find_sample(const std::string& id, const std::filesystem::path& file_path);
    std::optional<simple_sampler::engine::SampleHandle> find_sample(const std::string& id) const;

    void start_fade_out(const isize duration);
    void reset_fade();
    void reset_reverb();

    void start_voice(const usize index, const std::string& sample_id);
    void stop_voice(const usize index);
    void release_voice(const usize index);

    void set_voice_pitch(const usize index, const f64 pitch);
    void set_voice_volume(const usize index, const f64 volume);
    void set_voice_volume_ramp(const usize index, const f64 volume, const isize duration_us);
    void set_pan(const usize index, const f64 pan);
    void set_dry_wet_mix(const usize index, const f64 dry_wet_mix);
    void set_reverb_parameter(const usize index, i32 value);

    bool submit(const simple_sampler::engine::AudioCommand& command) override;
    simple_sampler::engine::ProcessResult process_frame();
    [[nodiscard]] simple_sampler::engine::SamplerStatus status() const;

    bool process(f32* master_samples, const simple_sampler::engine::Output output);

private:
    static constexpr usize command_queue_capacity = 4'096;

    void drain_command_queue();
    [[nodiscard]] bool push_command(const simple_sampler::engine::AudioCommand& command);
    [[nodiscard]] std::optional<simple_sampler::engine::AudioCommand> pop_command();

    std::unique_ptr<simple_sampler::engine::SampleRegistry> samples_;
    std::unique_ptr<simple_sampler::engine::SamplerEngine> engine_;
    std::array<simple_sampler::engine::AudioCommand, command_queue_capacity> command_queue_ {};
    usize command_queue_head_ = 0;
    usize command_queue_tail_ = 0;
    usize command_queue_size_ = 0;
    bool command_queue_overflowed_ = false;
};
