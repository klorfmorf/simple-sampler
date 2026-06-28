#include "sample.hpp"

#include "dr_wav.h"

#include <numbers>
#include <cmath>
#include <optional>

//! TODO: Organize this constant properly.
const isize SINC_WINDOW_RADIUS = 32;

namespace {
    inline f64 linear_to_decibels(const f64 x) {
        return 20.0 * std::log10(x);
    }

    inline f64 normalized_sinc(const f64 x) {
        if (std::abs(x) < 1e-9) return 1.0;
        const f64 pi_x = std::numbers::pi * x;

        return std::sin(pi_x) / pi_x;
    }

    inline f64 blackman_window(const f64 x) {
        if (std::abs(x) >= 1.0) return 0.0;
        return 0.42 + 0.5 * std::cos(std::numbers::pi * x) + 0.08 * std::cos(2.0 * std::numbers::pi * x);
    }

    inline f64 nuttall_window(const f64 x) {
        if (std::abs(x) >= 1.0) return 0.0;

        const f64 pi_x = std::numbers::pi * x;

        return 0.355768
            + 0.487396 * std::cos(pi_x)
            + 0.144232 * std::cos(2.0 * pi_x)
            + 0.012604 * std::cos(3.0 * pi_x);
    }

    inline f64 nuttall_window_one_cos(const f64 x) {
        if (std::abs(x) >= 1.0) return 0.0;

        const f64 pi_x = std::numbers::pi * x;

        const f64 c1 = std::cos(pi_x);

        const f64 c1_sq = c1 * c1;
        const f64 c2 = 2.0 * c1_sq - 1.0;

        const f64 c3 = 4.0 * c1 * c1_sq - 3.0 * c1;

        return 0.355768
            + 0.487396 * c1
            + 0.144232 * c2
            + 0.012604 * c3;
    }

    inline f64 fast_i0(f64 x) {
        f64 sum = 1.0;
        f64 t = 1.0;
        const f64 x2 = x * x * 0.25;

        for (int i = 1; i <= 25; ++i) {
            t *= x2 / (static_cast<f64>(i) * static_cast<f64>(i));
            sum += t;

            if (t < sum * 1e-15) break;
        }
        return sum;
    }

    inline f64 kaiser_window_approximate(const f64 x) {
        static const f64 KAISER_BETA = 12.0;
        static const f64 KAISER_DENOM = std::cyl_bessel_i(0.0, KAISER_BETA);

        const f64 arg = KAISER_BETA * std::sqrt(1.0 - (x * x));
        return fast_i0(arg) / KAISER_DENOM;
    }

    inline f64 kaiser_window(const f64 x) {
        if (std::abs(x) >= 1.0) return 0.0;

        constexpr f64 beta = 18.0;

        static const f64 i0_beta = std::cyl_bessel_i(0.0, beta);

        const f64 arg = beta * std::sqrt(1.0 - (x * x));
        return std::cyl_bessel_i(0.0, arg) / i0_beta;
    }

    class WavFile {
    public:
        explicit WavFile(const std::filesystem::path& file_path) {
            initialized_ = drwav_init_file_with_metadata(&wav_, file_path.c_str(), 0, nullptr);
        }

        WavFile(const WavFile&) = delete;
        WavFile& operator=(const WavFile&) = delete;

        WavFile(WavFile&&) = delete;
        WavFile& operator=(WavFile&&) = delete;

        ~WavFile() {
            if (initialized_) {
                drwav_uninit(&wav_);
            }
        }

        [[nodiscard]] explicit operator bool() const {
            return initialized_;
        }

        [[nodiscard]] drwav& get() {
            return wav_;
        }

    private:
        drwav wav_ {};
        bool initialized_ = false;
    };

    inline isize euclidean_mod(isize a, isize b) {
        return ((a % b) + b) % b;
    }

}

bool Sample::load_wav(const std::filesystem::path& file_path) {
    auto loaded_sample = Sample::load_wav_file(file_path);
    if (!loaded_sample) {
        return false;
    }

    *this = std::move(*loaded_sample);
    return true;
}

std::optional<Sample> Sample::load_wav_file(const std::filesystem::path& file_path) {
    WavFile wav_file(file_path);
    if (!wav_file) {
        return std::nullopt;
    }

    drwav& wav = wav_file.get();
    Sample sample;

    sample.data_.resize(static_cast<usize>(wav.totalPCMFrameCount) * wav.channels);
    const usize frames_read = drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, sample.data_.data());
    sample.data_.resize(frames_read * wav.channels);

    for (usize index = 0; index < wav.metadataCount; ++index) {
        if (wav.pMetadata[index].type == drwav_metadata_type_smpl) {
            const drwav_smpl* smpl = &wav.pMetadata[index].data.smpl;

            if (smpl->sampleLoopCount > 0) {
                const drwav_smpl_loop* smpl_loop = &smpl->pLoops[0];

                if (smpl_loop->type == drwav_smpl_loop_type_forward) {
                    sample.loop_enabled_ = true;
                    sample.loop_start_   = smpl_loop->firstSampleOffset;
                    sample.loop_end_     = smpl_loop->lastSampleOffset;

                    /*
                    If your WAVs have a duplicated sample at the end (off-by-one export),
                    uncomment the line below.
                    Standard WAVs use inclusive endpoints (Start to End).
                    */
                    if (sample.loop_end_ > sample.loop_start_) sample.loop_end_--;

                    if (sample.loop_end_ >= frames_read) {
                        sample.loop_end_ = frames_read > 0 ? frames_read - 1 : 0;
                    }

                    if (sample.loop_start_ > sample.loop_end_ || sample.loop_start_ >= frames_read) {
                        sample.loop_enabled_ = false;
                    } else {
                        sample.loop_length_ = static_cast<isize>(sample.loop_end_ - sample.loop_start_ + 1);
                        if (sample.loop_length_ <= 0) sample.loop_enabled_ = false;
                    }
                }
            }
        }
    }

    sample.frame_count_ = frames_read;
    sample.sample_rate_ = wav.sampleRate;
    sample.channels_    = wav.channels;
    sample.loaded_      = true;

    // See https://github.com/ares-emulator/ares/blob/d45be3297f375916142a1ae15f7dfe56cd20bcba/ares/n64/ai/io.cpp#L58
    // Also see https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/src/io/aisetfreq.c#L40
    const usize video_clock_frequency = 48'681'818;

    sample.sample_rate_ = video_clock_frequency / static_cast<usize>((video_clock_frequency / static_cast<f64>(sample.sample_rate_)) + 0.5);

    return sample;
}

std::optional<simple_sampler::engine::LoopRegion> Sample::loop_region() const {
    if (!loop_enabled_) {
        return std::nullopt;
    }

    return simple_sampler::engine::LoopRegion{loop_start_, loop_end_};
}

f32 Sample::get_sample(const f64 index, const f64 pitch_ratio, const usize channel) const {
    if (data_.empty() || channel >= channels_ || SINC_WINDOW_RADIUS <= 0) {
        return 0.0f;
    }

    f64 cutoff_freq_factor = 1.0;
    f64 effective_radius   = static_cast<f64>(SINC_WINDOW_RADIUS);

    if (pitch_ratio > 1.0) {
        cutoff_freq_factor = 1.0 / pitch_ratio;
        effective_radius  *= pitch_ratio;
    }

    f64 sum = 0.0;

    const isize start_k = static_cast<isize>(std::floor(index - effective_radius));
    const isize end_k   = static_cast<isize>(std::ceil(index + effective_radius));

    bool is_safe_window = false;

    if (loop_enabled_) {
        const isize i_start = static_cast<isize>(loop_start_);
        const isize i_end   = static_cast<isize>(loop_end_);

        if (index >= static_cast<f64>(loop_start_)) {
            if (start_k >= i_start && end_k <= i_end) {
                is_safe_window = true;
            }
        } else {
            if (start_k >= 0 && end_k <= i_end) {
                is_safe_window = true;
            }
        }
    } else {
        if (start_k >= 0 && end_k < static_cast<isize>(frame_count_)) {
            is_safe_window = true;
        }
    }

    if (is_safe_window) {
        for (isize k = start_k; k <= end_k; ++k) {
            const f64 time_diff  = index - static_cast<f64>(k);
            const f64 window_arg = time_diff / effective_radius;

            if (std::abs(window_arg) >= 1.0) continue;

            const f64 sinc_arg     = time_diff * cutoff_freq_factor;
            const f64 kernel_value = normalized_sinc(sinc_arg) * nuttall_window_one_cos(window_arg);

            sum += data_[static_cast<usize>(k) * channels_ + channel] * kernel_value;
        }
    } else {
        for (isize k_unmapped = start_k; k_unmapped <= end_k; ++k_unmapped) {
            const f64 time_diff  = index - static_cast<f64>(k_unmapped);
            const f64 window_arg = time_diff / effective_radius;

            if (std::abs(window_arg) >= 1.0) continue;

            const f64 sinc_arg     = time_diff * cutoff_freq_factor;
            const f64 kernel_value = normalized_sinc(sinc_arg) * nuttall_window_one_cos(window_arg);

            isize k_mapped = k_unmapped;

            if (loop_enabled_) {
                const isize i_start = static_cast<isize>(loop_start_);
                const isize i_end   = static_cast<isize>(loop_end_);
                const isize i_len   = static_cast<isize>(loop_length_);

                if (index >= static_cast<f64>(loop_start_)) {
                    isize offset = k_mapped - i_start;
                    k_mapped = i_start + euclidean_mod(offset, i_len);
                } else {
                    if (k_mapped > i_end) {
                        isize offset = k_mapped - i_start;
                        k_mapped = i_start + euclidean_mod(offset, i_len);
                    }
                }
            }

            if (k_mapped >= 0 && k_mapped < static_cast<isize>(frame_count_)) {
                sum += data_[static_cast<usize>(k_mapped) * channels_ + channel] * kernel_value;
            }
        }
    }

    return static_cast<f32>(sum * cutoff_freq_factor);
}
