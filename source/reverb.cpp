#include "reverb.hpp"

#include <cmath>
#include <numbers>
#include <algorithm>

void Reverb::DelayLine::resize_or_clear(const i32 length) {
    if (length <= 0) {
        left_.clear();
        right_.clear();
        return;
    }

    const auto size = static_cast<usize>(length);
    if (left_.size() != size) {
        left_.assign(size, 0.0);
        right_.assign(size, 0.0);
        return;
    }

    reset();
}

void Reverb::DelayLine::reset() {
    std::fill(left_.begin(), left_.end(), 0.0);
    std::fill(right_.begin(), right_.end(), 0.0);
}

Reverb::Reverb()
    : sample_rate_(48000.0),
    total_delay_length_samples_(0),
    write_pos_(0) {}

void Reverb::reset() {
    if (total_delay_length_samples_ > 0) {
        delay_line_.reset();
    }
    std::fill(lpf_state_L_.begin(), lpf_state_L_.end(), 0.0);
    std::fill(lpf_state_R_.begin(), lpf_state_R_.end(), 0.0);
    std::fill(chorus_phase_.begin(), chorus_phase_.end(), 0.0);
    write_pos_ = 0;
}

void Reverb::configure(std::span<const i32> fx_params, f64 system_sample_rate) {
    if (fx_params.size() < 2) {
        sections_.clear();
        total_delay_length_samples_ = 0;
        delay_line_.resize_or_clear(0);
        return;
    }

    sample_rate_ = system_sample_rate;

    i32 num_sections = fx_params[0];
    total_delay_length_samples_ = fx_params[1];

    if (num_sections < 0 || total_delay_length_samples_ <= 0) {
        sections_.clear();
        delay_line_.resize_or_clear(0);
        return;
    }

    if (fx_params.size() < static_cast<usize>(2 + (num_sections * 8))) {
        sections_.clear();
        total_delay_length_samples_ = 0;
        delay_line_.resize_or_clear(0);
        return;
    }

    if (total_delay_length_samples_ % 2 != 0) total_delay_length_samples_++;

    delay_line_.resize_or_clear(total_delay_length_samples_);

    sections_.resize(num_sections);
    lpf_state_L_.assign(num_sections, 0.0);
    lpf_state_R_.assign(num_sections, 0.0);
    chorus_phase_.assign(num_sections, 0.0);

    const i32* current_section_params = fx_params.data() + 2;

    for (i32 i = 0; i < num_sections; ++i) {
        ReverbSection& sec = sections_[i];
        sec.input_offset  = current_section_params[0];
        sec.output_offset = current_section_params[1];

        sec.fb_coeff   = static_cast<f64>(static_cast<i16>(current_section_params[2])) / 32768.0;
        sec.ff_coeff   = static_cast<f64>(static_cast<i16>(current_section_params[3])) / 32768.0;
        sec.gain_coeff = static_cast<f64>(static_cast<i16>(current_section_params[4])) / 32768.0;

        i32 chorus_rate_raw = current_section_params[5];
        i32 chorus_depth_raw = current_section_params[6];
        sec.lp_coeff   = static_cast<f64>(static_cast<i16>(current_section_params[7])) / 32768.0;

        sec.chorus_rate_rad_per_sample = (static_cast<f64>(chorus_rate_raw) / std::pow(2.0, 25)) * (2.0 * std::numbers::pi);

        if (chorus_depth_raw != 0) {
            f64 cents_val = static_cast<f64>(chorus_depth_raw) / 100.0;
            f64 nominal_delay = static_cast<f64>(std::abs(sec.output_offset - sec.input_offset));
            if(nominal_delay < 1.0) nominal_delay = 100.0;

            sec.chorus_depth_samples = nominal_delay * (std::pow(2.0, cents_val / 1200.0) - 1.0);
        } else {
            sec.chorus_depth_samples = 0.0;
        }
        current_section_params += 8;
    }
    reset();
}

inline f64 hermite_interp(f64 x, f64 y0, f64 y1, f64 y2, f64 y3) {
    f64 c0 = y1;
    f64 c1 = 0.5 * (y2 - y0);
    f64 c2 = y0 - 2.5 * y1 + 2.0 * y2 - 0.5 * y3;
    f64 c3 = 0.5 * (y3 - y0) + 1.5 * (y1 - y2);
    return ((c3 * x + c2) * x + c1) * x + c0;
}

f64 Reverb::read_at_offset(const std::vector<f64>& buffer, f64 offset) const {
    f64 read_pos = static_cast<f64>(write_pos_) + offset;

    f64 size_f = static_cast<f64>(total_delay_length_samples_);

    if (read_pos >= size_f) read_pos -= size_f;
    else if (read_pos < 0.0) read_pos += size_f;

    if (read_pos >= size_f || read_pos < 0.0) {
        read_pos = std::fmod(read_pos, size_f);
        if (read_pos < 0.0) read_pos += size_f;
    }

    i32 i1 = static_cast<i32>(read_pos);
    f64 frac = read_pos - static_cast<f64>(i1);

    i32 N = total_delay_length_samples_;
    i32 i0 = (i1 - 1 + N) % N;
    i32 i2 = (i1 + 1) % N;
    i32 i3 = (i1 + 2) % N;

    return hermite_interp(frac, buffer[i0], buffer[i1], buffer[i2], buffer[i3]);
}

void Reverb::process(f64 dry_L, f64 dry_R, f64& wet_L, f64& wet_R) {
    if (sections_.empty() || total_delay_length_samples_ == 0) {
        wet_L = 0.0; wet_R = 0.0;
        return;
    }

    delay_line_.left()[write_pos_] = dry_L;
    delay_line_.right()[write_pos_] = dry_R;

    wet_L = 0.0;
    wet_R = 0.0;

    for (size_t i = 0; i < sections_.size(); ++i) {
        const auto& p = sections_[i];

        bool has_chorus = (p.chorus_rate_rad_per_sample != 0.0 || p.chorus_depth_samples != 0.0);

        chorus_phase_[i] += p.chorus_rate_rad_per_sample;
        if (chorus_phase_[i] > 2.0 * std::numbers::pi) chorus_phase_[i] -= 2.0 * std::numbers::pi;

        f64 lfo_L = std::sin(chorus_phase_[i]);
        f64 lfo_R = std::sin(chorus_phase_[i] + std::numbers::pi);

        f64 mod_L = lfo_L * p.chorus_depth_samples;
        f64 mod_R = lfo_R * p.chorus_depth_samples;

        f64 tap_input    = static_cast<f64>(p.input_offset);
        f64 tap_output_L = static_cast<f64>(p.output_offset) + mod_L;
        f64 tap_output_R = static_cast<f64>(p.output_offset) + mod_R;

        f64 buff1_L = read_at_offset(delay_line_.left(), tap_input);
        f64 buff1_R = read_at_offset(delay_line_.right(), tap_input);

        f64 buff2_L = read_at_offset(delay_line_.left(), tap_output_L);
        f64 buff2_R = read_at_offset(delay_line_.right(), tap_output_R);

        buff2_L += buff1_L * p.ff_coeff;
        buff2_R += buff1_R * p.ff_coeff;

        lpf_state_L_[i] = (1.0 - p.lp_coeff) * buff2_L + p.lp_coeff * lpf_state_L_[i];
        lpf_state_R_[i] = (1.0 - p.lp_coeff) * buff2_R + p.lp_coeff * lpf_state_R_[i];

        buff2_L = lpf_state_L_[i];
        buff2_R = lpf_state_R_[i];

        f64 fb_val_L = buff1_L + buff2_L * p.fb_coeff;
        f64 fb_val_R = buff1_R + buff2_R * p.fb_coeff;

        i32 idx_input = (write_pos_ + p.input_offset) % total_delay_length_samples_;
        if (idx_input < 0) idx_input += total_delay_length_samples_;

        delay_line_.left()[idx_input] = fb_val_L;
        delay_line_.right()[idx_input] = fb_val_R;

        if (!has_chorus) {
            i32 idx_output = (write_pos_ + p.output_offset) % total_delay_length_samples_;
            if (idx_output < 0) idx_output += total_delay_length_samples_;

            delay_line_.left()[idx_output] = buff2_L;
            delay_line_.right()[idx_output] = buff2_R;
        }

        wet_L += buff2_L * p.gain_coeff;
        wet_R += buff2_R * p.gain_coeff;
    }

    write_pos_--;
    if (write_pos_ < 0) write_pos_ += total_delay_length_samples_;
}
