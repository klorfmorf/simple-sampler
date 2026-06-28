#pragma once

#include "types.hpp"

#include <span>
#include <vector>

// Helper struct to store parsed parameters for one reverb section
struct ReverbSection {
    // Memory offsets relative to the base pointer (simulated by write_pos_)
    i32 input_offset;  
    i32 output_offset; 

    // Coefficients (Normalised -1.0 to 1.0)
    f64 fb_coeff;   // Feedback
    f64 ff_coeff;   // Feedforward
    f64 gain_coeff; // Output Gain
    f64 lp_coeff;   // Low Pass Filter Coefficient

    // Chorus / Modulation
    f64 chorus_rate_rad_per_sample; // LFO phase increment
    f64 chorus_depth_samples;       // LFO modulation depth
};

class Reverb {
public:
    Reverb();

    /**
    * @brief Configures the reverb based on N64-style integer parameter arrays.
    * 
    * @param fx_params Pointer to the raw i32 array (e.g., CUSTOM_REVERB_PARAMS_COMPLEX).
    * @param system_sample_rate The actual sample rate the engine is running at (e.g., 48000.0).
    */
    void configure(std::span<const i32> fx_params, f64 system_sample_rate);

    /**
    * @brief Processes a stereo sample pair.
    * 
    * @param dry_L Input Left (Mono mix or Left channel)
    * @param dry_R Input Right (Mono mix or Right channel)
    * @param wet_L [Out] Reverb signal Left
    * @param wet_R [Out] Reverb signal Right
    */
    void process(f64 dry_L, f64 dry_R, f64& wet_L, f64& wet_R);

    /**
    * @brief Clears delay lines and filter states to silence.
    */
    void reset(); 

private:
    class DelayLine {
    public:
        void resize_or_clear(i32 length);
        void reset();

        [[nodiscard]] bool empty() const {
            return left_.empty();
        }

        [[nodiscard]] i32 size() const {
            return static_cast<i32>(left_.size());
        }

        [[nodiscard]] std::vector<f64>& left() {
            return left_;
        }

        [[nodiscard]] std::vector<f64>& right() {
            return right_;
        }

        [[nodiscard]] const std::vector<f64>& left() const {
            return left_;
        }

        [[nodiscard]] const std::vector<f64>& right() const {
            return right_;
        }

    private:
        std::vector<f64> left_;
        std::vector<f64> right_;
    };

    f64 sample_rate_;
    std::vector<ReverbSection> sections_;

    DelayLine delay_line_;
    i32 total_delay_length_samples_;
    i32 write_pos_; // The moving "head" of the circular buffer

    // Per-Section State
    std::vector<f64> lpf_state_L_;  // Left Channel Low-Pass State
    std::vector<f64> lpf_state_R_;  // Right Channel Low-Pass State
    std::vector<f64> chorus_phase_; // Current LFO phase (radians) per section

    // Interpolated read helper
    // Reads from buffer at (write_pos_ + offset) % size
    f64 read_at_offset(const std::vector<f64>& buffer, f64 offset) const;
};
