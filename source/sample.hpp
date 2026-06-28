#pragma once

#include "types.hpp"
#include "audio_types.hpp"

#include <filesystem>
#include <optional>
#include <vector>

class Sample {
public:
    bool load_wav(const std::filesystem::path& file_path);

    f32 get_sample(f64 index, f64 pitch_ratio, usize channel) const;

    [[nodiscard]] usize get_frame_count() const {
        return frame_count_;
    }

    [[nodiscard]] usize get_sample_rate() const {
        return sample_rate_;
    }

    [[nodiscard]] usize get_channel_count() const {
        return channels_;
    }

    [[nodiscard]] bool get_loop_enabled() const {
        return loop_enabled_;
    }

    [[nodiscard]] usize get_loop_start() const {
        return loop_start_;
    }

    [[nodiscard]] usize get_loop_end() const {
        return loop_end_;
    }

    [[nodiscard]] std::optional<simple_sampler::engine::LoopRegion> loop_region() const;

    [[nodiscard]] bool is_loaded() const {
        return loaded_;
    }

private:
    static std::optional<Sample> load_wav_file(const std::filesystem::path& file_path);

    std::vector<f32> data_;
    usize frame_count_ = 0;
    usize sample_rate_ = 0;
    usize channels_ = 0;

    bool loop_enabled_ = false;
    usize loop_start_ = 0, loop_end_ = 0;
    usize loop_length_ = 0;

    bool loaded_ = false;
};
