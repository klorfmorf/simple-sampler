#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define USE_OLD_INST_DRUM_FORMAT

/* Should the debug print statements have color? */
#define DEBUG_COLOR 1

#if DEBUG_COLOR
#define KNRM  "\x1B[0m"   // Reset
#define KRED  "\x1B[31m"  // Red
#define KGRN  "\x1B[32m"  // Green
#define KYEL  "\x1B[33m"  // Yellow
#define KBLU  "\x1B[34m"  // Blue
#define KMAG  "\x1B[35m"  // Magenta
#define KCYN  "\x1B[36m"  // Cyan
#define KWHT  "\x1B[37m"  // White
#else
#define KNRM  ""  // Reset
#define KRED  ""  // Red
#define KGRN  ""  // Green
#define KYEL  ""  // Yellow
#define KBLU  ""  // Blue
#define KMAG  ""  // Magenta
#define KCYN  ""  // Cyan
#define KWHT  ""  // White
#endif

typedef struct AudioDriverCallbacks
{
    void *user_data;
    void (*start_voice)(void *user_data, size_t voice, const char *sample);
    void (*stop_voice)(void *user_data, size_t voice);
    void (*set_pitch)(void *user_data, size_t voice, f32 pitch);
    void (*set_volume_ramp)(void *user_data, size_t voice, s16 volume, s32 duration);
    void (*set_pan)(void *user_data, size_t voice, u8 pan);
    void (*set_dry_wet_mix)(void *user_data, size_t voice, u8 mix);
    void (*set_reverb_parameter)(void *user_data, s16 index, s32 value);
    void (*set_master_volume_ramp)(void *user_data, f32 volume, s32 duration);
    void (*reset_reverb)(void *user_data);
    void (*log)(void *user_data, const char *message);
} AudioDriverCallbacks;

void audio_driver_set_callbacks(const AudioDriverCallbacks *callbacks);
void alSynStartVoice(size_t voice, const char* sample);
void alSynStopVoice(size_t voice);
void alSynSetPitch(size_t voice, f32 pitch);
void alSynSetVol(size_t voice, s16 volume, s32 duration);
void alSynSetPan(size_t voice, u8 pan);
void alSynSetFXMix(size_t voice, u8 mix);
void alSynSetFXParam(s16 index, void *parameters);
void start_fade_out(s32 duration);
void reset_fade();
void reset_reverb();

#define FALSE 0
#define TRUE 1

#define FREQUENCY 44100

#define AUDIO_LIST_COUNT 2

#define AUDIO_TASK_COUNT 3

#define RETRACE_MESSAGE_BUFFER_COUNT 64

#define TASK_MESSAGE_BUFFER_COUNT 64

#define DMA_MESSAGE_BUFFER_COUNT 64

#define DMA_IO_MESSAGE_BUFFER_COUNT 64

#define AUDIO_THREAD_STACK_COUNT 0x6000

#define JITTER_SAMPLES 0x100

#define DMA_BUFFER_COUNT 24

#define AUDIO_HEAP_SIZE 0x4E000

#define CALLBACK_TIME 5000

#define INITIAL_DELAY 6

// MNSG: 3
//  GGA: 4
#define BANK_COUNT 3

#define REQUESTS_COUNT 8

#define CHANNEL_COUNT 16

#define VOLUME_IS_CHANGING (1 << 0)
#define PITCH_IS_CHANGING (1 << 1)
#define IS_DRUM (1 << 2)

typedef enum EnvelopePhase
{
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
} EnvelopePhase;

typedef enum PitchChangeType
{
    FIXED_LENGTH,
    VARIABLE_LENGTH
} PitchChangeType;

typedef struct Channel
{
    u32 sequence_id;
    s8 priority;
    s8 initial_delay_counter;
    u8 flags;
    u8 unknown_4;
    u8 *sequence_data_needle;
    u8 bank;
    u8 sound;
    u8 voice_status;
    s8 dry_wet_mix;
    void *wave_table; // ALWaveTable changed!
    u16 tempo;
    u16 tempo_counter;
    u8 tempo_change_target;
    u8 tempo_change_duration;
    s16 tempo_change_amount;
    u8 previous_note_duration;
    u8 note_duration;
    u8 note_hold_ratio;
    u8 previous_note_hold_ratio;
    s8 note_hold_duration;
    u8 note_pitch;
    u8 unknown_22;
    u8 unknown_23;
    u32 pitch;
    s16 sound_transpose_detune;
    s16 effect_transpose;
    s16 effect_detune;
    s16 marker_transpose;
    s16 nested_marker_transpose;
    s16 vibrato_pitch;
    s16 random_pitch;
    u16 volume;
    u8 unknown_33;
    u8 unknown_34;
    u8 unknown_35;
    u8 unknown_36;
    s16 marker_volume;
    s16 nested_marker_volume;
    s16 volume_change_amount;
    u8 volume_change_target;
    u8 volume_change_duration;
    u8 note_velocity;
    u8 envelope_phase;
    u16 envelope_volume_target;
    u16 envelope_phase_duration;
    u16 envelope_attack_duration;
    u16 envelope_decay_duration;
    u16 envelope_sustain_duration;
    s16 envelope_decay_volume;
    u16 envelope_release_duration;
    u16 pan;
    u8 pan_change_target;
    u8 pan_change_duration;
    s16 pan_change_amount;
    s8 marker_loop_counter;
    u8 unknown_56;
    u8 *marker_needle;
    s8 nested_marker_loop_counter;
    u8 unknown_59;
    u8 unknown_60;
    u8 unknown_61;
    u8 *nested_marker_needle;
    u8 *master_marker_needle;
    s8 conditional_marker_flag;
    u8 unknown_65;
    u8 unknown_66;
    u8 unknown_67;
    u8 *conditional_marker_needle_1;
    u8 *conditional_marker_needle_2;
    s8 pitch_change_delay_duration;
    u8 pitch_change_duration;
    u8 unknown_72;
    u8 unknown_73;
    u32 pitch_change_target;
    s32 pitch_change_amount;
    u8 pitch_change_next_note_delay_duration;
    u8 pitch_change_next_note_duration;
    s16 pitch_change_next_note_parameter;
    u8 pitch_change_type;
    u8 random_index;
    u16 random_pitch_mask;
    u16 random_counter;
    u8 random_rate;
    u8 vibrato_intended_delay_duration;
    s8 vibrato_delay_duration;
    u8 vibrato_rate;
    u16 vibrato_counter;
    u8 vibrato_oscillation;
    u8 vibrato_oscillation_step_size;
    u16 vibrato_depth_target;
    u16 vibrato_depth;
    u8 vibrato_intended_start_duration;
    s8 vibrato_start_duration;
    u16 vibrato_depth_change_amount;
    u8 unknown_95;
    u8 unknown_96;

    /* new mnsg */
    u8 disable;

    /* for muting sequence voices */
    u8 vol_mult;

    /* for special transpose */
    s16 special_transpose;

    /* for special bank */
    u8 special_bank_used;
    u8 previous_bank;
} Channel;

typedef struct Sound
{
    s8 transpose;
    s8 detune;
    s8 attack;
    u8 decay;
    u8 sustain;
    s8 release;
    s8 pan;
#ifdef USE_OLD_INST_DRUM_FORMAT
    u8 unknown; // Early versions only.
#endif
} Sound;

typedef struct DrumSound
{
#ifdef USE_OLD_INST_DRUM_FORMAT
    u8 bank; // Early versions only.
#endif
    u8 id;
    Sound sound;
} DrumSound;

typedef struct AudioConfig
{
    u32 frequency;
    u32 samples_per_frame_coefficient;
    u32 audio_list_size;
} AudioConfig;

/* ----- external Function Prototypes ----- */

void external_initialize_player(void *player);
s32 external_player_handler(void *player);
u8 external_is_finished(void);
void external_initialize_voices();
void external_initialize_channels(u8 *data, u8 tracks, u16 id); // TEMPORARY

/* ----- player Function Prototypes  ----- */
void player_update_channels();
void player_update_channel();
void player_read_data();
void player_update_on_tick();
void player_fdsdfsfsdf();
void player_update_off_tick();
void player_set_pitch();
void player_set_volume();
void player_start_envelope_attack_phase();
void player_start_envelope_decay_phase();
void player_start_envelope_sustain_phase();
void player_start_envelope_release_phase();
void player_start_envelope_cutoff_phase();
void player_update_envelope_phase();

/* ----- effects Function Prototypes  ----- */
void effects_set_tempo();
void effects_set_tempo_change();
void effects_update_tempo_change();
void effects_set_sound();
void effects_load_sound(Sound *sound);
void effects_load_drum_sound();
void effects_set_volume_sound();
void effects_set_volume_sound_pan();
void effects_set_volume();
void effects_set_volume_change();
void effects_update_volume_change();
void effects_set_attack_decay();
void effects_set_sustain();
void effects_set_release();
void effects_set_reverb();
void effects_set_user_defined();
void effects_set_pan();
void effects_set_pan_(); // Figure out a name for this.
void effects_set_pan_change();
void effects_update_pan_change();
void effects_set_transpose();
void effects_set_detune();
void effects_set_vibrato();
void effects_initialize_vibrato();
void effects_update_vibrato_on_tick();
void effects_update_vibrato();
void effects_set_vibrato_change();
void effects_set_random();
void effects_update_random();
void effects_set_pitch_bend_dummy();
void effects_calculate_pitch_change_amount();
void effects_set_pitch_bend();
void effects_update_pitch_change_on_tick();
void effects_update_pitch_change_off_tick();
void effects_set_pitch_bend_start();
void effects_calculate_pitch_change_amount_this_note(); // Maybe this needs a different name.
void effects_set_portamento();
void effects_update_portamento();
void effects_set_marker();
void effects_go_to_marker();
void effects_set_nested_marker();
void effects_go_to_nested_marker();
void effects_set_master_marker();
void effects_go_to_master_marker();
void effects_set_conditional_marker();
void effects_go_to_conditional_marker();
void effects_set_echo();
void effects_set_bank();
void effects_set_nothing();
void effects_rest();
void effects_tie();
void effects_F8();
void effects_F9();
void effects_FA();
void effects_FB();
void effects_FC();
void effects_FD();
void effects_FE();
void effects_FC_FD_FE_sub(u8 arg);
void effects_end();

#ifdef __cplusplus
}
#endif

#endif // AUDIO_H
