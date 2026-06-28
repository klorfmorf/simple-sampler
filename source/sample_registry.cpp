#include "sample_registry.hpp"

#include <algorithm>
#include <ranges>

namespace simple_sampler::engine {

SampleRegistry::SampleRegistry(const usize max_sample_count)
    : samples_(max_sample_count) {}

bool SampleRegistry::load(const std::string_view id, const std::filesystem::path& file_path) {
    if (sample_map_.contains(std::string(id))) {
        return false;
    }

    Sample* sample = find_free_sample();
    if (!sample) {
        return false;
    }

    if (!sample->load_wav(file_path)) {
        return false;
    }

    const auto sample_index = static_cast<usize>(sample - samples_.data());
    sample_map_.insert({std::string(id), SampleHandle{sample_index}});
    return true;
}

std::optional<SampleHandle> SampleRegistry::load_or_find(const std::string_view id, const std::filesystem::path& file_path) {
    if (const auto existing = find(id)) {
        return existing;
    }

    if (!load(id, file_path)) {
        return std::nullopt;
    }

    return find(id);
}

std::optional<SampleHandle> SampleRegistry::find(const std::string_view id) const {
    const auto iterator = sample_map_.find(std::string(id));
    if (iterator == sample_map_.end()) {
        return std::nullopt;
    }

    return iterator->second;
}

const Sample* SampleRegistry::sample_at(const SampleHandle sample) const {
    if (!sample.valid() || sample.value >= samples_.size()) {
        return nullptr;
    }

    return &samples_[sample.value];
}

Sample* SampleRegistry::find_free_sample() {
    const auto iterator = std::ranges::find_if(samples_, [](const Sample& sample) {
        return !sample.is_loaded();
    });

    if (iterator == samples_.end()) {
        return nullptr;
    }

    return &*iterator;
}

} // namespace simple_sampler::engine
