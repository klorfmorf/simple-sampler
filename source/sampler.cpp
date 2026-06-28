#include "sampler.hpp"

#include "sample_registry.hpp"
#include "sampler_engine.hpp"

Sampler::Sampler(simple_sampler::engine::AudioConfig config)
    : samples_(std::make_unique<simple_sampler::engine::SampleRegistry>(config.max_sample_count)),
      engine_(std::make_unique<simple_sampler::engine::SamplerEngine>(*samples_, config)) {}

Sampler::Sampler()
    : Sampler(simple_sampler::engine::AudioConfig {}) {}

Sampler::~Sampler() = default;

Sampler::Sampler(Sampler&&) noexcept = default;

Sampler& Sampler::operator=(Sampler&&) noexcept = default;

bool Sampler::load_sample(const std::string& id, const std::filesystem::path& file_path) {
    return samples_->load(id, file_path);
}

std::optional<simple_sampler::engine::SampleHandle> Sampler::load_or_find_sample(const std::string& id, const std::filesystem::path& file_path) {
    return samples_->load_or_find(id, file_path);
}

std::optional<simple_sampler::engine::SampleHandle> Sampler::find_sample(const std::string& id) const {
    return samples_->find(id);
}

void Sampler::start_fade_out(const isize duration) {
    submit(simple_sampler::engine::AudioCommand::set_master_volume_ramp(0.0, duration));
}

void Sampler::reset_fade() {
    submit(simple_sampler::engine::AudioCommand::set_master_volume_ramp(1.0, 0));
}

void Sampler::reset_reverb() {
    submit(simple_sampler::engine::AudioCommand::reset_reverb());
}

void Sampler::start_voice(const usize index, const std::string& sample_id) {
    const auto sample = samples_->find(sample_id);
    if (!sample) {
        return;
    }

    submit(simple_sampler::engine::AudioCommand::start_voice({index}, *sample));
}

void Sampler::stop_voice(const usize index) {
    submit(simple_sampler::engine::AudioCommand::stop_voice({index}));
}

void Sampler::release_voice(const usize index) {
    submit(simple_sampler::engine::AudioCommand::release_voice({index}));
}

void Sampler::set_voice_pitch(const usize index, const f64 pitch) {
    submit(simple_sampler::engine::AudioCommand::set_pitch({index}, pitch));
}

void Sampler::set_voice_volume(const usize index, const f64 volume) {
    submit(simple_sampler::engine::AudioCommand::set_volume_ramp({index}, volume, 0));
}

void Sampler::set_voice_volume_ramp(const usize index, const f64 volume, const isize duration_us) {
    submit(simple_sampler::engine::AudioCommand::set_volume_ramp({index}, volume, duration_us));
}

void Sampler::set_pan(const usize index, const f64 pan) {
    submit(simple_sampler::engine::AudioCommand::set_pan({index}, pan));
}

void Sampler::set_dry_wet_mix(const usize index, const f64 dry_wet_mix) {
    submit(simple_sampler::engine::AudioCommand::set_dry_wet_mix({index}, dry_wet_mix));
}

void Sampler::set_reverb_parameter(const usize index, const i32 value) {
    submit(simple_sampler::engine::AudioCommand::set_reverb_parameter(index, value));
}

bool Sampler::submit(const simple_sampler::engine::AudioCommand& command) {
    return push_command(command);
}

simple_sampler::engine::ProcessResult Sampler::process_frame() {
    drain_command_queue();
    return engine_->process_frame();
}

simple_sampler::engine::SamplerStatus Sampler::status() const {
    simple_sampler::engine::SamplerStatus current_status = engine_->status();
    current_status.command_queue_overflowed = command_queue_overflowed_;
    return current_status;
}

bool Sampler::process(f32* master_samples, const simple_sampler::engine::Output output) {
    const simple_sampler::engine::ProcessResult result = process_frame();

    if (!master_samples || !result.has_samples) {
        return result.finished;
    }

    switch (output) {
    case simple_sampler::engine::Output::Monoaural:
        master_samples[0] = (result.samples.left + result.samples.right) * 0.5f;
        break;
    case simple_sampler::engine::Output::Stereo:
        master_samples[0] = result.samples.left;
        master_samples[1] = result.samples.right;
        break;
    }

    return result.finished;
}

void Sampler::drain_command_queue() {
    while (auto command = pop_command()) {
        engine_->apply(*command);
    }
}

bool Sampler::push_command(const simple_sampler::engine::AudioCommand& command) {
    if (command_queue_size_ == command_queue_capacity) {
        command_queue_overflowed_ = true;
        return false;
    }

    command_queue_[command_queue_tail_] = command;
    command_queue_tail_ = (command_queue_tail_ + 1) % command_queue_capacity;
    ++command_queue_size_;
    return true;
}

std::optional<simple_sampler::engine::AudioCommand> Sampler::pop_command() {
    if (command_queue_size_ == 0) {
        return std::nullopt;
    }

    const simple_sampler::engine::AudioCommand command = command_queue_[command_queue_head_];
    command_queue_head_ = (command_queue_head_ + 1) % command_queue_capacity;
    --command_queue_size_;
    return command;
}
