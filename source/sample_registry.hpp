#pragma once

#include "audio_types.hpp"
#include "sample.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace simple_sampler::engine {

class SampleRegistry {
public:
    explicit SampleRegistry(usize max_sample_count = default_max_sample_count);

    [[nodiscard]] bool load(std::string_view id, const std::filesystem::path& file_path);
    [[nodiscard]] std::optional<SampleHandle> load_or_find(std::string_view id, const std::filesystem::path& file_path);
    [[nodiscard]] std::optional<SampleHandle> find(std::string_view id) const;
    [[nodiscard]] const Sample* sample_at(SampleHandle sample) const;

private:
    [[nodiscard]] Sample* find_free_sample();

    std::vector<Sample> samples_;
    std::unordered_map<std::string, SampleHandle> sample_map_;
};

} // namespace simple_sampler::engine
