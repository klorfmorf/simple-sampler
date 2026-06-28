#include "driver.h"

#include <stdio.h>

#define printf(...) do { } while (0)

#define CURRENT_SAMPLE_BANK s_sample_file_paths_gga_banks

/* ----- externs ----- */
extern u8 g_paused;
extern u8 g_tempo_increase_amount;
extern u8 g_fade_amount;

/* ----- .data ----- */
void (*g_command_table[])() =
{
    effects_set_tempo, // 0xD0
    effects_set_tempo_change, // 0xD1
    effects_set_sound, // 0xD2
    effects_set_volume_sound, // 0xD3
    effects_set_volume_sound_pan, // 0xD4
    effects_set_volume, // 0xD5
    effects_set_volume_change, // 0xD6
    effects_set_attack_decay, // 0xD7
    effects_set_sustain, // 0xD8
    effects_set_release, // 0xD9
    effects_set_reverb, // 0xDA
    effects_end, // 0xDB
    effects_set_user_defined, // 0xDC
    effects_set_pan, // 0xDD
    effects_set_pan_change, // 0xDE
    effects_set_transpose, // 0xDF
    effects_set_detune, // 0xE0
    effects_set_vibrato, // 0xE1
    effects_set_vibrato_change, // 0xE2
    effects_set_random, // 0xE3
    effects_set_pitch_bend_dummy, // 0xE4
    effects_set_pitch_bend_start, // 0xE5
    effects_set_portamento, // 0xE6
    effects_set_marker, // 0xE7
    effects_go_to_marker, // 0xE8
    effects_set_nested_marker, // 0xE9
    effects_go_to_nested_marker, // 0xEA
    effects_set_master_marker, // 0xEB
    effects_go_to_master_marker, // 0xEC
    effects_set_conditional_marker, // 0xED
    effects_go_to_conditional_marker, // 0xEE
    effects_set_echo, // 0xEF
    effects_set_bank, // 0xF0
    effects_set_nothing, // 0xF1
    effects_rest, // 0xF2
    effects_tie, // 0xF3
    effects_end, // 0xF4
    effects_end, // 0xF5
    effects_end, // 0xF6
    effects_end, // 0xF7
    effects_F8, // 0xF8
    effects_F9, // 0xF9
    effects_FA, // 0xFA
    effects_FB, // 0xFB
    effects_FC, // 0xFC
    effects_FD, // 0xFD
    effects_FE, // 0xFE
    effects_end, // 0xFF
};

f32 g_pitch_table[] =
{
    0.03125f,
    0.03310822f,
    0.035076939f,
    0.037162717f,
    0.03937253f,
    0.041713718f,
    0.044194154f,
    0.046822064f,
    0.049606249f,
    0.052555967f,
    0.055681124f,
    0.058992095f,
    0.0625f,
    0.066216439f,
    0.070153877f,
    0.074325435f,
    0.07874506f,
    0.083427437f,
    0.088388309f,
    0.093644127f,
    0.099212497f,
    0.10511193f,
    0.11136225f,
    0.11798419f,
    0.125f,
    0.13243288f,
    0.14030775f,
    0.14865087f,
    0.15749012f,
    0.16685487f,
    0.17677662f,
    0.18728825f,
    0.19842499f,
    0.21022387f,
    0.2227245f,
    0.23596838f,
    0.25f,
    0.26486576f,
    0.2806155f,
    0.29730174f,
    0.31498024f,
    0.33370975f,
    0.35355324f,
    0.3745765f,
    0.39684999f,
    0.42044774f,
    0.44544899f,
    0.47193676f,
    0.5f,
    0.5297315f,
    0.561231f,
    0.59460348f,
    0.62996048f,
    0.66741949f,
    0.70710647f,
    0.749153f,
    0.79369998f,
    0.84089547f,
    0.89089799f,
    0.9438735f,
    1.0f,
    1.059463f,
    1.122462f,
    1.189207f,
    1.259921f,
    1.334839f,
    1.4142129f,
    1.498306f,
    1.5874f,
    1.6817909f,
    1.781796f,
    1.887747f,
    2.0f,
    0.0f,
};

#if 1
const char* s_sample_file_paths_mnsg_bank_0[] = {
    "assets/mnsg/bank0/B00I00S00.wav", "assets/mnsg/bank0/B00I01S00.wav", "assets/mnsg/bank0/B00I02S00.wav", "assets/mnsg/bank0/B00I03S00.wav",
    "assets/mnsg/bank0/B00I04S00.wav", "assets/mnsg/bank0/B00I05S00.wav", "assets/mnsg/bank0/B00I06S00.wav", "assets/mnsg/bank0/B00I07S00.wav",
    "assets/mnsg/bank0/B00I08S00.wav", "assets/mnsg/bank0/B00I09S00.wav", "assets/mnsg/bank0/B00I0AS00.wav", "assets/mnsg/bank0/B00I0BS00.wav",
    "assets/mnsg/bank0/B00I0CS00.wav", "assets/mnsg/bank0/B00I0DS00.wav", "assets/mnsg/bank0/B00I0ES00.wav", "assets/mnsg/bank0/B00I0FS00.wav",
    "assets/mnsg/bank0/B00I10S00.wav", "assets/mnsg/bank0/B00I11S00.wav", "assets/mnsg/bank0/B00I12S00.wav", "assets/mnsg/bank0/B00I13S00.wav",
    "assets/mnsg/bank0/B00I14S00.wav", "assets/mnsg/bank0/B00I15S00.wav", "assets/mnsg/bank0/B00I16S00.wav", "assets/mnsg/bank0/B00I17S00.wav",
    "assets/mnsg/bank0/B00I18S00.wav", "assets/mnsg/bank0/B00I19S00.wav", "assets/mnsg/bank0/B00I1AS00.wav", "assets/mnsg/bank0/B00I1BS00.wav",
    "assets/mnsg/bank0/B00I1CS00.wav", "assets/mnsg/bank0/B00I1DS00.wav", "assets/mnsg/bank0/B00I1ES00.wav", "assets/mnsg/bank0/B00I1FS00.wav",
    "assets/mnsg/bank0/B00I20S00.wav", "assets/mnsg/bank0/B00I21S00.wav", "assets/mnsg/bank0/B00I22S00.wav", "assets/mnsg/bank0/B00I23S00.wav",
    "assets/mnsg/bank0/B00I24S00.wav", "assets/mnsg/bank0/B00I25S00.wav", "assets/mnsg/bank0/B00I26S00.wav", "assets/mnsg/bank0/B00I27S00.wav",
    "assets/mnsg/bank0/B00I28S00.wav", "assets/mnsg/bank0/B00I29S00.wav", "assets/mnsg/bank0/B00I2AS00.wav", "assets/mnsg/bank0/B00I2BS00.wav",
    "assets/mnsg/bank0/B00I2CS00.wav", "assets/mnsg/bank0/B00I2DS00.wav", "assets/mnsg/bank0/B00I2ES00.wav", "assets/mnsg/bank0/B00I2FS00.wav",
    "assets/mnsg/bank0/B00I30S00.wav", "assets/mnsg/bank0/B00I31S00.wav", "assets/mnsg/bank0/B00I32S00.wav", "assets/mnsg/bank0/B00I33S00.wav",
    "assets/mnsg/bank0/B00I34S00.wav", "assets/mnsg/bank0/B00I35S00.wav", "assets/mnsg/bank0/B00I36S00.wav", "assets/mnsg/bank0/B00I37S00.wav",
    "assets/mnsg/bank0/B00I38S00.wav", "assets/mnsg/bank0/B00I39S00.wav", "assets/mnsg/bank0/B00I3AS00.wav", "assets/mnsg/bank0/B00I3BS00.wav",
    "assets/mnsg/bank0/B00I3CS00.wav", "assets/mnsg/bank0/B00I3DS00.wav", "assets/mnsg/bank0/B00I3ES00.wav", "assets/mnsg/bank0/B00I3FS00.wav",
    "assets/mnsg/bank0/B00I40S00.wav", "assets/mnsg/bank0/B00I41S00.wav", "assets/mnsg/bank0/B00I42S00.wav", "assets/mnsg/bank0/B00I43S00.wav",
    "assets/mnsg/bank0/B00I44S00.wav", "assets/mnsg/bank0/B00I45S00.wav", "assets/mnsg/bank0/B00I46S00.wav", "assets/mnsg/bank0/B00I47S00.wav",
    "assets/mnsg/bank0/B00I48S00.wav", "assets/mnsg/bank0/B00I49S00.wav", "assets/mnsg/bank0/B00I4AS00.wav", "assets/mnsg/bank0/B00I4BS00.wav",
    "assets/mnsg/bank0/B00I4CS00.wav", "assets/mnsg/bank0/B00I4DS00.wav", "assets/mnsg/bank0/B00I4ES00.wav", "assets/mnsg/bank0/B00I4FS00.wav",
    "assets/mnsg/bank0/B00I50S00.wav", "assets/mnsg/bank0/B00I51S00.wav", "assets/mnsg/bank0/B00I52S00.wav", "assets/mnsg/bank0/B00I53S00.wav",
    "assets/mnsg/bank0/B00I54S00.wav", "assets/mnsg/bank0/B00I55S00.wav", "assets/mnsg/bank0/B00I56S00.wav", "assets/mnsg/bank0/B00I57S00.wav",
    "assets/mnsg/bank0/B00I58S00.wav", "assets/mnsg/bank0/B00I59S00.wav", "assets/mnsg/bank0/B00I5AS00.wav", "assets/mnsg/bank0/B00I5BS00.wav",
    "assets/mnsg/bank0/B00I5CS00.wav", "assets/mnsg/bank0/B00I5DS00.wav", "assets/mnsg/bank0/B00I5ES00.wav", "assets/mnsg/bank0/B00I5FS00.wav",
    "assets/mnsg/bank0/B00I60S00.wav", "assets/mnsg/bank0/B00I61S00.wav", "assets/mnsg/bank0/B00I62S00.wav", "assets/mnsg/bank0/B00I63S00.wav",
    "assets/mnsg/bank0/B00I64S00.wav", "assets/mnsg/bank0/B00I65S00.wav", "assets/mnsg/bank0/B00I66S00.wav", "assets/mnsg/bank0/B00I67S00.wav",
    "assets/mnsg/bank0/B00I68S00.wav", "assets/mnsg/bank0/B00I69S00.wav", "assets/mnsg/bank0/B00I6AS00.wav", "assets/mnsg/bank0/B00I6BS00.wav",
    "assets/mnsg/bank0/B00I6CS00.wav", "assets/mnsg/bank0/B00I6DS00.wav", "assets/mnsg/bank0/B00I6ES00.wav", "assets/mnsg/bank0/B00I6FS00.wav",
    "assets/mnsg/bank0/B00I70S00.wav", "assets/mnsg/bank0/B00I71S00.wav", "assets/mnsg/bank0/B00I72S00.wav", "assets/mnsg/bank0/B00I73S00.wav",
    "assets/mnsg/bank0/B00I74S00.wav", "assets/mnsg/bank0/B00I75S00.wav", "assets/mnsg/bank0/B00I76S00.wav", "assets/mnsg/bank0/B00I77S00.wav",
    "assets/mnsg/bank0/B00I78S00.wav", "assets/mnsg/bank0/B00I79S00.wav", "assets/mnsg/bank0/B00I7AS00.wav", "assets/mnsg/bank0/B00I7BS00.wav",
    "assets/mnsg/bank0/B00I7CS00.wav", "assets/mnsg/bank0/B00I7DS00.wav", "assets/mnsg/bank0/B00I7ES00.wav", "assets/mnsg/bank0/B00I7FS00.wav",
    "assets/mnsg/bank0/B00I80S00.wav", "assets/mnsg/bank0/B00I81S00.wav", "assets/mnsg/bank0/B00I82S00.wav", "assets/mnsg/bank0/B00I83S00.wav",
    "assets/mnsg/bank0/B00I84S00.wav", "assets/mnsg/bank0/B00I85S00.wav"
};
#endif

#if 0
/* With custom sounds (higher quality samples) */
const char* s_sample_file_paths_mnsg_bank_0[] = {
    "assets/mnsg/bank0/B00I00S00.wav", "assets/mnsg/bank0/B00I01S00.wav", "assets/mnsg/bank0/B00I02S00.wav", "assets/mnsg/bank0/B00I03S00.wav",
    "assets/mnsg/bank0/B00I04S00.wav", "assets/mnsg/bank0/B00I05S00.wav", "assets/mnsg/bank0/B00I06S00.wav", "assets/mnsg/bank0/B00I07S00.wav",
    "assets/mnsg/bank0/B00I08S00.wav", "assets/mnsg/bank0/B00I09S00.wav", "assets/mnsg/bank0/B00I0AS00.wav", "assets/mnsg/bank0/B00I0BS00.wav",
    "assets/mnsg/bank0/B00I0CS00.wav", "assets/mnsg/bank0/B00I0DS00.wav", "assets/mnsg/bank0/B00I0ES00.wav", "assets/mnsg/bank0/B00I0FS00.wav",
    "assets/mnsg/bank0/B00I10S00.wav", "assets/mnsg/bank0/B00I11S00.wav", "assets/mnsg/bank0/B00I12S00.wav", "assets/mnsg/bank0/B00I13S00.wav",
    "assets/mnsg/bank0/B00I14S00.wav", "assets/mnsg/bank0/B00I15S00.wav", "assets/mnsg/bank0/B00I16S00.wav", "assets/mnsg/bank0/B00I17S00.wav",
    "assets/mnsg/bank0/B00I18S00.wav", "assets/mnsg/bank0/B00I19S00.wav", "assets/mnsg/bank0/B00I1AS00.wav", "assets/mnsg/bank0/B00I1BS00.wav",
    "assets/mnsg/bank0/B00I1CS00.wav", "assets/mnsg/bank0/B00I1DS00.wav", "assets/mnsg/bank0/B00I1ES00.wav", "assets/mnsg/bank0/B00I1FS00.wav",
    "assets/mnsg/bank0/B00I20S00.wav", "assets/mnsg/bank0/B00I21S00.wav", "assets/mnsg/bank0/B00I22S00.wav", "assets/mnsg/bank0/B00I23S00.wav",
    "assets/mnsg/bank0/B00I24S00.wav", "assets/mnsg/bank0/B00I25S00.wav", "assets/mnsg/bank0/B00I26S00.wav", "assets/mnsg/bank0/B00I27S00.wav",
    "assets/mnsg/bank0/B00I28S00.wav", "assets/mnsg/bank0/B00I29S00.wav", "assets/mnsg/bank0/B00I2AS00.wav", "assets/mnsg/bank0/B00I2BS00.wav",
    "assets/mnsg/bank0/B00I2CS00.wav", "assets/mnsg/bank0/B00I2DS00.wav", "assets/mnsg/bank0/B00I2ES00.wav", "assets/mnsg/bank0/B00I2FS00.wav",
    "assets/mnsg/bank0/B00I30S00.wav", "assets/mnsg/bank0/B00I31S00.wav", "assets/mnsg/bank0/B00I32S00.wav", "assets/mnsg/bank0/B00I33S00.wav",
    "assets/mnsg/bank0/B00I34S00.wav", "assets/mnsg/bank0/B00I35S00.wav", "assets/mnsg/bank0/B00I36S00.wav", "assets/mnsg/bank0/B00I37S00.wav",
    "assets/mnsg/bank0/B00I38S00.wav", "assets/mnsg/bank0/B00I39S00.wav", "assets/mnsg/bank0/B00I3AS00.wav", "assets/mnsg/bank0/B00I3BS00.wav",
    "assets/mnsg/bank0/B00I3CS00.wav", "assets/mnsg/bank0/B00I3DS00.wav", "assets/mnsg/bank0/B00I3ES00.wav", "assets/mnsg/bank0/B00I3FS00.wav",
    "assets/mnsg/bank0/B00I40S00.wav", "assets/mnsg/bank0/B00I41S00.wav", "assets/mnsg/bank0/B00I42S00.wav", "assets/mnsg/bank0/B00I43S00.wav",
    "assets/mnsg/bank0/B00I44S00.wav", "assets/mnsg/custom/B00I23S00.wav", "assets/mnsg/custom/B00I24S00.wav", "assets/mnsg/custom/B00I25S00.wav",
    "assets/mnsg/custom/B00I25S00.wav", "assets/mnsg/bank0/B00I49S00.wav", "assets/mnsg/bank0/B00I4AS00.wav", "assets/mnsg/bank0/B00I4BS00.wav",
    "assets/mnsg/bank0/B00I4CS00.wav", "assets/mnsg/custom/B00I3BS00.wav", "assets/mnsg/bank0/B00I4ES00.wav", "assets/mnsg/bank0/B00I4FS00.wav",
    "assets/mnsg/bank0/B00I50S00.wav", "assets/mnsg/bank0/B00I51S00.wav", "assets/mnsg/bank0/B00I52S00.wav", "assets/mnsg/bank0/B00I53S00.wav",
    "assets/mnsg/bank0/B00I54S00.wav", "assets/mnsg/bank0/B00I55S00.wav", "assets/mnsg/bank0/B00I56S00.wav", "assets/mnsg/bank0/B00I57S00.wav",
    "assets/mnsg/bank0/B00I58S00.wav", "assets/mnsg/bank0/B00I59S00.wav", "assets/mnsg/bank0/B00I5AS00.wav", "assets/mnsg/bank0/B00I5BS00.wav",
    "assets/mnsg/bank0/B00I5CS00.wav", "assets/mnsg/bank0/B00I5DS00.wav", "assets/mnsg/bank0/B00I5ES00.wav", "assets/mnsg/bank0/B00I5FS00.wav",
    "assets/mnsg/bank0/B00I60S00.wav", "assets/mnsg/bank0/B00I61S00.wav", "assets/mnsg/bank0/B00I62S00.wav", "assets/mnsg/bank0/B00I63S00.wav",
    "assets/mnsg/bank0/B00I64S00.wav", "assets/mnsg/bank0/B00I65S00.wav", "assets/mnsg/bank0/B00I66S00.wav", "assets/mnsg/bank0/B00I67S00.wav",
    "assets/mnsg/bank0/B00I68S00.wav", "assets/mnsg/bank0/B00I69S00.wav", "assets/mnsg/bank0/B00I6AS00.wav", "assets/mnsg/bank0/B00I6BS00.wav",
    "assets/mnsg/bank0/B00I6CS00.wav", "assets/mnsg/bank0/B00I6DS00.wav", "assets/mnsg/bank0/B00I6ES00.wav", "assets/mnsg/bank0/B00I6FS00.wav",
    "assets/mnsg/bank0/B00I70S00.wav", "assets/mnsg/bank0/B00I71S00.wav", "assets/mnsg/bank0/B00I72S00.wav", "assets/mnsg/bank0/B00I73S00.wav",
    "assets/mnsg/bank0/B00I74S00.wav", "assets/mnsg/bank0/B00I75S00.wav", "assets/mnsg/bank0/B00I76S00.wav", "assets/mnsg/bank0/B00I77S00.wav",
    "assets/mnsg/bank0/B00I78S00.wav", "assets/mnsg/bank0/B00I79S00.wav", "assets/mnsg/bank0/B00I7AS00.wav", "assets/mnsg/bank0/B00I7BS00.wav",
    "assets/mnsg/bank0/B00I7CS00.wav", "assets/mnsg/bank0/B00I7DS00.wav", "assets/mnsg/bank0/B00I7ES00.wav", "assets/mnsg/bank0/B00I7FS00.wav",
    "assets/mnsg/bank0/B00I80S00.wav", "assets/mnsg/bank0/B00I81S00.wav", "assets/mnsg/bank0/B00I82S00.wav", "assets/mnsg/bank0/B00I83S00.wav",
    "assets/mnsg/bank0/B00I84S00.wav", "assets/mnsg/bank0/B00I85S00.wav"
};
#endif

#if 0
/* With custom sounds (higher quality samples) (custom_2) */
const char* s_sample_file_paths_mnsg_bank_0[] = {
    "assets/mnsg/bank0/B00I00S00.wav", "assets/mnsg/bank0/B00I01S00.wav", "assets/mnsg/bank0/B00I02S00.wav", "assets/mnsg/bank0/B00I03S00.wav",
    "assets/mnsg/bank0/B00I04S00.wav", "assets/mnsg/bank0/B00I05S00.wav", "assets/mnsg/bank0/B00I06S00.wav", "assets/mnsg/bank0/B00I07S00.wav",
    "assets/mnsg/bank0/B00I08S00.wav", "assets/mnsg/bank0/B00I09S00.wav", "assets/mnsg/bank0/B00I0AS00.wav", "assets/mnsg/bank0/B00I0BS00.wav",
    "assets/mnsg/bank0/B00I0CS00.wav", "assets/mnsg/bank0/B00I0DS00.wav", "assets/mnsg/bank0/B00I0ES00.wav", "assets/mnsg/bank0/B00I0FS00.wav",
    "assets/mnsg/bank0/B00I10S00.wav", "assets/mnsg/bank0/B00I11S00.wav", "assets/mnsg/bank0/B00I12S00.wav", "assets/mnsg/bank0/B00I13S00.wav",
    "assets/mnsg/bank0/B00I14S00.wav", "assets/mnsg/bank0/B00I15S00.wav", "assets/mnsg/bank0/B00I16S00.wav", "assets/mnsg/bank0/B00I17S00.wav",
    "assets/mnsg/bank0/B00I18S00.wav", "assets/mnsg/bank0/B00I19S00.wav", "assets/mnsg/bank0/B00I1AS00.wav", "assets/mnsg/bank0/B00I1BS00.wav",
    "assets/mnsg/bank0/B00I1CS00.wav", "assets/mnsg/bank0/B00I1DS00.wav", "assets/mnsg/bank0/B00I1ES00.wav", "assets/mnsg/bank0/B00I1FS00.wav",
    "assets/mnsg/bank0/B00I20S00.wav", "assets/mnsg/bank0/B00I21S00.wav", "assets/mnsg/bank0/B00I22S00.wav", "assets/mnsg/bank0/B00I23S00.wav",
    "assets/mnsg/bank0/B00I24S00.wav", "assets/mnsg/bank0/B00I25S00.wav", "assets/mnsg/bank0/B00I26S00.wav", "assets/mnsg/bank0/B00I27S00.wav",
    "assets/mnsg/bank0/B00I28S00.wav", "assets/mnsg/bank0/B00I29S00.wav", "assets/mnsg/bank0/B00I2AS00.wav", "assets/mnsg/bank0/B00I2BS00.wav",
    "assets/mnsg/bank0/B00I2CS00.wav", "assets/mnsg/bank0/B00I2DS00.wav", "assets/mnsg/bank0/B00I2ES00.wav", "assets/mnsg/bank0/B00I2FS00.wav",
    "assets/mnsg/bank0/B00I30S00.wav", "assets/mnsg/bank0/B00I31S00.wav", "assets/mnsg/bank0/B00I32S00.wav", "assets/mnsg/bank0/B00I33S00.wav",
    "assets/mnsg/bank0/B00I34S00.wav", "assets/mnsg/bank0/B00I35S00.wav", "assets/mnsg/bank0/B00I36S00.wav", "assets/mnsg/bank0/B00I37S00.wav",
    "assets/mnsg/bank0/B00I38S00.wav", "assets/mnsg/bank0/B00I39S00.wav", "assets/mnsg/bank0/B00I3AS00.wav", "assets/mnsg/bank0/B00I3BS00.wav",
    "assets/mnsg/bank0/B00I3CS00.wav", "assets/mnsg/bank0/B00I3DS00.wav", "assets/mnsg/bank0/B00I3ES00.wav", "assets/mnsg/bank0/B00I3FS00.wav",
    "assets/mnsg/bank0/B00I40S00.wav", "assets/mnsg/bank0/B00I41S00.wav", "assets/mnsg/bank0/B00I42S00.wav", "assets/mnsg/bank0/B00I43S00.wav",
    "assets/mnsg/bank0/B00I44S00.wav", "assets/mnsg/bank0/B00I45S00.wav", "assets/mnsg/bank0/B00I46S00.wav", "assets/mnsg/bank0/B00I47S00.wav",
    "assets/mnsg/bank0/B00I48S00.wav", "assets/mnsg/bank0/B00I49S00.wav", "assets/mnsg/bank0/B00I4AS00.wav", "assets/mnsg/bank0/B00I4BS00.wav",
    "assets/mnsg/bank0/B00I4CS00.wav", "assets/mnsg/custom/B00I3BS00.wav", "assets/mnsg/bank0/B00I4ES00.wav", "assets/mnsg/bank0/B00I4FS00.wav",
    "assets/mnsg/bank0/B00I50S00.wav", "assets/mnsg/bank0/B00I51S00.wav", "assets/mnsg/bank0/B00I52S00.wav", "assets/mnsg/bank0/B00I53S00.wav",
    "assets/mnsg/bank0/B00I54S00.wav", "assets/mnsg/bank0/B00I55S00.wav", "assets/mnsg/bank0/B00I56S00.wav", "assets/mnsg/bank0/B00I57S00.wav",
    "assets/mnsg/bank0/B00I58S00.wav", "assets/mnsg/bank0/B00I59S00.wav", "assets/mnsg/bank0/B00I5AS00.wav", "assets/mnsg/bank0/B00I5BS00.wav",
    "assets/mnsg/bank0/B00I5CS00.wav", "assets/mnsg/bank0/B00I5DS00.wav", "assets/mnsg/bank0/B00I5ES00.wav", "assets/mnsg/bank0/B00I5FS00.wav",
    "assets/mnsg/bank0/B00I60S00.wav", "assets/mnsg/bank0/B00I61S00.wav", "assets/mnsg/bank0/B00I62S00.wav", "assets/mnsg/bank0/B00I63S00.wav",
    "assets/mnsg/bank0/B00I64S00.wav", "assets/mnsg/bank0/B00I65S00.wav", "assets/mnsg/bank0/B00I66S00.wav", "assets/mnsg/bank0/B00I67S00.wav",
    "assets/mnsg/bank0/B00I68S00.wav", "assets/mnsg/bank0/B00I69S00.wav", "assets/mnsg/bank0/B00I6AS00.wav", "assets/mnsg/bank0/B00I6BS00.wav",
    "assets/mnsg/bank0/B00I6CS00.wav", "assets/mnsg/bank0/B00I6DS00.wav", "assets/mnsg/bank0/B00I6ES00.wav", "assets/mnsg/bank0/B00I6FS00.wav",
    "assets/mnsg/bank0/B00I70S00.wav", "assets/mnsg/bank0/B00I71S00.wav", "assets/mnsg/bank0/B00I72S00.wav", "assets/mnsg/bank0/B00I73S00.wav",
    "assets/mnsg/bank0/B00I74S00.wav", "assets/mnsg/bank0/B00I75S00.wav", "assets/mnsg/bank0/B00I76S00.wav", "assets/mnsg/bank0/B00I77S00.wav",
    "assets/mnsg/bank0/B00I78S00.wav", "assets/mnsg/bank0/B00I79S00.wav", "assets/mnsg/bank0/B00I7AS00.wav", "assets/mnsg/bank0/B00I7BS00.wav",
    "assets/mnsg/bank0/B00I7CS00.wav", "assets/mnsg/bank0/B00I7DS00.wav", "assets/mnsg/bank0/B00I7ES00.wav", "assets/mnsg/bank0/B00I7FS00.wav",
    "assets/mnsg/bank0/B00I80S00.wav", "assets/mnsg/bank0/B00I81S00.wav", "assets/mnsg/bank0/B00I82S00.wav", "assets/mnsg/bank0/B00I83S00.wav",
    "assets/mnsg/bank0/B00I84S00.wav", "assets/mnsg/bank0/B00I85S00.wav"
};
#endif

const char* s_sample_file_paths_mnsg_bank_1[] = {
    "assets/mnsg/bank1/B01I00S00.wav", "assets/mnsg/bank1/B01I01S00.wav", "assets/mnsg/bank1/B01I02S00.wav", "assets/mnsg/bank1/B01I03S00.wav",
    "assets/mnsg/bank1/B01I04S00.wav", "assets/mnsg/bank1/B01I05S00.wav", "assets/mnsg/bank1/B01I06S00.wav", "assets/mnsg/bank1/B01I07S00.wav",
    "assets/mnsg/bank1/B01I08S00.wav", "assets/mnsg/bank1/B01I09S00.wav", "assets/mnsg/bank1/B01I0AS00.wav", "assets/mnsg/bank1/B01I0BS00.wav",
    "assets/mnsg/bank1/B01I0CS00.wav", "assets/mnsg/bank1/B01I0DS00.wav", "assets/mnsg/bank1/B01I0ES00.wav", "assets/mnsg/bank1/B01I0FS00.wav",
    "assets/mnsg/bank1/B01I10S00.wav", "assets/mnsg/bank1/B01I11S00.wav", "assets/mnsg/bank1/B01I12S00.wav", "assets/mnsg/bank1/B01I13S00.wav",
    "assets/mnsg/bank1/B01I14S00.wav", "assets/mnsg/bank1/B01I15S00.wav", "assets/mnsg/bank1/B01I16S00.wav", "assets/mnsg/bank1/B01I17S00.wav",
    "assets/mnsg/bank1/B01I18S00.wav", "assets/mnsg/bank1/B01I19S00.wav", "assets/mnsg/bank1/B01I1AS00.wav", "assets/mnsg/bank1/B01I1BS00.wav",
    "assets/mnsg/bank1/B01I1CS00.wav", "assets/mnsg/bank1/B01I1DS00.wav", "assets/mnsg/bank1/B01I1ES00.wav", "assets/mnsg/bank1/B01I1FS00.wav",
    "assets/mnsg/bank1/B01I20S00.wav", "assets/mnsg/bank1/B01I21S00.wav", "assets/mnsg/bank1/B01I22S00.wav", "assets/mnsg/bank1/B01I23S00.wav",
    "assets/mnsg/bank1/B01I24S00.wav", "assets/mnsg/bank1/B01I25S00.wav", "assets/mnsg/bank1/B01I26S00.wav", "assets/mnsg/bank1/B01I27S00.wav",
    "assets/mnsg/bank1/B01I28S00.wav", "assets/mnsg/bank1/B01I29S00.wav", "assets/mnsg/bank1/B01I2AS00.wav", "assets/mnsg/bank1/B01I2BS00.wav",
    "assets/mnsg/bank1/B01I2CS00.wav", "assets/mnsg/bank1/B01I2DS00.wav", "assets/mnsg/bank1/B01I2ES00.wav", "assets/mnsg/bank1/B01I2FS00.wav",
    "assets/mnsg/bank1/B01I30S00.wav", "assets/mnsg/bank1/B01I31S00.wav", "assets/mnsg/bank1/B01I32S00.wav", "assets/mnsg/bank1/B01I33S00.wav",
    "assets/mnsg/bank1/B01I34S00.wav", "assets/mnsg/bank1/B01I35S00.wav", "assets/mnsg/bank1/B01I36S00.wav", "assets/mnsg/bank1/B01I37S00.wav",
    "assets/mnsg/bank1/B01I38S00.wav", "assets/mnsg/bank1/B01I39S00.wav", "assets/mnsg/bank1/B01I3AS00.wav", "assets/mnsg/bank1/B01I3BS00.wav",
    "assets/mnsg/bank1/B01I3CS00.wav", "assets/mnsg/bank1/B01I3DS00.wav", "assets/mnsg/bank1/B01I3ES00.wav", "assets/mnsg/bank1/B01I3FS00.wav",
    "assets/mnsg/bank1/B01I40S00.wav", "assets/mnsg/bank1/B01I41S00.wav", "assets/mnsg/bank1/B01I42S00.wav", "assets/mnsg/bank1/B01I43S00.wav",
    "assets/mnsg/bank1/B01I44S00.wav", "assets/mnsg/bank1/B01I45S00.wav", "assets/mnsg/bank1/B01I46S00.wav", "assets/mnsg/bank1/B01I47S00.wav",
    "assets/mnsg/bank1/B01I48S00.wav", "assets/mnsg/bank1/B01I49S00.wav", "assets/mnsg/bank1/B01I4AS00.wav", "assets/mnsg/bank1/B01I4BS00.wav",
    "assets/mnsg/bank1/B01I4CS00.wav", "assets/mnsg/bank1/B01I4DS00.wav", "assets/mnsg/bank1/B01I4ES00.wav",
};

const char* s_sample_file_paths_mnsg_bank_2[] = {
    "assets/mnsg/bank2/B02I00S00.wav", "assets/mnsg/bank2/B02I01S00.wav", "assets/mnsg/bank2/B02I02S00.wav", "assets/mnsg/bank2/B02I03S00.wav",
    "assets/mnsg/bank2/B02I04S00.wav", "assets/mnsg/bank2/B02I05S00.wav", "assets/mnsg/bank2/B02I06S00.wav", "assets/mnsg/bank2/B02I07S00.wav",
    "assets/mnsg/bank2/B02I08S00.wav", "assets/mnsg/bank2/B02I09S00.wav", "assets/mnsg/bank2/B02I0AS00.wav", "assets/mnsg/bank2/B02I0BS00.wav",
    "assets/mnsg/bank2/B02I0CS00.wav", "assets/mnsg/bank2/B02I0DS00.wav", "assets/mnsg/bank2/B02I0ES00.wav", "assets/mnsg/bank2/B02I0FS00.wav",
    "assets/mnsg/bank2/B02I10S00.wav", "assets/mnsg/bank2/B02I11S00.wav", "assets/mnsg/bank2/B02I12S00.wav", "assets/mnsg/bank2/B02I13S00.wav",
    "assets/mnsg/bank2/B02I14S00.wav", "assets/mnsg/bank2/B02I15S00.wav", "assets/mnsg/bank2/B02I16S00.wav", "assets/mnsg/bank2/B02I17S00.wav",
    "assets/mnsg/bank2/B02I18S00.wav", "assets/mnsg/bank2/B02I19S00.wav", "assets/mnsg/bank2/B02I1AS00.wav", "assets/mnsg/bank2/B02I1BS00.wav",
    "assets/mnsg/bank2/B02I1CS00.wav", "assets/mnsg/bank2/B02I1DS00.wav", "assets/mnsg/bank2/B02I1ES00.wav", "assets/mnsg/bank2/B02I1FS00.wav",
    "assets/mnsg/bank2/B02I20S00.wav", "assets/mnsg/bank2/B02I21S00.wav", "assets/mnsg/bank2/B02I22S00.wav", "assets/mnsg/bank2/B02I23S00.wav",
    "assets/mnsg/bank2/B02I24S00.wav", "assets/mnsg/bank2/B02I25S00.wav", "assets/mnsg/bank2/B02I26S00.wav", "assets/mnsg/bank2/B02I27S00.wav",
    "assets/mnsg/bank2/B02I28S00.wav", "assets/mnsg/bank2/B02I29S00.wav", "assets/mnsg/bank2/B02I2AS00.wav", "assets/mnsg/bank2/B02I2BS00.wav",
    "assets/mnsg/bank2/B02I2CS00.wav", "assets/mnsg/bank2/B02I2DS00.wav", "assets/mnsg/bank2/B02I2ES00.wav", "assets/mnsg/bank2/B02I2FS00.wav",
    "assets/mnsg/bank2/B02I30S00.wav", "assets/mnsg/bank2/B02I31S00.wav", "assets/mnsg/bank2/B02I32S00.wav", "assets/mnsg/bank2/B02I33S00.wav",
    "assets/mnsg/bank2/B02I34S00.wav", "assets/mnsg/bank2/B02I35S00.wav", "assets/mnsg/bank2/B02I36S00.wav", "assets/mnsg/bank2/B02I37S00.wav",
    "assets/mnsg/bank2/B02I38S00.wav", "assets/mnsg/bank2/B02I39S00.wav", "assets/mnsg/bank2/B02I3AS00.wav", "assets/mnsg/bank2/B02I3BS00.wav",
    "assets/mnsg/bank2/B02I3CS00.wav", "assets/mnsg/bank2/B02I3DS00.wav", "assets/mnsg/bank2/B02I3ES00.wav", "assets/mnsg/bank2/B02I3FS00.wav",
    "assets/mnsg/bank2/B02I40S00.wav", "assets/mnsg/bank2/B02I41S00.wav", "assets/mnsg/bank2/B02I42S00.wav", "assets/mnsg/bank2/B02I43S00.wav",
    "assets/mnsg/bank2/B02I44S00.wav", "assets/mnsg/bank2/B02I45S00.wav", "assets/mnsg/bank2/B02I46S00.wav", "assets/mnsg/bank2/B02I47S00.wav",
    "assets/mnsg/bank2/B02I48S00.wav", "assets/mnsg/bank2/B02I49S00.wav", "assets/mnsg/bank2/B02I4AS00.wav", "assets/mnsg/bank2/B02I4BS00.wav",
    "assets/mnsg/bank2/B02I4CS00.wav", "assets/mnsg/bank2/B02I4DS00.wav", "assets/mnsg/bank2/B02I4ES00.wav", "assets/mnsg/bank2/B02I4FS00.wav",
    "assets/mnsg/bank2/B02I50S00.wav", "assets/mnsg/bank2/B02I51S00.wav", "assets/mnsg/bank2/B02I52S00.wav", "assets/mnsg/bank2/B02I53S00.wav",
    "assets/mnsg/bank2/B02I54S00.wav", "assets/mnsg/bank2/B02I55S00.wav", "assets/mnsg/bank2/B02I56S00.wav", "assets/mnsg/bank2/B02I57S00.wav",
    "assets/mnsg/bank2/B02I58S00.wav", "assets/mnsg/bank2/B02I59S00.wav", "assets/mnsg/bank2/B02I5AS00.wav", "assets/mnsg/bank2/B02I5BS00.wav",
    "assets/mnsg/bank2/B02I5CS00.wav", "assets/mnsg/bank2/B02I5DS00.wav", "assets/mnsg/bank2/B02I5ES00.wav", "assets/mnsg/bank2/B02I5FS00.wav",
    "assets/mnsg/bank2/B02I60S00.wav", "assets/mnsg/bank2/B02I61S00.wav", "assets/mnsg/bank2/B02I62S00.wav", "assets/mnsg/bank2/B02I63S00.wav",
    "assets/mnsg/bank2/B02I64S00.wav", "assets/mnsg/bank2/B02I65S00.wav", "assets/mnsg/bank2/B02I66S00.wav", "assets/mnsg/bank2/B02I67S00.wav",
    "assets/mnsg/bank2/B02I68S00.wav", "assets/mnsg/bank2/B02I69S00.wav", "assets/mnsg/bank2/B02I6AS00.wav", "assets/mnsg/bank2/B02I6BS00.wav",
    "assets/mnsg/bank2/B02I6CS00.wav", "assets/mnsg/bank2/B02I6DS00.wav", "assets/mnsg/bank2/B02I6ES00.wav", "assets/mnsg/bank2/B02I6FS00.wav",
    "assets/mnsg/bank2/B02I70S00.wav", "assets/mnsg/bank2/B02I71S00.wav", "assets/mnsg/bank2/B02I72S00.wav", "assets/mnsg/bank2/B02I73S00.wav",
    "assets/mnsg/bank2/B02I74S00.wav", "assets/mnsg/bank2/B02I75S00.wav", "assets/mnsg/bank2/B02I76S00.wav", "assets/mnsg/bank2/B02I77S00.wav",
    "assets/mnsg/bank2/B02I78S00.wav", "assets/mnsg/bank2/B02I79S00.wav", "assets/mnsg/bank2/B02I7AS00.wav", "assets/mnsg/bank2/B02I7BS00.wav",
    "assets/mnsg/bank2/B02I7CS00.wav", "assets/mnsg/bank2/B02I7DS00.wav", "assets/mnsg/bank2/B02I7ES00.wav", "assets/mnsg/bank2/B02I7FS00.wav",
    "assets/mnsg/bank2/B02I80S00.wav", "assets/mnsg/bank2/B02I81S00.wav", "assets/mnsg/bank2/B02I82S00.wav", "assets/mnsg/bank2/B02I83S00.wav",
    "assets/mnsg/bank2/B02I84S00.wav", "assets/mnsg/bank2/B02I85S00.wav", "assets/mnsg/bank2/B02I86S00.wav", "assets/mnsg/bank2/B02I87S00.wav",
    "assets/mnsg/bank2/B02I88S00.wav", "assets/mnsg/bank2/B02I89S00.wav", "assets/mnsg/bank2/B02I8AS00.wav", "assets/mnsg/bank2/B02I8BS00.wav",
    "assets/mnsg/bank2/B02I8CS00.wav", "assets/mnsg/bank2/B02I8DS00.wav", "assets/mnsg/bank2/B02I8ES00.wav", "assets/mnsg/bank2/B02I8FS00.wav",
    "assets/mnsg/bank2/B02I90S00.wav", "assets/mnsg/bank2/B02I91S00.wav", "assets/mnsg/bank2/B02I92S00.wav", "assets/mnsg/bank2/B02I93S00.wav",
    "assets/mnsg/bank2/B02I94S00.wav", "assets/mnsg/bank2/B02I95S00.wav", "assets/mnsg/bank2/B02I96S00.wav", "assets/mnsg/bank2/B02I97S00.wav",
    "assets/mnsg/bank2/B02I98S00.wav", "assets/mnsg/bank2/B02I99S00.wav", "assets/mnsg/bank2/B02I9AS00.wav", "assets/mnsg/bank2/B02I9BS00.wav",
    "assets/mnsg/bank2/B02I9CS00.wav", "assets/mnsg/bank2/B02I9DS00.wav", "assets/mnsg/bank2/B02I9ES00.wav", "assets/mnsg/bank2/B02I9FS00.wav",
    "assets/mnsg/bank2/B02IA0S00.wav", "assets/mnsg/bank2/B02IA1S00.wav", "assets/mnsg/bank2/B02IA2S00.wav", "assets/mnsg/bank2/B02IA3S00.wav",
    "assets/mnsg/bank2/B02IA4S00.wav", "assets/mnsg/bank2/B02IA5S00.wav", "assets/mnsg/bank2/B02IA6S00.wav", "assets/mnsg/bank2/B02IA7S00.wav",
    "assets/mnsg/bank2/B02IA8S00.wav", "assets/mnsg/bank2/B02IA9S00.wav", "assets/mnsg/bank2/B02IAAS00.wav", "assets/mnsg/bank2/B02IABS00.wav",
    "assets/mnsg/bank2/B02IACS00.wav", "assets/mnsg/bank2/B02IADS00.wav", "assets/mnsg/bank2/B02IAES00.wav", "assets/mnsg/bank2/B02IAFS00.wav",
    "assets/mnsg/bank2/B02IB0S00.wav", "assets/mnsg/bank2/B02IB1S00.wav", "assets/mnsg/bank2/B02IB2S00.wav", "assets/mnsg/bank2/B02IB3S00.wav",
    "assets/mnsg/bank2/B02IB4S00.wav", "assets/mnsg/bank2/B02IB5S00.wav", "assets/mnsg/bank2/B02IB6S00.wav", "assets/mnsg/bank2/B02IB7S00.wav",
    "assets/mnsg/bank2/B02IB8S00.wav", "assets/mnsg/bank2/B02IB9S00.wav", "assets/mnsg/bank2/B02IBAS00.wav", "assets/mnsg/bank2/B02IBBS00.wav",
    "assets/mnsg/bank2/B02IBCS00.wav", "assets/mnsg/bank2/B02IBDS00.wav", "assets/mnsg/bank2/B02IBES00.wav", "assets/mnsg/bank2/B02IBFS00.wav",
    "assets/mnsg/bank2/B02IC0S00.wav", "assets/mnsg/bank2/B02IC1S00.wav", "assets/mnsg/bank2/B02IC2S00.wav", "assets/mnsg/bank2/B02IC3S00.wav",
    "assets/mnsg/bank2/B02IC4S00.wav", "assets/mnsg/bank2/B02IC5S00.wav", "assets/mnsg/bank2/B02IC6S00.wav", "assets/mnsg/bank2/B02IC7S00.wav",
    "assets/mnsg/bank2/B02IC8S00.wav", "assets/mnsg/bank2/B02IC9S00.wav", "assets/mnsg/bank2/B02ICAS00.wav", "assets/mnsg/bank2/B02ICBS00.wav",
    "assets/mnsg/bank2/B02ICCS00.wav", "assets/mnsg/bank2/B02ICDS00.wav", "assets/mnsg/bank2/B02ICES00.wav", "assets/mnsg/bank2/B02ICFS00.wav",
    "assets/mnsg/bank2/B02ID0S00.wav", "assets/mnsg/bank2/B02ID1S00.wav", "assets/mnsg/bank2/B02ID2S00.wav", "assets/mnsg/bank2/B02ID3S00.wav",
    "assets/mnsg/bank2/B02ID4S00.wav", "assets/mnsg/bank2/B02ID5S00.wav", "assets/mnsg/bank2/B02ID6S00.wav", "assets/mnsg/bank2/B02ID7S00.wav",
    "assets/mnsg/bank2/B02ID8S00.wav", "assets/mnsg/bank2/B02ID9S00.wav", "assets/mnsg/bank2/B02IDAS00.wav", "assets/mnsg/bank2/B02IDBS00.wav",
    "assets/mnsg/bank2/B02IDCS00.wav", "assets/mnsg/bank2/B02IDDS00.wav", "assets/mnsg/bank2/B02IDES00.wav", "assets/mnsg/bank2/B02IDFS00.wav"
};

const char** s_sample_file_paths_mnsg_banks[] = {
    s_sample_file_paths_mnsg_bank_0,
    s_sample_file_paths_mnsg_bank_1,
    s_sample_file_paths_mnsg_bank_2,
};

const char* s_sample_file_paths_gga_bank_0[] = {
    "assets/gga/B00I00S00.wav", "assets/gga/B00I01S00.wav", "assets/gga/B00I02S00.wav", "assets/gga/B00I03S00.wav",
    "assets/gga/B00I04S00.wav", "assets/gga/B00I05S00.wav", "assets/gga/B00I06S00.wav", "assets/gga/B00I07S00.wav",
    "assets/gga/B00I08S00.wav", "assets/gga/B00I09S00.wav", "assets/gga/B00I0AS00.wav", "assets/gga/B00I0BS00.wav",
    "assets/gga/B00I0CS00.wav", "assets/gga/B00I0DS00.wav", "assets/gga/B00I0ES00.wav", "assets/gga/B00I0FS00.wav",
    "assets/gga/B00I10S00.wav", "assets/gga/B00I11S00.wav", "assets/gga/B00I12S00.wav", "assets/gga/B00I13S00.wav",
    "assets/gga/B00I14S00.wav", "assets/gga/B00I15S00.wav", "assets/gga/B00I16S00.wav", "assets/gga/B00I17S00.wav",
    "assets/gga/B00I18S00.wav", "assets/gga/B00I19S00.wav", "assets/gga/B00I1AS00.wav", "assets/gga/B00I1BS00.wav",
    "assets/gga/B00I1CS00.wav", "assets/gga/B00I1DS00.wav", "assets/gga/B00I1ES00.wav", "assets/gga/B00I1FS00.wav",
    "assets/gga/B00I20S00.wav", "assets/gga/B00I21S00.wav", "assets/gga/B00I22S00.wav", "assets/gga/B00I23S00.wav",
    "assets/gga/B00I24S00.wav", "assets/gga/B00I25S00.wav", "assets/gga/B00I26S00.wav", "assets/gga/B00I27S00.wav",
    "assets/gga/B00I28S00.wav", "assets/gga/B00I29S00.wav", "assets/gga/B00I2AS00.wav", "assets/gga/B00I2BS00.wav",
    "assets/gga/B00I2CS00.wav", "assets/gga/B00I2DS00.wav", "assets/gga/B00I2ES00.wav", "assets/gga/B00I2FS00.wav",
    "assets/gga/B00I30S00.wav", "assets/gga/B00I31S00.wav", "assets/gga/B00I32S00.wav", "assets/gga/B00I33S00.wav",
    "assets/gga/B00I34S00.wav", "assets/gga/B00I35S00.wav", "assets/gga/B00I36S00.wav", "assets/gga/B00I37S00.wav",
    "assets/gga/B00I38S00.wav", "assets/gga/B00I39S00.wav", "assets/gga/B00I3AS00.wav", "assets/gga/B00I3BS00.wav",
    "assets/gga/B00I3CS00.wav", "assets/gga/B00I3DS00.wav", "assets/gga/B00I3ES00.wav", "assets/gga/B00I3FS00.wav",
    "assets/gga/B00I40S00.wav", "assets/gga/B00I41S00.wav", "assets/gga/B00I42S00.wav", "assets/gga/B00I43S00.wav",
    "assets/gga/B00I44S00.wav", "assets/gga/B00I45S00.wav", "assets/gga/B00I46S00.wav", "assets/gga/B00I47S00.wav",
    "assets/gga/B00I48S00.wav", "assets/gga/B00I49S00.wav", "assets/gga/B00I4AS00.wav", "assets/gga/B00I4BS00.wav",
    "assets/gga/B00I4CS00.wav", "assets/gga/B00I4DS00.wav", "assets/gga/B00I4ES00.wav", "assets/gga/B00I4FS00.wav",
    "assets/gga/B00I50S00.wav", "assets/gga/B00I51S00.wav", "assets/gga/B00I52S00.wav", "assets/gga/B00I53S00.wav",
    "assets/gga/B00I54S00.wav", "assets/gga/B00I55S00.wav", "assets/gga/B00I56S00.wav", "assets/gga/B00I57S00.wav",
    "assets/gga/B00I58S00.wav", "assets/gga/B00I59S00.wav", "assets/gga/B00I5AS00.wav", "assets/gga/B00I5BS00.wav",
    "assets/gga/B00I5CS00.wav", "assets/gga/B00I5DS00.wav", "assets/gga/B00I5ES00.wav", "assets/gga/B00I5FS00.wav",
    "assets/gga/B00I60S00.wav", "assets/gga/B00I61S00.wav", "assets/gga/B00I62S00.wav", "assets/gga/B00I63S00.wav",
    "assets/gga/B00I64S00.wav", "assets/gga/B00I65S00.wav", "assets/gga/B00I66S00.wav", "assets/gga/B00I67S00.wav",
    "assets/gga/B00I68S00.wav", "assets/gga/B00I69S00.wav", "assets/gga/B00I6AS00.wav", "assets/gga/B00I6BS00.wav",
    "assets/gga/B00I6CS00.wav", "assets/gga/B00I6DS00.wav", "assets/gga/B00I6ES00.wav", "assets/gga/B00I6FS00.wav",
    "assets/gga/B00I70S00.wav", "assets/gga/B00I71S00.wav", "assets/gga/B00I72S00.wav", "assets/gga/B00I73S00.wav",
    "assets/gga/B00I74S00.wav", "assets/gga/B00I75S00.wav", "assets/gga/B00I76S00.wav", "assets/gga/B00I77S00.wav",
    "assets/gga/B00I78S00.wav", "assets/gga/B00I79S00.wav", "assets/gga/B00I7AS00.wav", "assets/gga/B00I7BS00.wav",
    "assets/gga/B00I7CS00.wav", "assets/gga/B00I7DS00.wav", "assets/gga/B00I7ES00.wav", "assets/gga/B00I7FS00.wav",
    "assets/gga/B00I80S00.wav", "assets/gga/B00I81S00.wav", "assets/gga/B00I82S00.wav", "assets/gga/B00I83S00.wav",
    "assets/gga/B00I84S00.wav", "assets/gga/B00I85S00.wav", "assets/gga/B00I86S00.wav", "assets/gga/B00I87S00.wav",
    "assets/gga/B00I88S00.wav", "assets/gga/B00I89S00.wav", "assets/gga/B00I8AS00.wav", "assets/gga/B00I8BS00.wav",
    "assets/gga/B00I8CS00.wav", "assets/gga/B00I8DS00.wav", "assets/gga/B00I8ES00.wav", "assets/gga/B00I8FS00.wav",
    "assets/gga/B00I90S00.wav", "assets/gga/B00I91S00.wav", "assets/gga/B00I92S00.wav", "assets/gga/B00I93S00.wav",
    "assets/gga/B00I94S00.wav", "assets/gga/B00I95S00.wav", "assets/gga/B00I96S00.wav", "assets/gga/B00I97S00.wav",
    "assets/gga/B00I98S00.wav", "assets/gga/B00I99S00.wav", "assets/gga/B00I9AS00.wav", "assets/gga/B00I9BS00.wav",
    "assets/gga/B00I9CS00.wav", "assets/gga/B00I9DS00.wav"
};

const char* s_sample_file_paths_gga_bank_1[] = {
    "assets/gga/B01I00S00.wav", "assets/gga/B01I01S00.wav", "assets/gga/B01I02S00.wav", "assets/gga/B01I03S00.wav",
    "assets/gga/B01I04S00.wav", "assets/gga/B01I05S00.wav", "assets/gga/B01I06S00.wav", "assets/gga/B01I07S00.wav",
    "assets/gga/B01I08S00.wav", "assets/gga/B01I09S00.wav", "assets/gga/B01I0AS00.wav", "assets/gga/B01I0BS00.wav",
    "assets/gga/B01I0CS00.wav", "assets/gga/B01I0DS00.wav", "assets/gga/B01I0ES00.wav", "assets/gga/B01I0FS00.wav",
    "assets/gga/B01I10S00.wav", "assets/gga/B01I11S00.wav", "assets/gga/B01I12S00.wav", "assets/gga/B01I13S00.wav",
    "assets/gga/B01I14S00.wav", "assets/gga/B01I15S00.wav", "assets/gga/B01I16S00.wav", "assets/gga/B01I17S00.wav",
    "assets/gga/B01I18S00.wav", "assets/gga/B01I19S00.wav", "assets/gga/B01I1AS00.wav", "assets/gga/B01I1BS00.wav",
    "assets/gga/B01I1CS00.wav", "assets/gga/B01I1DS00.wav", "assets/gga/B01I1ES00.wav", "assets/gga/B01I1FS00.wav",
    "assets/gga/B01I20S00.wav", "assets/gga/B01I21S00.wav", "assets/gga/B01I22S00.wav", "assets/gga/B01I23S00.wav",
    "assets/gga/B01I24S00.wav", "assets/gga/B01I25S00.wav", "assets/gga/B01I26S00.wav", "assets/gga/B01I27S00.wav",
    "assets/gga/B01I28S00.wav", "assets/gga/B01I29S00.wav", "assets/gga/B01I2AS00.wav", "assets/gga/B01I2BS00.wav",
    "assets/gga/B01I2CS00.wav", "assets/gga/B01I2DS00.wav", "assets/gga/B01I2ES00.wav", "assets/gga/B01I2FS00.wav",
    "assets/gga/B01I30S00.wav", "assets/gga/B01I31S00.wav", "assets/gga/B01I32S00.wav", "assets/gga/B01I33S00.wav",
    "assets/gga/B01I34S00.wav", "assets/gga/B01I35S00.wav", "assets/gga/B01I36S00.wav", "assets/gga/B01I37S00.wav",
    "assets/gga/B01I38S00.wav", "assets/gga/B01I39S00.wav", "assets/gga/B01I3AS00.wav", "assets/gga/B01I3BS00.wav",
    "assets/gga/B01I3CS00.wav", "assets/gga/B01I3DS00.wav", "assets/gga/B01I3ES00.wav", "assets/gga/B01I3FS00.wav",
    "assets/gga/B01I40S00.wav", "assets/gga/B01I41S00.wav", "assets/gga/B01I42S00.wav", "assets/gga/B01I43S00.wav",
    "assets/gga/B01I44S00.wav", "assets/gga/B01I45S00.wav", "assets/gga/B01I46S00.wav", "assets/gga/B01I47S00.wav",
    "assets/gga/B01I48S00.wav", "assets/gga/B01I49S00.wav", "assets/gga/B01I4AS00.wav", "assets/gga/B01I4BS00.wav",
    "assets/gga/B01I4CS00.wav", "assets/gga/B01I4DS00.wav", "assets/gga/B01I4ES00.wav", "assets/gga/B01I4FS00.wav",
    "assets/gga/B01I50S00.wav", "assets/gga/B01I51S00.wav", "assets/gga/B01I52S00.wav", "assets/gga/B01I53S00.wav",
    "assets/gga/B01I54S00.wav", "assets/gga/B01I55S00.wav", "assets/gga/B01I56S00.wav", "assets/gga/B01I57S00.wav",
    "assets/gga/B01I58S00.wav", "assets/gga/B01I59S00.wav", "assets/gga/B01I5AS00.wav", "assets/gga/B01I5BS00.wav",
    "assets/gga/B01I5CS00.wav", "assets/gga/B01I5DS00.wav", "assets/gga/B01I5ES00.wav", "assets/gga/B01I5FS00.wav",
    "assets/gga/B01I60S00.wav", "assets/gga/B01I61S00.wav", "assets/gga/B01I62S00.wav", "assets/gga/B01I63S00.wav",
    "assets/gga/B01I64S00.wav", "assets/gga/B01I65S00.wav", "assets/gga/B01I66S00.wav", "assets/gga/B01I67S00.wav",
    "assets/gga/B01I68S00.wav", "assets/gga/B01I69S00.wav", "assets/gga/B01I6AS00.wav", "assets/gga/B01I6BS00.wav",
    "assets/gga/B01I6CS00.wav", "assets/gga/B01I6DS00.wav", "assets/gga/B01I6ES00.wav", "assets/gga/B01I6FS00.wav",
    "assets/gga/B01I70S00.wav", "assets/gga/B01I71S00.wav", "assets/gga/B01I72S00.wav", "assets/gga/B01I73S00.wav",
    "assets/gga/B01I74S00.wav", "assets/gga/B01I75S00.wav", "assets/gga/B01I76S00.wav", "assets/gga/B01I77S00.wav",
    "assets/gga/B01I78S00.wav", "assets/gga/B01I79S00.wav", "assets/gga/B01I7AS00.wav", "assets/gga/B01I7BS00.wav",
    "assets/gga/B01I7CS00.wav", "assets/gga/B01I7DS00.wav", "assets/gga/B01I7ES00.wav", "assets/gga/B01I7FS00.wav",
    "assets/gga/B01I80S00.wav", "assets/gga/B01I81S00.wav", "assets/gga/B01I82S00.wav", "assets/gga/B01I83S00.wav",
    "assets/gga/B01I84S00.wav", "assets/gga/B01I85S00.wav", "assets/gga/B01I86S00.wav", "assets/gga/B01I87S00.wav",
    "assets/gga/B01I88S00.wav", "assets/gga/B01I89S00.wav", "assets/gga/B01I8AS00.wav", "assets/gga/B01I8BS00.wav",
    "assets/gga/B01I8CS00.wav"
};

const char* s_sample_file_paths_gga_bank_2[] = {
    "assets/gga/B02I00S00.wav", "assets/gga/B02I01S00.wav", "assets/gga/B02I02S00.wav", "assets/gga/B02I03S00.wav",
    "assets/gga/B02I04S00.wav", "assets/gga/B02I05S00.wav", "assets/gga/B02I06S00.wav", "assets/gga/B02I07S00.wav",
    "assets/gga/B02I08S00.wav", "assets/gga/B02I09S00.wav", "assets/gga/B02I0AS00.wav", "assets/gga/B02I0BS00.wav",
    "assets/gga/B02I0CS00.wav", "assets/gga/B02I0DS00.wav", "assets/gga/B02I0ES00.wav", "assets/gga/B02I0FS00.wav",
    "assets/gga/B02I10S00.wav", "assets/gga/B02I11S00.wav", "assets/gga/B02I12S00.wav", "assets/gga/B02I13S00.wav",
    "assets/gga/B02I14S00.wav", "assets/gga/B02I15S00.wav", "assets/gga/B02I16S00.wav", "assets/gga/B02I17S00.wav",
    "assets/gga/B02I18S00.wav", "assets/gga/B02I19S00.wav", "assets/gga/B02I1AS00.wav", "assets/gga/B02I1BS00.wav",
    "assets/gga/B02I1CS00.wav", "assets/gga/B02I1DS00.wav", "assets/gga/B02I1ES00.wav", "assets/gga/B02I1FS00.wav",
    "assets/gga/B02I20S00.wav", "assets/gga/B02I21S00.wav", "assets/gga/B02I22S00.wav", "assets/gga/B02I23S00.wav",
    "assets/gga/B02I24S00.wav", "assets/gga/B02I25S00.wav", "assets/gga/B02I26S00.wav", "assets/gga/B02I27S00.wav",
    "assets/gga/B02I28S00.wav", "assets/gga/B02I29S00.wav", "assets/gga/B02I2AS00.wav", "assets/gga/B02I2BS00.wav",
    "assets/gga/B02I2CS00.wav", "assets/gga/B02I2DS00.wav", "assets/gga/B02I2ES00.wav", "assets/gga/B02I2FS00.wav",
    "assets/gga/B02I30S00.wav", "assets/gga/B02I31S00.wav", "assets/gga/B02I32S00.wav", "assets/gga/B02I33S00.wav",
    "assets/gga/B02I34S00.wav", "assets/gga/B02I35S00.wav", "assets/gga/B02I36S00.wav", "assets/gga/B02I37S00.wav",
    "assets/gga/B02I38S00.wav", "assets/gga/B02I39S00.wav", "assets/gga/B02I3AS00.wav", "assets/gga/B02I3BS00.wav",
    "assets/gga/B02I3CS00.wav", "assets/gga/B02I3DS00.wav", "assets/gga/B02I3ES00.wav", "assets/gga/B02I3FS00.wav",
    "assets/gga/B02I40S00.wav", "assets/gga/B02I41S00.wav", "assets/gga/B02I42S00.wav", "assets/gga/B02I43S00.wav",
    "assets/gga/B02I44S00.wav", "assets/gga/B02I45S00.wav", "assets/gga/B02I46S00.wav", "assets/gga/B02I47S00.wav",
    "assets/gga/B02I48S00.wav", "assets/gga/B02I49S00.wav", "assets/gga/B02I4AS00.wav", "assets/gga/B02I4BS00.wav",
    "assets/gga/B02I4CS00.wav", "assets/gga/B02I4DS00.wav", "assets/gga/B02I4ES00.wav", "assets/gga/B02I4FS00.wav",
    "assets/gga/B02I50S00.wav", "assets/gga/B02I51S00.wav", "assets/gga/B02I52S00.wav", "assets/gga/B02I53S00.wav",
    "assets/gga/B02I54S00.wav", "assets/gga/B02I55S00.wav", "assets/gga/B02I56S00.wav", "assets/gga/B02I57S00.wav",
    "assets/gga/B02I58S00.wav", "assets/gga/B02I59S00.wav", "assets/gga/B02I5AS00.wav", "assets/gga/B02I5BS00.wav",
    "assets/gga/B02I5CS00.wav", "assets/gga/B02I5DS00.wav", "assets/gga/B02I5ES00.wav", "assets/gga/B02I5FS00.wav",
    "assets/gga/B02I60S00.wav", "assets/gga/B02I61S00.wav", "assets/gga/B02I62S00.wav", "assets/gga/B02I63S00.wav",
    "assets/gga/B02I64S00.wav", "assets/gga/B02I65S00.wav", "assets/gga/B02I66S00.wav", "assets/gga/B02I67S00.wav",
    "assets/gga/B02I68S00.wav", "assets/gga/B02I69S00.wav", "assets/gga/B02I6AS00.wav", "assets/gga/B02I6BS00.wav",
    "assets/gga/B02I6CS00.wav", "assets/gga/B02I6DS00.wav", "assets/gga/B02I6ES00.wav", "assets/gga/B02I6FS00.wav",
    "assets/gga/B02I70S00.wav", "assets/gga/B02I71S00.wav", "assets/gga/B02I72S00.wav", "assets/gga/B02I73S00.wav",
    "assets/gga/B02I74S00.wav", "assets/gga/B02I75S00.wav", "assets/gga/B02I76S00.wav", "assets/gga/B02I77S00.wav",
    "assets/gga/B02I78S00.wav", "assets/gga/B02I79S00.wav", "assets/gga/B02I7AS00.wav", "assets/gga/B02I7BS00.wav",
    "assets/gga/B02I7CS00.wav", "assets/gga/B02I7DS00.wav", "assets/gga/B02I7ES00.wav", "assets/gga/B02I7FS00.wav",
    "assets/gga/B02I80S00.wav", "assets/gga/B02I81S00.wav", "assets/gga/B02I82S00.wav", "assets/gga/B02I83S00.wav",
    "assets/gga/B02I84S00.wav", "assets/gga/B02I85S00.wav", "assets/gga/B02I86S00.wav", "assets/gga/B02I87S00.wav",
    "assets/gga/B02I88S00.wav", "assets/gga/B02I89S00.wav", "assets/gga/B02I8AS00.wav", "assets/gga/B02I8BS00.wav",
    "assets/gga/B02I8CS00.wav", "assets/gga/B02I8DS00.wav", "assets/gga/B02I8ES00.wav", "assets/gga/B02I8FS00.wav",
    "assets/gga/B02I90S00.wav", "assets/gga/B02I91S00.wav", "assets/gga/B02I92S00.wav", "assets/gga/B02I93S00.wav",
    "assets/gga/B02I94S00.wav", "assets/gga/B02I95S00.wav", "assets/gga/B02I96S00.wav", "assets/gga/B02I97S00.wav",
    "assets/gga/B02I98S00.wav", "assets/gga/B02I99S00.wav", "assets/gga/B02I9AS00.wav", "assets/gga/B02I9BS00.wav",
    "assets/gga/B02I9CS00.wav", "assets/gga/B02I9DS00.wav", "assets/gga/B02I9ES00.wav", "assets/gga/B02I9FS00.wav",
    "assets/gga/B02IA0S00.wav", "assets/gga/B02IA1S00.wav", "assets/gga/B02IA2S00.wav", "assets/gga/B02IA3S00.wav",
    "assets/gga/B02IA4S00.wav", "assets/gga/B02IA5S00.wav", "assets/gga/B02IA6S00.wav", "assets/gga/B02IA7S00.wav",
    "assets/gga/B02IA8S00.wav", "assets/gga/B02IA9S00.wav",
};

const char* s_sample_file_paths_gga_bank_3[] = {
    "assets/gga/B03I00S00.wav", "assets/gga/B03I01S00.wav", "assets/gga/B03I02S00.wav", "assets/gga/B03I03S00.wav",
    "assets/gga/B03I04S00.wav", "assets/gga/B03I05S00.wav", "assets/gga/B03I06S00.wav", "assets/gga/B03I07S00.wav",
    "assets/gga/B03I08S00.wav", "assets/gga/B03I09S00.wav", "assets/gga/B03I0AS00.wav", "assets/gga/B03I0BS00.wav",
    "assets/gga/B03I0CS00.wav", "assets/gga/B03I0DS00.wav", "assets/gga/B03I0ES00.wav", "assets/gga/B03I0FS00.wav",
    "assets/gga/B03I10S00.wav", "assets/gga/B03I11S00.wav", "assets/gga/B03I12S00.wav", "assets/gga/B03I13S00.wav",
    "assets/gga/B03I14S00.wav", "assets/gga/B03I15S00.wav", "assets/gga/B03I16S00.wav", "assets/gga/B03I17S00.wav",
    "assets/gga/B03I18S00.wav", "assets/gga/B03I19S00.wav", "assets/gga/B03I1AS00.wav", "assets/gga/B03I1BS00.wav",
    "assets/gga/B03I1CS00.wav", "assets/gga/B03I1DS00.wav", "assets/gga/B03I1ES00.wav", "assets/gga/B03I1FS00.wav",
    "assets/gga/B03I20S00.wav", "assets/gga/B03I21S00.wav", "assets/gga/B03I22S00.wav", "assets/gga/B03I23S00.wav",
    "assets/gga/B03I24S00.wav", "assets/gga/B03I25S00.wav", "assets/gga/B03I26S00.wav", "assets/gga/B03I27S00.wav",
    "assets/gga/B03I28S00.wav", "assets/gga/B03I29S00.wav", "assets/gga/B03I2AS00.wav", "assets/gga/B03I2BS00.wav",
    "assets/gga/B03I2CS00.wav", "assets/gga/B03I2DS00.wav", "assets/gga/B03I2ES00.wav", "assets/gga/B03I2FS00.wav",
    "assets/gga/B03I30S00.wav", "assets/gga/B03I31S00.wav", "assets/gga/B03I32S00.wav", "assets/gga/B03I33S00.wav",
    "assets/gga/B03I34S00.wav", "assets/gga/B03I35S00.wav", "assets/gga/B03I36S00.wav", "assets/gga/B03I37S00.wav",
    "assets/gga/B03I38S00.wav", "assets/gga/B03I39S00.wav", "assets/gga/B03I3AS00.wav", "assets/gga/B03I3BS00.wav",
    "assets/gga/B03I3CS00.wav", "assets/gga/B03I3DS00.wav", "assets/gga/B03I3ES00.wav", "assets/gga/B03I3FS00.wav",
    "assets/gga/B03I40S00.wav", "assets/gga/B03I41S00.wav", "assets/gga/B03I42S00.wav", "assets/gga/B03I43S00.wav",
    "assets/gga/B03I44S00.wav", "assets/gga/B03I45S00.wav", "assets/gga/B03I46S00.wav", "assets/gga/B03I47S00.wav",
    "assets/gga/B03I48S00.wav", "assets/gga/B03I49S00.wav", "assets/gga/B03I4AS00.wav", "assets/gga/B03I4BS00.wav",
    "assets/gga/B03I4CS00.wav", "assets/gga/B03I4DS00.wav", "assets/gga/B03I4ES00.wav", "assets/gga/B03I4FS00.wav",
    "assets/gga/B03I50S00.wav", "assets/gga/B03I51S00.wav", "assets/gga/B03I52S00.wav", "assets/gga/B03I53S00.wav",
    "assets/gga/B03I54S00.wav", "assets/gga/B03I55S00.wav", "assets/gga/B03I56S00.wav", "assets/gga/B03I57S00.wav",
    "assets/gga/B03I58S00.wav", "assets/gga/B03I59S00.wav", "assets/gga/B03I5AS00.wav", "assets/gga/B03I5BS00.wav",
    "assets/gga/B03I5CS00.wav", "assets/gga/B03I5DS00.wav", "assets/gga/B03I5ES00.wav", "assets/gga/B03I5FS00.wav",
    "assets/gga/B03I60S00.wav", "assets/gga/B03I61S00.wav", "assets/gga/B03I62S00.wav", "assets/gga/B03I63S00.wav",
    "assets/gga/B03I64S00.wav", "assets/gga/B03I65S00.wav", "assets/gga/B03I66S00.wav", "assets/gga/B03I67S00.wav",
    "assets/gga/B03I68S00.wav", "assets/gga/B03I69S00.wav", "assets/gga/B03I6AS00.wav", "assets/gga/B03I6BS00.wav",
    "assets/gga/B03I6CS00.wav", "assets/gga/B03I6DS00.wav", "assets/gga/B03I6ES00.wav", "assets/gga/B03I6FS00.wav",
    "assets/gga/B03I70S00.wav", "assets/gga/B03I71S00.wav", "assets/gga/B03I72S00.wav", "assets/gga/B03I73S00.wav",
    "assets/gga/B03I74S00.wav", "assets/gga/B03I75S00.wav", "assets/gga/B03I76S00.wav", "assets/gga/B03I77S00.wav"
};

const char** s_sample_file_paths_gga_banks[] = {
    s_sample_file_paths_gga_bank_0,
    s_sample_file_paths_gga_bank_1,
    s_sample_file_paths_gga_bank_2,
    s_sample_file_paths_gga_bank_3,
};

const char* s_sample_file_paths_iss2000_bank_0[] = {
    "assets/iss2000/B00I00S00.wav", "assets/iss2000/B00I01S00.wav", "assets/iss2000/B00I02S00.wav", "assets/iss2000/B00I03S00.wav",
    "assets/iss2000/B00I04S00.wav", "assets/iss2000/B00I05S00.wav", "assets/iss2000/B00I06S00.wav", "assets/iss2000/B00I07S00.wav",
    "assets/iss2000/B00I08S00.wav", "assets/iss2000/B00I09S00.wav", "assets/iss2000/B00I0AS00.wav", "assets/iss2000/B00I0BS00.wav",
    "assets/iss2000/B00I0CS00.wav", "assets/iss2000/B00I0DS00.wav", "assets/iss2000/B00I0ES00.wav", "assets/iss2000/B00I0FS00.wav",
    "assets/iss2000/B00I10S00.wav", "assets/iss2000/B00I11S00.wav", "assets/iss2000/B00I12S00.wav", "assets/iss2000/B00I13S00.wav",
    "assets/iss2000/B00I14S00.wav", "assets/iss2000/B00I15S00.wav", "assets/iss2000/B00I16S00.wav", "assets/iss2000/B00I17S00.wav",
    "assets/iss2000/B00I18S00.wav", "assets/iss2000/B00I19S00.wav", "assets/iss2000/B00I1AS00.wav", "assets/iss2000/B00I1BS00.wav",
    "assets/iss2000/B00I1CS00.wav", "assets/iss2000/B00I1DS00.wav", "assets/iss2000/B00I1ES00.wav", "assets/iss2000/B00I1FS00.wav",
    "assets/iss2000/B00I20S00.wav", "assets/iss2000/B00I21S00.wav", "assets/iss2000/B00I22S00.wav", "assets/iss2000/B00I23S00.wav",
    "assets/iss2000/B00I24S00.wav", "assets/iss2000/B00I25S00.wav", "assets/iss2000/B00I26S00.wav", "assets/iss2000/B00I27S00.wav",
    "assets/iss2000/B00I28S00.wav", "assets/iss2000/B00I29S00.wav", "assets/iss2000/B00I2AS00.wav", "assets/iss2000/B00I2BS00.wav",
    "assets/iss2000/B00I2CS00.wav", "assets/iss2000/B00I2DS00.wav", "assets/iss2000/B00I2ES00.wav", "assets/iss2000/B00I2FS00.wav",
    "assets/iss2000/B00I30S00.wav", "assets/iss2000/B00I31S00.wav", "assets/iss2000/B00I32S00.wav", "assets/iss2000/B00I33S00.wav",
    "assets/iss2000/B00I34S00.wav", "assets/iss2000/B00I35S00.wav", "assets/iss2000/B00I36S00.wav", "assets/iss2000/B00I37S00.wav",
    "assets/iss2000/B00I38S00.wav", "assets/iss2000/B00I39S00.wav", "assets/iss2000/B00I3AS00.wav", "assets/iss2000/B00I3BS00.wav",
    "assets/iss2000/B00I3CS00.wav", "assets/iss2000/B00I3DS00.wav", "assets/iss2000/B00I3ES00.wav", "assets/iss2000/B00I3FS00.wav",
    "assets/iss2000/B00I40S00.wav", "assets/iss2000/B00I41S00.wav", "assets/iss2000/B00I42S00.wav", "assets/iss2000/B00I43S00.wav",
    "assets/iss2000/B00I44S00.wav", "assets/iss2000/B00I45S00.wav", "assets/iss2000/B00I46S00.wav", "assets/iss2000/B00I47S00.wav",
    "assets/iss2000/B00I48S00.wav", "assets/iss2000/B00I49S00.wav", "assets/iss2000/B00I4AS00.wav", "assets/iss2000/B00I4BS00.wav",
    "assets/iss2000/B00I4CS00.wav", "assets/iss2000/B00I4DS00.wav", "assets/iss2000/B00I4ES00.wav", "assets/iss2000/B00I4FS00.wav",
    "assets/iss2000/B00I50S00.wav", "assets/iss2000/B00I51S00.wav", "assets/iss2000/B00I52S00.wav", "assets/iss2000/B00I53S00.wav",
    "assets/iss2000/B00I54S00.wav", "assets/iss2000/B00I55S00.wav", "assets/iss2000/B00I56S00.wav", "assets/iss2000/B00I57S00.wav",
    "assets/iss2000/B00I58S00.wav", "assets/iss2000/B00I59S00.wav", "assets/iss2000/B00I5AS00.wav", "assets/iss2000/B00I5BS00.wav",
    "assets/iss2000/B00I5CS00.wav", "assets/iss2000/B00I5DS00.wav", "assets/iss2000/B00I5ES00.wav", "assets/iss2000/B00I5FS00.wav",
    "assets/iss2000/B00I60S00.wav", "assets/iss2000/B00I61S00.wav", "assets/iss2000/B00I62S00.wav", "assets/iss2000/B00I63S00.wav",
    "assets/iss2000/B00I64S00.wav", "assets/iss2000/B00I65S00.wav", "assets/iss2000/B00I66S00.wav", "assets/iss2000/B00I67S00.wav",
    "assets/iss2000/B00I68S00.wav", "assets/iss2000/B00I69S00.wav", "assets/iss2000/B00I6AS00.wav", "assets/iss2000/B00I6BS00.wav",
    "assets/iss2000/B00I6CS00.wav", "assets/iss2000/B00I6DS00.wav", "assets/iss2000/B00I6ES00.wav", "assets/iss2000/B00I6FS00.wav",
    "assets/iss2000/B00I70S00.wav", "assets/iss2000/B00I71S00.wav", "assets/iss2000/B00I72S00.wav", "assets/iss2000/B00I73S00.wav",
    "assets/iss2000/B00I74S00.wav", "assets/iss2000/B00I75S00.wav", "assets/iss2000/B00I76S00.wav", "assets/iss2000/B00I77S00.wav",
    "assets/iss2000/B00I78S00.wav", "assets/iss2000/B00I79S00.wav", "assets/iss2000/B00I7AS00.wav", "assets/iss2000/B00I7BS00.wav",
    "assets/iss2000/B00I7CS00.wav", "assets/iss2000/B00I7DS00.wav", "assets/iss2000/B00I7ES00.wav", "assets/iss2000/B00I7FS00.wav",
    "assets/iss2000/B00I80S00.wav", "assets/iss2000/B00I81S00.wav", "assets/iss2000/B00I82S00.wav", "assets/iss2000/B00I83S00.wav",
    "assets/iss2000/B00I84S00.wav", "assets/iss2000/B00I85S00.wav", "assets/iss2000/B00I86S00.wav", "assets/iss2000/B00I87S00.wav",
    "assets/iss2000/B00I88S00.wav", "assets/iss2000/B00I89S00.wav", "assets/iss2000/B00I8AS00.wav", "assets/iss2000/B00I8BS00.wav",
    "assets/iss2000/B00I8CS00.wav", "assets/iss2000/B00I8DS00.wav", "assets/iss2000/B00I8ES00.wav", "assets/iss2000/B00I8FS00.wav",
    "assets/iss2000/B00I90S00.wav", "assets/iss2000/B00I91S00.wav", "assets/iss2000/B00I92S00.wav", "assets/iss2000/B00I93S00.wav",
    "assets/iss2000/B00I94S00.wav", "assets/iss2000/B00I95S00.wav", "assets/iss2000/B00I96S00.wav", "assets/iss2000/B00I97S00.wav",
    "assets/iss2000/B00I98S00.wav", "assets/iss2000/B00I99S00.wav", "assets/iss2000/B00I9AS00.wav", "assets/iss2000/B00I9BS00.wav",
    "assets/iss2000/B00I9CS00.wav", "assets/iss2000/B00I9DS00.wav", "assets/iss2000/B00I9ES00.wav", "assets/iss2000/B00I9FS00.wav",
    "assets/iss2000/B00IA0S00.wav", "assets/iss2000/B00IA1S00.wav", "assets/iss2000/B00IA2S00.wav", "assets/iss2000/B00IA3S00.wav",
    "assets/iss2000/B00IA4S00.wav", "assets/iss2000/B00IA5S00.wav", "assets/iss2000/B00IA6S00.wav", "assets/iss2000/B00IA7S00.wav",
    "assets/iss2000/B00IA8S00.wav", "assets/iss2000/B00IA9S00.wav", "assets/iss2000/B00IAAS00.wav", "assets/iss2000/B00IABS00.wav",
    "assets/iss2000/B00IACS00.wav", "assets/iss2000/B00IADS00.wav", "assets/iss2000/B00IAES00.wav", "assets/iss2000/B00IAFS00.wav",
    "assets/iss2000/B00IB0S00.wav", "assets/iss2000/B00IB1S00.wav"
};

const char** s_sample_file_paths_iss2000_banks[] = {
    s_sample_file_paths_iss2000_bank_0,
    s_sample_file_paths_mnsg_bank_1,
    s_sample_file_paths_mnsg_bank_2,
};

const char* s_sample_file_paths_iss64_bank_0[] = {
    "assets/iss64/B00I00S00.wav", "assets/iss64/B00I01S00.wav", "assets/iss64/B00I02S00.wav", "assets/iss64/B00I03S00.wav",
    "assets/iss64/B00I04S00.wav", "assets/iss64/B00I05S00.wav", "assets/iss64/B00I06S00.wav", "assets/iss64/B00I07S00.wav",
    "assets/iss64/B00I08S00.wav", "assets/iss64/B00I09S00.wav", "assets/iss64/B00I0AS00.wav", "assets/iss64/B00I0BS00.wav",
    "assets/iss64/B00I0CS00.wav", "assets/iss64/B00I0DS00.wav", "assets/iss64/B00I0ES00.wav", "assets/iss64/B00I0FS00.wav",
    "assets/iss64/B00I10S00.wav", "assets/iss64/B00I11S00.wav", "assets/iss64/B00I12S00.wav", "assets/iss64/B00I13S00.wav",
    "assets/iss64/B00I14S00.wav", "assets/iss64/B00I15S00.wav", "assets/iss64/B00I16S00.wav", "assets/iss64/B00I17S00.wav",
    "assets/iss64/B00I18S00.wav", "assets/iss64/B00I19S00.wav", "assets/iss64/B00I1AS00.wav", "assets/iss64/B00I1BS00.wav",
    "assets/iss64/B00I1CS00.wav", "assets/iss64/B00I1DS00.wav", "assets/iss64/B00I1ES00.wav", "assets/iss64/B00I1FS00.wav",
    "assets/iss64/B00I20S00.wav", "assets/iss64/B00I21S00.wav", "assets/iss64/B00I22S00.wav", "assets/iss64/B00I23S00.wav",
    "assets/iss64/B00I24S00.wav", "assets/iss64/B00I25S00.wav", "assets/iss64/B00I26S00.wav", "assets/iss64/B00I27S00.wav",
};

const char* s_sample_file_paths_iss64_bank_1[] = {
    "assets/iss64/B01I00S00.wav", "assets/iss64/B01I01S00.wav", "assets/iss64/B01I02S00.wav", "assets/iss64/B01I03S00.wav",
    "assets/iss64/B01I04S00.wav", "assets/iss64/B01I05S00.wav", "assets/iss64/B01I06S00.wav", "assets/iss64/B01I07S00.wav",
    "assets/iss64/B01I08S00.wav", "assets/iss64/B01I09S00.wav", "assets/iss64/B01I0AS00.wav", "assets/iss64/B01I0BS00.wav",
    "assets/iss64/B01I0CS00.wav", "assets/iss64/B01I0DS00.wav", "assets/iss64/B01I0ES00.wav", "assets/iss64/B01I0FS00.wav",
    "assets/iss64/B01I10S00.wav", "assets/iss64/B01I11S00.wav", "assets/iss64/B01I12S00.wav", "assets/iss64/B01I13S00.wav",
    "assets/iss64/B01I14S00.wav", "assets/iss64/B01I15S00.wav", "assets/iss64/B01I16S00.wav", "assets/iss64/B01I17S00.wav",
    "assets/iss64/B01I18S00.wav", "assets/iss64/B01I19S00.wav", "assets/iss64/B01I1AS00.wav", "assets/iss64/B01I1BS00.wav",
    "assets/iss64/B01I1CS00.wav", "assets/iss64/B01I1DS00.wav", "assets/iss64/B01I1ES00.wav", "assets/iss64/B01I1FS00.wav",
    "assets/iss64/B01I20S00.wav", "assets/iss64/B01I21S00.wav", "assets/iss64/B01I22S00.wav", "assets/iss64/B01I23S00.wav",
    "assets/iss64/B01I24S00.wav", "assets/iss64/B01I25S00.wav", "assets/iss64/B01I26S00.wav",
};

const char* s_sample_file_paths_iss64_bank_2[] = {
    "assets/iss64/B02I00S00.wav", "assets/iss64/B02I01S00.wav", "assets/iss64/B02I02S00.wav", "assets/iss64/B02I03S00.wav",
    "assets/iss64/B02I04S00.wav", "assets/iss64/B02I05S00.wav", "assets/iss64/B02I06S00.wav", "assets/iss64/B02I07S00.wav",
    "assets/iss64/B02I08S00.wav", "assets/iss64/B02I09S00.wav", "assets/iss64/B02I0AS00.wav", "assets/iss64/B02I0BS00.wav",
    "assets/iss64/B02I0CS00.wav", "assets/iss64/B02I0DS00.wav", "assets/iss64/B02I0ES00.wav", "assets/iss64/B02I0FS00.wav",
    "assets/iss64/B02I10S00.wav", "assets/iss64/B02I11S00.wav", "assets/iss64/B02I12S00.wav", "assets/iss64/B02I13S00.wav",
    "assets/iss64/B02I14S00.wav", "assets/iss64/B02I15S00.wav", "assets/iss64/B02I16S00.wav", "assets/iss64/B02I17S00.wav",
    "assets/iss64/B02I18S00.wav", "assets/iss64/B02I19S00.wav", "assets/iss64/B02I1AS00.wav", "assets/iss64/B02I1BS00.wav",
    "assets/iss64/B02I1CS00.wav", "assets/iss64/B02I1DS00.wav", "assets/iss64/B02I1ES00.wav", "assets/iss64/B02I1FS00.wav",
    "assets/iss64/B02I20S00.wav", "assets/iss64/B02I21S00.wav", "assets/iss64/B02I22S00.wav", "assets/iss64/B02I23S00.wav",
    "assets/iss64/B02I24S00.wav", "assets/iss64/B02I25S00.wav", "assets/iss64/B02I26S00.wav", "assets/iss64/B02I27S00.wav",
    "assets/iss64/B02I28S00.wav", "assets/iss64/B02I29S00.wav", "assets/iss64/B02I2AS00.wav", "assets/iss64/B02I2BS00.wav",
    "assets/iss64/B02I2CS00.wav", "assets/iss64/B02I2DS00.wav", "assets/iss64/B02I2ES00.wav", "assets/iss64/B02I2FS00.wav",
    "assets/iss64/B02I30S00.wav", "assets/iss64/B02I31S00.wav", "assets/iss64/B02I32S00.wav", "assets/iss64/B02I33S00.wav",
    "assets/iss64/B02I34S00.wav", "assets/iss64/B02I35S00.wav", "assets/iss64/B02I36S00.wav", "assets/iss64/B02I37S00.wav",
    "assets/iss64/B02I38S00.wav", "assets/iss64/B02I39S00.wav", "assets/iss64/B02I3AS00.wav", "assets/iss64/B02I3BS00.wav",
    "assets/iss64/B02I3CS00.wav", "assets/iss64/B02I3DS00.wav", "assets/iss64/B02I3ES00.wav", "assets/iss64/B02I3FS00.wav",
    "assets/iss64/B02I40S00.wav", "assets/iss64/B02I41S00.wav", "assets/iss64/B02I42S00.wav", "assets/iss64/B02I43S00.wav",
    "assets/iss64/B02I44S00.wav", "assets/iss64/B02I45S00.wav", "assets/iss64/B02I46S00.wav", "assets/iss64/B02I47S00.wav",
    "assets/iss64/B02I48S00.wav", "assets/iss64/B02I49S00.wav", "assets/iss64/B02I4AS00.wav", "assets/iss64/B02I4BS00.wav",
    "assets/iss64/B02I4CS00.wav", "assets/iss64/B02I4DS00.wav", "assets/iss64/B02I4ES00.wav", "assets/iss64/B02I4FS00.wav",
    "assets/iss64/B02I50S00.wav", "assets/iss64/B02I51S00.wav", "assets/iss64/B02I52S00.wav", "assets/iss64/B02I53S00.wav",
    "assets/iss64/B02I54S00.wav", "assets/iss64/B02I55S00.wav", "assets/iss64/B02I56S00.wav", "assets/iss64/B02I57S00.wav",
    "assets/iss64/B02I58S00.wav", "assets/iss64/B02I59S00.wav", "assets/iss64/B02I5AS00.wav", "assets/iss64/B02I5BS00.wav",
    "assets/iss64/B02I5CS00.wav", "assets/iss64/B02I5DS00.wav", "assets/iss64/B02I5ES00.wav", "assets/iss64/B02I5FS00.wav",
    "assets/iss64/B02I60S00.wav", "assets/iss64/B02I61S00.wav", "assets/iss64/B02I62S00.wav", "assets/iss64/B02I63S00.wav",
    "assets/iss64/B02I64S00.wav", "assets/iss64/B02I65S00.wav", "assets/iss64/B02I66S00.wav", "assets/iss64/B02I67S00.wav",
    "assets/iss64/B02I68S00.wav", "assets/iss64/B02I69S00.wav", "assets/iss64/B02I6AS00.wav", "assets/iss64/B02I6BS00.wav",
    "assets/iss64/B02I6CS00.wav", "assets/iss64/B02I6DS00.wav", "assets/iss64/B02I6ES00.wav", "assets/iss64/B02I6FS00.wav",
    "assets/iss64/B02I70S00.wav", "assets/iss64/B02I71S00.wav", "assets/iss64/B02I72S00.wav", "assets/iss64/B02I73S00.wav",
    "assets/iss64/B02I74S00.wav", "assets/iss64/B02I75S00.wav", "assets/iss64/B02I76S00.wav", "assets/iss64/B02I77S00.wav",
    "assets/iss64/B02I78S00.wav", "assets/iss64/B02I79S00.wav", "assets/iss64/B02I7AS00.wav", "assets/iss64/B02I7BS00.wav",
    "assets/iss64/B02I7CS00.wav", "assets/iss64/B02I7DS00.wav", "assets/iss64/B02I7ES00.wav", "assets/iss64/B02I7FS00.wav",
    "assets/iss64/B02I80S00.wav", "assets/iss64/B02I81S00.wav", "assets/iss64/B02I82S00.wav", "assets/iss64/B02I83S00.wav",
    "assets/iss64/B02I84S00.wav", "assets/iss64/B02I85S00.wav", "assets/iss64/B02I86S00.wav", "assets/iss64/B02I87S00.wav",
    "assets/iss64/B02I88S00.wav", "assets/iss64/B02I89S00.wav", "assets/iss64/B02I8AS00.wav", "assets/iss64/B02I8BS00.wav",
    "assets/iss64/B02I8CS00.wav", "assets/iss64/B02I8DS00.wav", "assets/iss64/B02I8ES00.wav", "assets/iss64/B02I8FS00.wav",
    "assets/iss64/B02I90S00.wav", "assets/iss64/B02I91S00.wav", "assets/iss64/B02I92S00.wav", "assets/iss64/B02I93S00.wav",
    "assets/iss64/B02I94S00.wav", "assets/iss64/B02I95S00.wav", "assets/iss64/B02I96S00.wav", "assets/iss64/B02I97S00.wav",
    "assets/iss64/B02I98S00.wav", "assets/iss64/B02I99S00.wav", "assets/iss64/B02I9AS00.wav", "assets/iss64/B02I9BS00.wav",
    "assets/iss64/B02I9CS00.wav", "assets/iss64/B02I9DS00.wav", "assets/iss64/B02I9ES00.wav", "assets/iss64/B02I9FS00.wav",
    "assets/iss64/B02IA0S00.wav", "assets/iss64/B02IA1S00.wav", "assets/iss64/B02IA2S00.wav", "assets/iss64/B02IA3S00.wav",
    "assets/iss64/B02IA4S00.wav", "assets/iss64/B02IA5S00.wav", "assets/iss64/B02IA6S00.wav", "assets/iss64/B02IA7S00.wav",
    "assets/iss64/B02IA8S00.wav", "assets/iss64/B02IA9S00.wav", "assets/iss64/B02IAAS00.wav", "assets/iss64/B02IABS00.wav",
    "assets/iss64/B02IACS00.wav", "assets/iss64/B02IADS00.wav", "assets/iss64/B02IAES00.wav", "assets/iss64/B02IAFS00.wav",
    "assets/iss64/B02IB0S00.wav", "assets/iss64/B02IB1S00.wav", "assets/iss64/B02IB2S00.wav", "assets/iss64/B02IB3S00.wav",
    "assets/iss64/B02IB4S00.wav", "assets/iss64/B02IB5S00.wav", "assets/iss64/B02IB6S00.wav", "assets/iss64/B02IB7S00.wav",
    "assets/iss64/B02IB8S00.wav", "assets/iss64/B02IB9S00.wav", "assets/iss64/B02IBAS00.wav", "assets/iss64/B02IBBS00.wav",
    "assets/iss64/B02IBCS00.wav", "assets/iss64/B02IBDS00.wav", "assets/iss64/B02IBES00.wav", "assets/iss64/B02IBFS00.wav",
    "assets/iss64/B02IC0S00.wav", "assets/iss64/B02IC1S00.wav", "assets/iss64/B02IC2S00.wav", "assets/iss64/B02IC3S00.wav",
    "assets/iss64/B02IC4S00.wav", "assets/iss64/B02IC5S00.wav", "assets/iss64/B02IC6S00.wav", "assets/iss64/B02IC7S00.wav",
    "assets/iss64/B02IC8S00.wav", "assets/iss64/B02IC9S00.wav", "assets/iss64/B02ICAS00.wav", "assets/iss64/B02ICBS00.wav",
    "assets/iss64/B02ICCS00.wav", "assets/iss64/B02ICDS00.wav", "assets/iss64/B02ICES00.wav", "assets/iss64/B02ICFS00.wav",
    "assets/iss64/B02ID0S00.wav", "assets/iss64/B02ID1S00.wav", "assets/iss64/B02ID2S00.wav", "assets/iss64/B02ID3S00.wav",
    "assets/iss64/B02ID4S00.wav", "assets/iss64/B02ID5S00.wav", "assets/iss64/B02ID6S00.wav", "assets/iss64/B02ID7S00.wav",
    "assets/iss64/B02ID8S00.wav", "assets/iss64/B02ID9S00.wav", "assets/iss64/B02IDAS00.wav", "assets/iss64/B02IDBS00.wav",
    "assets/iss64/B02IDCS00.wav", "assets/iss64/B02IDDS00.wav", "assets/iss64/B02IDES00.wav", "assets/iss64/B02IDFS00.wav",
    "assets/iss64/B02IE0S00.wav", "assets/iss64/B02IE1S00.wav", "assets/iss64/B02IE2S00.wav", "assets/iss64/B02IE3S00.wav",
    "assets/iss64/B02IE4S00.wav", "assets/iss64/B02IE5S00.wav", "assets/iss64/B02IE6S00.wav", "assets/iss64/B02IE7S00.wav",
    "assets/iss64/B02IE8S00.wav", "assets/iss64/B02IE9S00.wav", "assets/iss64/B02IEAS00.wav", "assets/iss64/B02IEBS00.wav",
    "assets/iss64/B02IECS00.wav", "assets/iss64/B02IEDS00.wav", "assets/iss64/B02IEES00.wav", "assets/iss64/B02IEFS00.wav",
    "assets/iss64/B02IF0S00.wav", "assets/iss64/B02IF1S00.wav", "assets/iss64/B02IF2S00.wav", "assets/iss64/B02IF3S00.wav",
    "assets/iss64/B02IF4S00.wav", "assets/iss64/B02IF5S00.wav"
};

const char* s_sample_file_paths_iss64_bank_3[] = {
    "assets/iss64/B03I00S00.wav", "assets/iss64/B03I01S00.wav", "assets/iss64/B03I02S00.wav", "assets/iss64/B03I03S00.wav",
    "assets/iss64/B03I04S00.wav", "assets/iss64/B03I05S00.wav", "assets/iss64/B03I06S00.wav", "assets/iss64/B03I07S00.wav",
    "assets/iss64/B03I08S00.wav", "assets/iss64/B03I09S00.wav", "assets/iss64/B03I0AS00.wav", "assets/iss64/B03I0BS00.wav",
    "assets/iss64/B03I0CS00.wav", "assets/iss64/B03I0DS00.wav", "assets/iss64/B03I0ES00.wav", "assets/iss64/B03I0FS00.wav",
    "assets/iss64/B03I10S00.wav", "assets/iss64/B03I11S00.wav", "assets/iss64/B03I12S00.wav", "assets/iss64/B03I13S00.wav",
    "assets/iss64/B03I14S00.wav", "assets/iss64/B03I15S00.wav", "assets/iss64/B03I16S00.wav", "assets/iss64/B03I17S00.wav",
    "assets/iss64/B03I18S00.wav", "assets/iss64/B03I19S00.wav", "assets/iss64/B03I1AS00.wav", "assets/iss64/B03I1BS00.wav",
    "assets/iss64/B03I1CS00.wav", "assets/iss64/B03I1DS00.wav", "assets/iss64/B03I1ES00.wav", "assets/iss64/B03I1FS00.wav",
    "assets/iss64/B03I20S00.wav", "assets/iss64/B03I21S00.wav", "assets/iss64/B03I22S00.wav", "assets/iss64/B03I23S00.wav",
    "assets/iss64/B03I24S00.wav", "assets/iss64/B03I25S00.wav", "assets/iss64/B03I26S00.wav", "assets/iss64/B03I27S00.wav",
    "assets/iss64/B03I28S00.wav", "assets/iss64/B03I29S00.wav", "assets/iss64/B03I2AS00.wav", "assets/iss64/B03I2BS00.wav",
    "assets/iss64/B03I2CS00.wav", "assets/iss64/B03I2DS00.wav", "assets/iss64/B03I2ES00.wav", "assets/iss64/B03I2FS00.wav",
    "assets/iss64/B03I30S00.wav", "assets/iss64/B03I31S00.wav", "assets/iss64/B03I32S00.wav", "assets/iss64/B03I33S00.wav",
    "assets/iss64/B03I34S00.wav", "assets/iss64/B03I35S00.wav", "assets/iss64/B03I36S00.wav", "assets/iss64/B03I37S00.wav",
    "assets/iss64/B03I38S00.wav", "assets/iss64/B03I39S00.wav", "assets/iss64/B03I3AS00.wav", "assets/iss64/B03I3BS00.wav",
    "assets/iss64/B03I3CS00.wav", "assets/iss64/B03I3DS00.wav", "assets/iss64/B03I3ES00.wav", "assets/iss64/B03I3FS00.wav",
    "assets/iss64/B03I40S00.wav", "assets/iss64/B03I41S00.wav", "assets/iss64/B03I42S00.wav", "assets/iss64/B03I43S00.wav",
    "assets/iss64/B03I44S00.wav", "assets/iss64/B03I45S00.wav", "assets/iss64/B03I46S00.wav", "assets/iss64/B03I47S00.wav",
    "assets/iss64/B03I48S00.wav", "assets/iss64/B03I49S00.wav", "assets/iss64/B03I4AS00.wav", "assets/iss64/B03I4BS00.wav",
    "assets/iss64/B03I4CS00.wav", "assets/iss64/B03I4DS00.wav", "assets/iss64/B03I4ES00.wav", "assets/iss64/B03I4FS00.wav",
    "assets/iss64/B03I50S00.wav", "assets/iss64/B03I51S00.wav", "assets/iss64/B03I52S00.wav", "assets/iss64/B03I53S00.wav",
    "assets/iss64/B03I54S00.wav", "assets/iss64/B03I55S00.wav", "assets/iss64/B03I56S00.wav", "assets/iss64/B03I57S00.wav",
    "assets/iss64/B03I58S00.wav", "assets/iss64/B03I59S00.wav", "assets/iss64/B03I5AS00.wav", "assets/iss64/B03I5BS00.wav",
    "assets/iss64/B03I5CS00.wav", "assets/iss64/B03I5DS00.wav", "assets/iss64/B03I5ES00.wav", "assets/iss64/B03I5FS00.wav",
    "assets/iss64/B03I60S00.wav", "assets/iss64/B03I61S00.wav", "assets/iss64/B03I62S00.wav", "assets/iss64/B03I63S00.wav",
    "assets/iss64/B03I64S00.wav", "assets/iss64/B03I65S00.wav", "assets/iss64/B03I66S00.wav", "assets/iss64/B03I67S00.wav",
    "assets/iss64/B03I68S00.wav", "assets/iss64/B03I69S00.wav", "assets/iss64/B03I6AS00.wav", "assets/iss64/B03I6BS00.wav",
    "assets/iss64/B03I6CS00.wav", "assets/iss64/B03I6DS00.wav", "assets/iss64/B03I6ES00.wav", "assets/iss64/B03I6FS00.wav",
    "assets/iss64/B03I70S00.wav", "assets/iss64/B03I71S00.wav", "assets/iss64/B03I72S00.wav", "assets/iss64/B03I73S00.wav",
    "assets/iss64/B03I74S00.wav", "assets/iss64/B03I75S00.wav", "assets/iss64/B03I76S00.wav", "assets/iss64/B03I77S00.wav",
    "assets/iss64/B03I78S00.wav", "assets/iss64/B03I79S00.wav", "assets/iss64/B03I7AS00.wav", "assets/iss64/B03I7BS00.wav",
    "assets/iss64/B03I7CS00.wav", "assets/iss64/B03I7DS00.wav", "assets/iss64/B03I7ES00.wav", "assets/iss64/B03I7FS00.wav",
    "assets/iss64/B03I80S00.wav", "assets/iss64/B03I81S00.wav", "assets/iss64/B03I82S00.wav", "assets/iss64/B03I83S00.wav",
    "assets/iss64/B03I84S00.wav", "assets/iss64/B03I85S00.wav", "assets/iss64/B03I86S00.wav", "assets/iss64/B03I87S00.wav",
    "assets/iss64/B03I88S00.wav", "assets/iss64/B03I89S00.wav", "assets/iss64/B03I8AS00.wav", "assets/iss64/B03I8BS00.wav",
    "assets/iss64/B03I8CS00.wav", "assets/iss64/B03I8DS00.wav", "assets/iss64/B03I8ES00.wav", "assets/iss64/B03I8FS00.wav",
    "assets/iss64/B03I90S00.wav", "assets/iss64/B03I91S00.wav", "assets/iss64/B03I92S00.wav", "assets/iss64/B03I93S00.wav",
    "assets/iss64/B03I94S00.wav", "assets/iss64/B03I95S00.wav", "assets/iss64/B03I96S00.wav", "assets/iss64/B03I97S00.wav",
    "assets/iss64/B03I98S00.wav", "assets/iss64/B03I99S00.wav", "assets/iss64/B03I9AS00.wav", "assets/iss64/B03I9BS00.wav",
    "assets/iss64/B03I9CS00.wav", "assets/iss64/B03I9DS00.wav", "assets/iss64/B03I9ES00.wav", "assets/iss64/B03I9FS00.wav",
    "assets/iss64/B03IA0S00.wav", "assets/iss64/B03IA1S00.wav", "assets/iss64/B03IA2S00.wav", "assets/iss64/B03IA3S00.wav",
    "assets/iss64/B03IA4S00.wav", "assets/iss64/B03IA5S00.wav", "assets/iss64/B03IA6S00.wav", "assets/iss64/B03IA7S00.wav",
    "assets/iss64/B03IA8S00.wav", "assets/iss64/B03IA9S00.wav", "assets/iss64/B03IAAS00.wav", "assets/iss64/B03IABS00.wav",
    "assets/iss64/B03IACS00.wav", "assets/iss64/B03IADS00.wav", "assets/iss64/B03IAES00.wav", "assets/iss64/B03IAFS00.wav",
    "assets/iss64/B03IB0S00.wav", "assets/iss64/B03IB1S00.wav", "assets/iss64/B03IB2S00.wav", "assets/iss64/B03IB3S00.wav",
    "assets/iss64/B03IB4S00.wav", "assets/iss64/B03IB5S00.wav", "assets/iss64/B03IB6S00.wav", "assets/iss64/B03IB7S00.wav",
    "assets/iss64/B03IB8S00.wav", "assets/iss64/B03IB9S00.wav", "assets/iss64/B03IBAS00.wav", "assets/iss64/B03IBBS00.wav",
    "assets/iss64/B03IBCS00.wav", "assets/iss64/B03IBDS00.wav",
};

const char** s_sample_file_paths_iss64_banks[] = {
    s_sample_file_paths_iss64_bank_0,
    s_sample_file_paths_iss64_bank_1,
    s_sample_file_paths_iss64_bank_2,
    s_sample_file_paths_iss64_bank_3,
};

const char* s_sample_file_paths_cv_bank_0[] = {
    "assets/cv/B00I00S00.wav", "assets/cv/B00I01S00.wav", "assets/cv/B00I02S00.wav", "assets/cv/B00I03S00.wav",
    "assets/cv/B00I04S00.wav", "assets/cv/B00I05S00.wav", "assets/cv/B00I06S00.wav", "assets/cv/B00I07S00.wav",
    "assets/cv/B00I08S00.wav", "assets/cv/B00I09S00.wav", "assets/cv/B00I0AS00.wav", "assets/cv/B00I0BS00.wav",
    "assets/cv/B00I0CS00.wav", "assets/cv/B00I0DS00.wav", "assets/cv/B00I0ES00.wav", "assets/cv/B00I0FS00.wav",
    "assets/cv/B00I10S00.wav", "assets/cv/B00I11S00.wav", "assets/cv/B00I12S00.wav", "assets/cv/B00I13S00.wav",
    "assets/cv/B00I14S00.wav", "assets/cv/B00I15S00.wav", "assets/cv/B00I16S00.wav", "assets/cv/B00I17S00.wav",
    "assets/cv/B00I18S00.wav", "assets/cv/B00I19S00.wav", "assets/cv/B00I1AS00.wav", "assets/cv/B00I1BS00.wav",
    "assets/cv/B00I1CS00.wav", "assets/cv/B00I1DS00.wav", "assets/cv/B00I1ES00.wav", "assets/cv/B00I1FS00.wav",
    "assets/cv/B00I20S00.wav", "assets/cv/B00I21S00.wav", "assets/cv/B00I22S00.wav", "assets/cv/B00I23S00.wav",
    "assets/cv/B00I24S00.wav", "assets/cv/B00I25S00.wav", "assets/cv/B00I26S00.wav", "assets/cv/B00I27S00.wav",
    "assets/cv/B00I28S00.wav", "assets/cv/B00I29S00.wav", "assets/cv/B00I2AS00.wav", "assets/cv/B00I2BS00.wav",
    "assets/cv/B00I2CS00.wav", "assets/cv/B00I2DS00.wav", "assets/cv/B00I2ES00.wav", "assets/cv/B00I2FS00.wav",
    "assets/cv/B00I30S00.wav", "assets/cv/B00I31S00.wav", "assets/cv/B00I32S00.wav", "assets/cv/B00I33S00.wav",
    "assets/cv/B00I34S00.wav", "assets/cv/B00I35S00.wav", "assets/cv/B00I36S00.wav", "assets/cv/B00I37S00.wav",
    "assets/cv/B00I38S00.wav", "assets/cv/B00I39S00.wav", "assets/cv/B00I3AS00.wav", "assets/cv/B00I3BS00.wav",
    "assets/cv/B00I3CS00.wav", "assets/cv/B00I3DS00.wav", "assets/cv/B00I3ES00.wav", "assets/cv/B00I3FS00.wav",
    "assets/cv/B00I40S00.wav", "assets/cv/B00I41S00.wav", "assets/cv/B00I42S00.wav", "assets/cv/B00I43S00.wav",
    "assets/cv/B00I44S00.wav", "assets/cv/B00I45S00.wav", "assets/cv/B00I46S00.wav", "assets/cv/B00I47S00.wav",
    "assets/cv/B00I48S00.wav", "assets/cv/B00I49S00.wav", "assets/cv/B00I4AS00.wav", "assets/cv/B00I4BS00.wav",
    "assets/cv/B00I4CS00.wav", "assets/cv/B00I4DS00.wav", "assets/cv/B00I4ES00.wav", "assets/cv/B00I4FS00.wav",
    "assets/cv/B00I50S00.wav", "assets/cv/B00I51S00.wav", "assets/cv/B00I52S00.wav", "assets/cv/B00I53S00.wav",
    "assets/cv/B00I54S00.wav", "assets/cv/B00I55S00.wav", "assets/cv/B00I56S00.wav", "assets/cv/B00I57S00.wav",
    "assets/cv/B00I58S00.wav", "assets/cv/B00I59S00.wav", "assets/cv/B00I5AS00.wav", "assets/cv/B00I5BS00.wav",
    "assets/cv/B00I5CS00.wav", "assets/cv/B00I5DS00.wav"
};

const char* s_sample_file_paths_cv_bank_1[] = {
    "assets/cv/B01I00S00.wav", "assets/cv/B01I01S00.wav", "assets/cv/B01I02S00.wav", "assets/cv/B01I03S00.wav",
    "assets/cv/B01I04S00.wav", "assets/cv/B01I05S00.wav", "assets/cv/B01I06S00.wav", "assets/cv/B01I07S00.wav",
    "assets/cv/B01I08S00.wav", "assets/cv/B01I09S00.wav", "assets/cv/B01I0AS00.wav", "assets/cv/B01I0BS00.wav",
    "assets/cv/B01I0CS00.wav", "assets/cv/B01I0DS00.wav", "assets/cv/B01I0ES00.wav", "assets/cv/B01I0FS00.wav",
    "assets/cv/B01I10S00.wav", "assets/cv/B01I11S00.wav", "assets/cv/B01I12S00.wav", "assets/cv/B01I13S00.wav",
    "assets/cv/B01I14S00.wav", "assets/cv/B01I15S00.wav", "assets/cv/B01I16S00.wav", "assets/cv/B01I17S00.wav",
    "assets/cv/B01I18S00.wav", "assets/cv/B01I19S00.wav", "assets/cv/B01I1AS00.wav", "assets/cv/B01I1BS00.wav",
    "assets/cv/B01I1CS00.wav", "assets/cv/B01I1DS00.wav", "assets/cv/B01I1ES00.wav", "assets/cv/B01I1FS00.wav",
    "assets/cv/B01I20S00.wav", "assets/cv/B01I21S00.wav", "assets/cv/B01I22S00.wav", "assets/cv/B01I23S00.wav",
    "assets/cv/B01I24S00.wav", "assets/cv/B01I25S00.wav", "assets/cv/B01I26S00.wav", "assets/cv/B01I27S00.wav",
    "assets/cv/B01I28S00.wav", "assets/cv/B01I29S00.wav", "assets/cv/B01I2AS00.wav", "assets/cv/B01I2BS00.wav",
    "assets/cv/B01I2CS00.wav", "assets/cv/B01I2DS00.wav", "assets/cv/B01I2ES00.wav", "assets/cv/B01I2FS00.wav",
    "assets/cv/B01I30S00.wav", "assets/cv/B01I31S00.wav", "assets/cv/B01I32S00.wav", "assets/cv/B01I33S00.wav",
    "assets/cv/B01I34S00.wav", "assets/cv/B01I35S00.wav", "assets/cv/B01I36S00.wav", "assets/cv/B01I37S00.wav",
    "assets/cv/B01I38S00.wav", "assets/cv/B01I39S00.wav", "assets/cv/B01I3AS00.wav", "assets/cv/B01I3BS00.wav",
    "assets/cv/B01I3CS00.wav", "assets/cv/B01I3DS00.wav", "assets/cv/B01I3ES00.wav", "assets/cv/B01I3FS00.wav",
    "assets/cv/B01I40S00.wav", "assets/cv/B01I41S00.wav", "assets/cv/B01I42S00.wav", "assets/cv/B01I43S00.wav",
    "assets/cv/B01I44S00.wav", "assets/cv/B01I45S00.wav", "assets/cv/B01I46S00.wav", "assets/cv/B01I47S00.wav",
    "assets/cv/B01I48S00.wav", "assets/cv/B01I49S00.wav", "assets/cv/B01I4AS00.wav", "assets/cv/B01I4BS00.wav",
    "assets/cv/B01I4CS00.wav", "assets/cv/B01I4DS00.wav", "assets/cv/B01I4ES00.wav", "assets/cv/B01I4FS00.wav",
    "assets/cv/B01I50S00.wav", "assets/cv/B01I51S00.wav", "assets/cv/B01I52S00.wav", "assets/cv/B01I53S00.wav",
    "assets/cv/B01I54S00.wav", "assets/cv/B01I55S00.wav", "assets/cv/B01I56S00.wav", "assets/cv/B01I57S00.wav",
    "assets/cv/B01I58S00.wav", "assets/cv/B01I59S00.wav", "assets/cv/B01I5AS00.wav", "assets/cv/B01I5BS00.wav",
    "assets/cv/B01I5CS00.wav", "assets/cv/B01I5DS00.wav", "assets/cv/B01I5ES00.wav", "assets/cv/B01I5FS00.wav",
    "assets/cv/B01I60S00.wav", "assets/cv/B01I61S00.wav", "assets/cv/B01I62S00.wav", "assets/cv/B01I63S00.wav",
    "assets/cv/B01I64S00.wav", "assets/cv/B01I65S00.wav", "assets/cv/B01I66S00.wav", "assets/cv/B01I67S00.wav",
    "assets/cv/B01I68S00.wav", "assets/cv/B01I69S00.wav", "assets/cv/B01I6AS00.wav", "assets/cv/B01I6BS00.wav",
    "assets/cv/B01I6CS00.wav", "assets/cv/B01I6DS00.wav", "assets/cv/B01I6ES00.wav", "assets/cv/B01I6FS00.wav",
    "assets/cv/B01I70S00.wav", "assets/cv/B01I71S00.wav", "assets/cv/B01I72S00.wav", "assets/cv/B01I73S00.wav",
    "assets/cv/B01I74S00.wav", "assets/cv/B01I75S00.wav", "assets/cv/B01I76S00.wav", "assets/cv/B01I77S00.wav",
    "assets/cv/B01I78S00.wav", "assets/cv/B01I79S00.wav", "assets/cv/B01I7AS00.wav", "assets/cv/B01I7BS00.wav",
    "assets/cv/B01I7CS00.wav", "assets/cv/B01I7DS00.wav", "assets/cv/B01I7ES00.wav", "assets/cv/B01I7FS00.wav",
    "assets/cv/B01I80S00.wav", "assets/cv/B01I81S00.wav", "assets/cv/B01I82S00.wav", "assets/cv/B01I83S00.wav",
    "assets/cv/B01I84S00.wav", "assets/cv/B01I85S00.wav", "assets/cv/B01I86S00.wav", "assets/cv/B01I87S00.wav",
    "assets/cv/B01I88S00.wav", "assets/cv/B01I89S00.wav", "assets/cv/B01I8AS00.wav", "assets/cv/B01I8BS00.wav",
    "assets/cv/B01I8CS00.wav", "assets/cv/B01I8DS00.wav", "assets/cv/B01I8ES00.wav", "assets/cv/B01I8FS00.wav",
    "assets/cv/B01I90S00.wav", "assets/cv/B01I91S00.wav", "assets/cv/B01I92S00.wav", "assets/cv/B01I93S00.wav",
    "assets/cv/B01I94S00.wav", "assets/cv/B01I95S00.wav", "assets/cv/B01I96S00.wav", "assets/cv/B01I97S00.wav",
    "assets/cv/B01I98S00.wav", "assets/cv/B01I99S00.wav", "assets/cv/B01I9AS00.wav", "assets/cv/B01I9BS00.wav",
    "assets/cv/B01I9CS00.wav", "assets/cv/B01I9DS00.wav", "assets/cv/B01I9ES00.wav", "assets/cv/B01I9FS00.wav",
    "assets/cv/B01IA0S00.wav", "assets/cv/B01IA1S00.wav", "assets/cv/B01IA2S00.wav", "assets/cv/B01IA3S00.wav",
    "assets/cv/B01IA4S00.wav", "assets/cv/B01IA5S00.wav", "assets/cv/B01IA6S00.wav", "assets/cv/B01IA7S00.wav",
    "assets/cv/B01IA8S00.wav", "assets/cv/B01IA9S00.wav", "assets/cv/B01IAAS00.wav",
};

const char* s_sample_file_paths_cv_bank_2[] = {
    "assets/cv/B02I00S00.wav", "assets/cv/B02I01S00.wav", "assets/cv/B02I02S00.wav", "assets/cv/B02I03S00.wav",
    "assets/cv/B02I04S00.wav", "assets/cv/B02I05S00.wav", "assets/cv/B02I06S00.wav", "assets/cv/B02I07S00.wav",
    "assets/cv/B02I08S00.wav", "assets/cv/B02I09S00.wav", "assets/cv/B02I0AS00.wav", "assets/cv/B02I0BS00.wav",
    "assets/cv/B02I0CS00.wav", "assets/cv/B02I0DS00.wav", "assets/cv/B02I0ES00.wav", "assets/cv/B02I0FS00.wav",
    "assets/cv/B02I10S00.wav", "assets/cv/B02I11S00.wav", "assets/cv/B02I12S00.wav", "assets/cv/B02I13S00.wav",
    "assets/cv/B02I14S00.wav", "assets/cv/B02I15S00.wav", "assets/cv/B02I16S00.wav", "assets/cv/B02I17S00.wav",
    "assets/cv/B02I18S00.wav", "assets/cv/B02I19S00.wav", "assets/cv/B02I1AS00.wav", "assets/cv/B02I1BS00.wav",
    "assets/cv/B02I1CS00.wav", "assets/cv/B02I1DS00.wav", "assets/cv/B02I1ES00.wav", "assets/cv/B02I1FS00.wav",
    "assets/cv/B02I20S00.wav", "assets/cv/B02I21S00.wav", "assets/cv/B02I22S00.wav", "assets/cv/B02I23S00.wav",
    "assets/cv/B02I24S00.wav", "assets/cv/B02I25S00.wav", "assets/cv/B02I26S00.wav", "assets/cv/B02I27S00.wav",
    "assets/cv/B02I28S00.wav", "assets/cv/B02I29S00.wav", "assets/cv/B02I2AS00.wav", "assets/cv/B02I2BS00.wav",
    "assets/cv/B02I2CS00.wav", "assets/cv/B02I2DS00.wav", "assets/cv/B02I2ES00.wav", "assets/cv/B02I2FS00.wav",
    "assets/cv/B02I30S00.wav", "assets/cv/B02I31S00.wav", "assets/cv/B02I32S00.wav", "assets/cv/B02I33S00.wav",
    "assets/cv/B02I34S00.wav", "assets/cv/B02I35S00.wav", "assets/cv/B02I36S00.wav", "assets/cv/B02I37S00.wav",
    "assets/cv/B02I38S00.wav", "assets/cv/B02I39S00.wav", "assets/cv/B02I3AS00.wav", "assets/cv/B02I3BS00.wav"
};

const char** s_sample_file_paths_cv_banks[] = {
    s_sample_file_paths_cv_bank_0,
    s_sample_file_paths_cv_bank_1,
    s_sample_file_paths_cv_bank_2,
};

const char* s_sample_file_paths_gms_bank_0[] = {
    "assets/gms/B00I00S00.wav", "assets/gms/B00I01S00.wav", "assets/gms/B00I02S00.wav", "assets/gms/B00I03S00.wav",
    "assets/gms/B00I04S00.wav", "assets/gms/B00I05S00.wav", "assets/gms/B00I06S00.wav", "assets/gms/B00I07S00.wav",
    "assets/gms/B00I08S00.wav", "assets/gms/B00I09S00.wav", "assets/gms/B00I0AS00.wav", "assets/gms/B00I0BS00.wav",
    "assets/gms/B00I0CS00.wav", "assets/gms/B00I0DS00.wav", "assets/gms/B00I0ES00.wav", "assets/gms/B00I0FS00.wav",
    "assets/gms/B00I10S00.wav", "assets/gms/B00I11S00.wav", "assets/gms/B00I12S00.wav", "assets/gms/B00I13S00.wav",
    "assets/gms/B00I14S00.wav", "assets/gms/B00I15S00.wav", "assets/gms/B00I16S00.wav", "assets/gms/B00I17S00.wav",
    "assets/gms/B00I18S00.wav", "assets/gms/B00I19S00.wav", "assets/gms/B00I1AS00.wav", "assets/gms/B00I1BS00.wav",
    "assets/gms/B00I1CS00.wav", "assets/gms/B00I1DS00.wav", "assets/gms/B00I1ES00.wav", "assets/gms/B00I1FS00.wav",
    "assets/gms/B00I20S00.wav", "assets/gms/B00I21S00.wav", "assets/gms/B00I22S00.wav", "assets/gms/B00I23S00.wav",
    "assets/gms/B00I24S00.wav", "assets/gms/B00I25S00.wav", "assets/gms/B00I26S00.wav", "assets/gms/B00I27S00.wav",
    "assets/gms/B00I28S00.wav", "assets/gms/B00I29S00.wav", "assets/gms/B00I2AS00.wav", "assets/gms/B00I2BS00.wav",
    "assets/gms/B00I2CS00.wav", "assets/gms/B00I2DS00.wav", "assets/gms/B00I2ES00.wav", "assets/gms/B00I2FS00.wav",
    "assets/gms/B00I30S00.wav", "assets/gms/B00I31S00.wav", "assets/gms/B00I32S00.wav", "assets/gms/B00I33S00.wav",
    "assets/gms/B00I34S00.wav", "assets/gms/B00I35S00.wav", "assets/gms/B00I36S00.wav", "assets/gms/B00I37S00.wav",
    "assets/gms/B00I38S00.wav", "assets/gms/B00I39S00.wav", "assets/gms/B00I3AS00.wav", "assets/gms/B00I3BS00.wav",
    "assets/gms/B00I3CS00.wav", "assets/gms/B00I3DS00.wav", "assets/gms/B00I3ES00.wav", "assets/gms/B00I3FS00.wav",
    "assets/gms/B00I40S00.wav", "assets/gms/B00I41S00.wav", "assets/gms/B00I42S00.wav", "assets/gms/B00I43S00.wav",
    "assets/gms/B00I44S00.wav", "assets/gms/B00I45S00.wav", "assets/gms/B00I46S00.wav", "assets/gms/B00I47S00.wav",
    "assets/gms/B00I48S00.wav", "assets/gms/B00I49S00.wav", "assets/gms/B00I4AS00.wav", "assets/gms/B00I4BS00.wav",
    "assets/gms/B00I4CS00.wav", "assets/gms/B00I4DS00.wav", "assets/gms/B00I4ES00.wav", "assets/gms/B00I4FS00.wav",
    "assets/gms/B00I50S00.wav", "assets/gms/B00I51S00.wav", "assets/gms/B00I52S00.wav", "assets/gms/B00I53S00.wav",
    "assets/gms/B00I54S00.wav", "assets/gms/B00I55S00.wav", "assets/gms/B00I56S00.wav", "assets/gms/B00I57S00.wav",
    "assets/gms/B00I58S00.wav", "assets/gms/B00I59S00.wav", "assets/gms/B00I5AS00.wav", "assets/gms/B00I5BS00.wav",
    "assets/gms/B00I5CS00.wav", "assets/gms/B00I5DS00.wav", "assets/gms/B00I5ES00.wav", "assets/gms/B00I5FS00.wav",
    "assets/gms/B00I60S00.wav", "assets/gms/B00I61S00.wav", "assets/gms/B00I62S00.wav", "assets/gms/B00I63S00.wav",
    "assets/gms/B00I64S00.wav", "assets/gms/B00I65S00.wav", "assets/gms/B00I66S00.wav", "assets/gms/B00I67S00.wav",
    "assets/gms/B00I68S00.wav", "assets/gms/B00I69S00.wav", "assets/gms/B00I6AS00.wav", "assets/gms/B00I6BS00.wav",
    "assets/gms/B00I6CS00.wav", "assets/gms/B00I6DS00.wav", "assets/gms/B00I6ES00.wav", "assets/gms/B00I6FS00.wav",
    "assets/gms/B00I70S00.wav", "assets/gms/B00I71S00.wav", "assets/gms/B00I72S00.wav", "assets/gms/B00I73S00.wav",
    "assets/gms/B00I74S00.wav", "assets/gms/B00I75S00.wav", "assets/gms/B00I76S00.wav", "assets/gms/B00I77S00.wav",
    "assets/gms/B00I78S00.wav", "assets/gms/B00I79S00.wav", "assets/gms/B00I7AS00.wav", "assets/gms/B00I7BS00.wav",
    "assets/gms/B00I7CS00.wav", "assets/gms/B00I7DS00.wav", "assets/gms/B00I7ES00.wav", "assets/gms/B00I7FS00.wav",
    "assets/gms/B00I80S00.wav", "assets/gms/B00I81S00.wav", "assets/gms/B00I82S00.wav", "assets/gms/B00I83S00.wav",
    "assets/gms/B00I84S00.wav", "assets/gms/B00I85S00.wav", "assets/gms/B00I86S00.wav", "assets/gms/B00I87S00.wav",
    "assets/gms/B00I88S00.wav", "assets/gms/B00I89S00.wav", "assets/gms/B00I8AS00.wav", "assets/gms/B00I8BS00.wav",
    "assets/gms/B00I8CS00.wav", "assets/gms/B00I8DS00.wav", "assets/gms/B00I8ES00.wav", "assets/gms/B00I8FS00.wav",
    "assets/gms/B00I90S00.wav", "assets/gms/B00I91S00.wav", "assets/gms/B00I92S00.wav", "assets/gms/B00I93S00.wav",
    "assets/gms/B00I94S00.wav", "assets/gms/B00I95S00.wav", "assets/gms/B00I96S00.wav", "assets/gms/B00I97S00.wav",
    "assets/gms/B00I98S00.wav", "assets/gms/B00I99S00.wav", "assets/gms/B00I9AS00.wav", "assets/gms/B00I9BS00.wav",
    "assets/gms/B00I9CS00.wav", "assets/gms/B00I9DS00.wav", "assets/gms/B00I9ES00.wav", "assets/gms/B00I9FS00.wav",
    "assets/gms/B00IA0S00.wav", "assets/gms/B00IA1S00.wav", "assets/gms/B00IA2S00.wav", "assets/gms/B00IA3S00.wav",
    "assets/gms/B00IA4S00.wav", "assets/gms/B00IA5S00.wav", "assets/gms/B00IA6S00.wav", "assets/gms/B00IA7S00.wav",
    "assets/gms/B00IA8S00.wav", "assets/gms/B00IA9S00.wav", "assets/gms/B00IAAS00.wav", "assets/gms/B00IABS00.wav",
    "assets/gms/B00IACS00.wav", "assets/gms/B00IADS00.wav", "assets/gms/B00IAES00.wav", "assets/gms/B00IAFS00.wav",
    "assets/gms/B00IB0S00.wav", "assets/gms/B00IB1S00.wav", "assets/gms/B00IB2S00.wav"
};

const char* s_sample_file_paths_gms_bank_1[] = {
    "assets/gms/B01I00S00.wav", "assets/gms/B01I01S00.wav", "assets/gms/B01I02S00.wav", "assets/gms/B01I03S00.wav",
    "assets/gms/B01I04S00.wav", "assets/gms/B01I05S00.wav", "assets/gms/B01I06S00.wav", "assets/gms/B01I07S00.wav",
    "assets/gms/B01I08S00.wav", "assets/gms/B01I09S00.wav", "assets/gms/B01I0AS00.wav", "assets/gms/B01I0BS00.wav",
    "assets/gms/B01I0CS00.wav", "assets/gms/B01I0DS00.wav", "assets/gms/B01I0ES00.wav", "assets/gms/B01I0FS00.wav",
    "assets/gms/B01I10S00.wav", "assets/gms/B01I11S00.wav", "assets/gms/B01I12S00.wav", "assets/gms/B01I13S00.wav",
    "assets/gms/B01I14S00.wav", "assets/gms/B01I15S00.wav", "assets/gms/B01I16S00.wav", "assets/gms/B01I17S00.wav",
    "assets/gms/B01I18S00.wav", "assets/gms/B01I19S00.wav", "assets/gms/B01I1AS00.wav", "assets/gms/B01I1BS00.wav",
    "assets/gms/B01I1CS00.wav", "assets/gms/B01I1DS00.wav", "assets/gms/B01I1ES00.wav", "assets/gms/B01I1FS00.wav",
    "assets/gms/B01I20S00.wav", "assets/gms/B01I21S00.wav", "assets/gms/B01I22S00.wav", "assets/gms/B01I23S00.wav",
    "assets/gms/B01I24S00.wav", "assets/gms/B01I25S00.wav", "assets/gms/B01I26S00.wav", "assets/gms/B01I27S00.wav",
    "assets/gms/B01I28S00.wav", "assets/gms/B01I29S00.wav", "assets/gms/B01I2AS00.wav", "assets/gms/B01I2BS00.wav",
    "assets/gms/B01I2CS00.wav", "assets/gms/B01I2DS00.wav", "assets/gms/B01I2ES00.wav", "assets/gms/B01I2FS00.wav",
    "assets/gms/B01I30S00.wav", "assets/gms/B01I31S00.wav", "assets/gms/B01I32S00.wav", "assets/gms/B01I33S00.wav",
    "assets/gms/B01I34S00.wav", "assets/gms/B01I35S00.wav", "assets/gms/B01I36S00.wav", "assets/gms/B01I37S00.wav",
    "assets/gms/B01I38S00.wav", "assets/gms/B01I39S00.wav", "assets/gms/B01I3AS00.wav", "assets/gms/B01I3BS00.wav",
    "assets/gms/B01I3CS00.wav", "assets/gms/B01I3DS00.wav", "assets/gms/B01I3ES00.wav", "assets/gms/B01I3FS00.wav",
    "assets/gms/B01I40S00.wav", "assets/gms/B01I41S00.wav", "assets/gms/B01I42S00.wav", "assets/gms/B01I43S00.wav",
    "assets/gms/B01I44S00.wav", "assets/gms/B01I45S00.wav", "assets/gms/B01I46S00.wav", "assets/gms/B01I47S00.wav",
    "assets/gms/B01I48S00.wav", "assets/gms/B01I49S00.wav", "assets/gms/B01I4AS00.wav", "assets/gms/B01I4BS00.wav",
    "assets/gms/B01I4CS00.wav", "assets/gms/B01I4DS00.wav", "assets/gms/B01I4ES00.wav"
};

const char* s_sample_file_paths_gms_bank_2[] = {
    "assets/gms/B02I00S00.wav", "assets/gms/B02I01S00.wav", "assets/gms/B02I02S00.wav", "assets/gms/B02I03S00.wav",
    "assets/gms/B02I04S00.wav", "assets/gms/B02I05S00.wav", "assets/gms/B02I06S00.wav", "assets/gms/B02I07S00.wav",
    "assets/gms/B02I08S00.wav", "assets/gms/B02I09S00.wav", "assets/gms/B02I0AS00.wav", "assets/gms/B02I0BS00.wav",
    "assets/gms/B02I0CS00.wav", "assets/gms/B02I0DS00.wav", "assets/gms/B02I0ES00.wav", "assets/gms/B02I0FS00.wav",
    "assets/gms/B02I10S00.wav", "assets/gms/B02I11S00.wav", "assets/gms/B02I12S00.wav", "assets/gms/B02I13S00.wav",
    "assets/gms/B02I14S00.wav", "assets/gms/B02I15S00.wav", "assets/gms/B02I16S00.wav", "assets/gms/B02I17S00.wav",
    "assets/gms/B02I18S00.wav", "assets/gms/B02I19S00.wav", "assets/gms/B02I1AS00.wav", "assets/gms/B02I1BS00.wav",
    "assets/gms/B02I1CS00.wav", "assets/gms/B02I1DS00.wav", "assets/gms/B02I1ES00.wav", "assets/gms/B02I1FS00.wav",
    "assets/gms/B02I20S00.wav", "assets/gms/B02I21S00.wav", "assets/gms/B02I22S00.wav", "assets/gms/B02I23S00.wav",
    "assets/gms/B02I24S00.wav", "assets/gms/B02I25S00.wav", "assets/gms/B02I26S00.wav", "assets/gms/B02I27S00.wav",
    "assets/gms/B02I28S00.wav", "assets/gms/B02I29S00.wav", "assets/gms/B02I2AS00.wav", "assets/gms/B02I2BS00.wav",
    "assets/gms/B02I2CS00.wav", "assets/gms/B02I2DS00.wav", "assets/gms/B02I2ES00.wav", "assets/gms/B02I2FS00.wav",
    "assets/gms/B02I30S00.wav", "assets/gms/B02I31S00.wav", "assets/gms/B02I32S00.wav", "assets/gms/B02I33S00.wav",
    "assets/gms/B02I34S00.wav", "assets/gms/B02I35S00.wav", "assets/gms/B02I36S00.wav", "assets/gms/B02I37S00.wav",
    "assets/gms/B02I38S00.wav", "assets/gms/B02I39S00.wav", "assets/gms/B02I3AS00.wav", "assets/gms/B02I3BS00.wav",
    "assets/gms/B02I3CS00.wav", "assets/gms/B02I3DS00.wav", "assets/gms/B02I3ES00.wav", "assets/gms/B02I3FS00.wav",
    "assets/gms/B02I40S00.wav", "assets/gms/B02I41S00.wav", "assets/gms/B02I42S00.wav", "assets/gms/B02I43S00.wav",
    "assets/gms/B02I44S00.wav", "assets/gms/B02I45S00.wav", "assets/gms/B02I46S00.wav", "assets/gms/B02I47S00.wav",
    "assets/gms/B02I48S00.wav", "assets/gms/B02I49S00.wav", "assets/gms/B02I4AS00.wav", "assets/gms/B02I4BS00.wav",
    "assets/gms/B02I4CS00.wav", "assets/gms/B02I4DS00.wav", "assets/gms/B02I4ES00.wav", "assets/gms/B02I4FS00.wav",
    "assets/gms/B02I50S00.wav", "assets/gms/B02I51S00.wav", "assets/gms/B02I52S00.wav", "assets/gms/B02I53S00.wav",
    "assets/gms/B02I54S00.wav", "assets/gms/B02I55S00.wav", "assets/gms/B02I56S00.wav", "assets/gms/B02I57S00.wav",
    "assets/gms/B02I58S00.wav", "assets/gms/B02I59S00.wav", "assets/gms/B02I5AS00.wav", "assets/gms/B02I5BS00.wav",
    "assets/gms/B02I5CS00.wav", "assets/gms/B02I5DS00.wav", "assets/gms/B02I5ES00.wav", "assets/gms/B02I5FS00.wav",
    "assets/gms/B02I60S00.wav", "assets/gms/B02I61S00.wav", "assets/gms/B02I62S00.wav", "assets/gms/B02I63S00.wav",
    "assets/gms/B02I64S00.wav", "assets/gms/B02I65S00.wav", "assets/gms/B02I66S00.wav", "assets/gms/B02I67S00.wav",
    "assets/gms/B02I68S00.wav", "assets/gms/B02I69S00.wav", "assets/gms/B02I6AS00.wav", "assets/gms/B02I6BS00.wav",
    "assets/gms/B02I6CS00.wav", "assets/gms/B02I6DS00.wav", "assets/gms/B02I6ES00.wav", "assets/gms/B02I6FS00.wav",
    "assets/gms/B02I70S00.wav", "assets/gms/B02I71S00.wav", "assets/gms/B02I72S00.wav", "assets/gms/B02I73S00.wav",
    "assets/gms/B02I74S00.wav", "assets/gms/B02I75S00.wav", "assets/gms/B02I76S00.wav", "assets/gms/B02I77S00.wav",
    "assets/gms/B02I78S00.wav", "assets/gms/B02I79S00.wav", "assets/gms/B02I7AS00.wav", "assets/gms/B02I7BS00.wav",
    "assets/gms/B02I7CS00.wav", "assets/gms/B02I7DS00.wav", "assets/gms/B02I7ES00.wav", "assets/gms/B02I7FS00.wav",
    "assets/gms/B02I80S00.wav", "assets/gms/B02I81S00.wav", "assets/gms/B02I82S00.wav", "assets/gms/B02I83S00.wav",
    "assets/gms/B02I84S00.wav", "assets/gms/B02I85S00.wav", "assets/gms/B02I86S00.wav", "assets/gms/B02I87S00.wav",
    "assets/gms/B02I88S00.wav", "assets/gms/B02I89S00.wav", "assets/gms/B02I8AS00.wav", "assets/gms/B02I8BS00.wav",
    "assets/gms/B02I8CS00.wav", "assets/gms/B02I8DS00.wav", "assets/gms/B02I8ES00.wav", "assets/gms/B02I8FS00.wav",
    "assets/gms/B02I90S00.wav", "assets/gms/B02I91S00.wav", "assets/gms/B02I92S00.wav", "assets/gms/B02I93S00.wav",
    "assets/gms/B02I94S00.wav", "assets/gms/B02I95S00.wav", "assets/gms/B02I96S00.wav", "assets/gms/B02I97S00.wav",
    "assets/gms/B02I98S00.wav", "assets/gms/B02I99S00.wav", "assets/gms/B02I9AS00.wav", "assets/gms/B02I9BS00.wav",
    "assets/gms/B02I9CS00.wav", "assets/gms/B02I9DS00.wav", "assets/gms/B02I9ES00.wav", "assets/gms/B02I9FS00.wav",
    "assets/gms/B02IA0S00.wav", "assets/gms/B02IA1S00.wav", "assets/gms/B02IA2S00.wav", "assets/gms/B02IA3S00.wav",
    "assets/gms/B02IA4S00.wav", "assets/gms/B02IA5S00.wav", "assets/gms/B02IA6S00.wav", "assets/gms/B02IA7S00.wav",
    "assets/gms/B02IA8S00.wav", "assets/gms/B02IA9S00.wav", "assets/gms/B02IAAS00.wav", "assets/gms/B02IABS00.wav",
    "assets/gms/B02IACS00.wav", "assets/gms/B02IADS00.wav", "assets/gms/B02IAES00.wav", "assets/gms/B02IAFS00.wav",
    "assets/gms/B02IB0S00.wav", "assets/gms/B02IB1S00.wav", "assets/gms/B02IB2S00.wav", "assets/gms/B02IB3S00.wav",
    "assets/gms/B02IB4S00.wav", "assets/gms/B02IB5S00.wav", "assets/gms/B02IB6S00.wav", "assets/gms/B02IB7S00.wav",
    "assets/gms/B02IB8S00.wav", "assets/gms/B02IB9S00.wav", "assets/gms/B02IBAS00.wav", "assets/gms/B02IBBS00.wav",
    "assets/gms/B02IBCS00.wav", "assets/gms/B02IBDS00.wav", "assets/gms/B02IBES00.wav", "assets/gms/B02IBFS00.wav",
    "assets/gms/B02IC0S00.wav", "assets/gms/B02IC1S00.wav", "assets/gms/B02IC2S00.wav", "assets/gms/B02IC3S00.wav",
    "assets/gms/B02IC4S00.wav", "assets/gms/B02IC5S00.wav", "assets/gms/B02IC6S00.wav", "assets/gms/B02IC7S00.wav",
    "assets/gms/B02IC8S00.wav", "assets/gms/B02IC9S00.wav", "assets/gms/B02ICAS00.wav", "assets/gms/B02ICBS00.wav",
    "assets/gms/B02ICCS00.wav", "assets/gms/B02ICDS00.wav", "assets/gms/B02ICES00.wav", "assets/gms/B02ICFS00.wav",
    "assets/gms/B02ID0S00.wav", "assets/gms/B02ID1S00.wav", "assets/gms/B02ID2S00.wav", "assets/gms/B02ID3S00.wav",
    "assets/gms/B02ID4S00.wav", "assets/gms/B02ID5S00.wav", "assets/gms/B02ID6S00.wav", "assets/gms/B02ID7S00.wav",
    "assets/gms/B02ID8S00.wav", "assets/gms/B02ID9S00.wav", "assets/gms/B02IDAS00.wav", "assets/gms/B02IDBS00.wav",
    "assets/gms/B02IDCS00.wav", "assets/gms/B02IDDS00.wav", "assets/gms/B02IDES00.wav", "assets/gms/B02IDFS00.wav",
    "assets/gms/B02IE0S00.wav", "assets/gms/B02IE1S00.wav", "assets/gms/B02IE2S00.wav", "assets/gms/B02IE3S00.wav",
    "assets/gms/B02IE4S00.wav", "assets/gms/B02IE5S00.wav", "assets/gms/B02IE6S00.wav", "assets/gms/B02IE7S00.wav",
    "assets/gms/B02IE8S00.wav", "assets/gms/B02IE9S00.wav", "assets/gms/B02IEAS00.wav", "assets/gms/B02IEBS00.wav",
    "assets/gms/B02IECS00.wav", "assets/gms/B02IEDS00.wav", "assets/gms/B02IEES00.wav", "assets/gms/B02IEFS00.wav",
    "assets/gms/B02IF0S00.wav", "assets/gms/B02IF1S00.wav", "assets/gms/B02IF2S00.wav"
};

const char** s_sample_file_paths_gms_banks[] = {
    s_sample_file_paths_gms_bank_0,
    s_sample_file_paths_gms_bank_1,
    s_sample_file_paths_gms_bank_2,
};

const char* s_sample_file_paths_mm_bank_0[] = {
    "assets/mm/B00I00S00.wav", "assets/mm/B00I01S00.wav", "assets/mm/B00I02S00.wav", "assets/mm/B00I03S00.wav",
    "assets/mm/B00I04S00.wav", "assets/mm/B00I05S00.wav", "assets/mm/B00I06S00.wav", "assets/mm/B00I07S00.wav",
    "assets/mm/B00I08S00.wav", "assets/mm/B00I09S00.wav", "assets/mm/B00I0AS00.wav", "assets/mm/B00I0BS00.wav",
    "assets/mm/B00I0CS00.wav", "assets/mm/B00I0DS00.wav", "assets/mm/B00I0ES00.wav", "assets/mm/B00I0FS00.wav",
    "assets/mm/B00I10S00.wav", "assets/mm/B00I11S00.wav", "assets/mm/B00I12S00.wav", "assets/mm/B00I13S00.wav",
    "assets/mm/B00I14S00.wav", "assets/mm/B00I15S00.wav", "assets/mm/B00I16S00.wav", "assets/mm/B00I17S00.wav",
    "assets/mm/B00I18S00.wav", "assets/mm/B00I19S00.wav", "assets/mm/B00I1AS00.wav", "assets/mm/B00I1BS00.wav",
    "assets/mm/B00I1CS00.wav", "assets/mm/B00I1DS00.wav", "assets/mm/B00I1ES00.wav", "assets/mm/B00I1FS00.wav",
    "assets/mm/B00I20S00.wav", "assets/mm/B00I21S00.wav", "assets/mm/B00I22S00.wav", "assets/mm/B00I23S00.wav",
    "assets/mm/B00I24S00.wav", "assets/mm/B00I25S00.wav", "assets/mm/B00I26S00.wav", "assets/mm/B00I27S00.wav",
    "assets/mm/B00I28S00.wav", "assets/mm/B00I29S00.wav", "assets/mm/B00I2AS00.wav", "assets/mm/B00I2BS00.wav",
    "assets/mm/B00I2CS00.wav", "assets/mm/B00I2DS00.wav", "assets/mm/B00I2ES00.wav", "assets/mm/B00I2FS00.wav",
    "assets/mm/B00I30S00.wav", "assets/mm/B00I31S00.wav", "assets/mm/B00I32S00.wav", "assets/mm/B00I33S00.wav",
    "assets/mm/B00I34S00.wav", "assets/mm/B00I35S00.wav", "assets/mm/B00I36S00.wav", "assets/mm/B00I37S00.wav",
    "assets/mm/B00I38S00.wav", "assets/mm/B00I39S00.wav", "assets/mm/B00I3AS00.wav", "assets/mm/B00I3BS00.wav",
    "assets/mm/B00I3CS00.wav", "assets/mm/B00I3DS00.wav", "assets/mm/B00I3ES00.wav", "assets/mm/B00I3FS00.wav",
    "assets/mm/B00I40S00.wav", "assets/mm/B00I41S00.wav", "assets/mm/B00I42S00.wav", "assets/mm/B00I43S00.wav",
    "assets/mm/B00I44S00.wav", "assets/mm/B00I45S00.wav", "assets/mm/B00I46S00.wav", "assets/mm/B00I47S00.wav",
    "assets/mm/B00I48S00.wav", "assets/mm/B00I49S00.wav", "assets/mm/B00I4AS00.wav", "assets/mm/B00I4BS00.wav",
    "assets/mm/B00I4CS00.wav", "assets/mm/B00I4DS00.wav"
};

const char* s_sample_file_paths_mm_bank_1[] = {
    "assets/mm/B01I00S00.wav", "assets/mm/B01I01S00.wav", "assets/mm/B01I02S00.wav", "assets/mm/B01I03S00.wav",
    "assets/mm/B01I04S00.wav", "assets/mm/B01I05S00.wav", "assets/mm/B01I06S00.wav", "assets/mm/B01I07S00.wav",
    "assets/mm/B01I08S00.wav", "assets/mm/B01I09S00.wav", "assets/mm/B01I0AS00.wav", "assets/mm/B01I0BS00.wav",
    "assets/mm/B01I0CS00.wav", "assets/mm/B01I0DS00.wav", "assets/mm/B01I0ES00.wav", "assets/mm/B01I0FS00.wav",
    "assets/mm/B01I10S00.wav", "assets/mm/B01I11S00.wav", "assets/mm/B01I12S00.wav", "assets/mm/B01I13S00.wav",
    "assets/mm/B01I14S00.wav", "assets/mm/B01I15S00.wav", "assets/mm/B01I16S00.wav", "assets/mm/B01I17S00.wav",
    "assets/mm/B01I18S00.wav", "assets/mm/B01I19S00.wav", "assets/mm/B01I1AS00.wav", "assets/mm/B01I1BS00.wav",
    "assets/mm/B01I1CS00.wav", "assets/mm/B01I1DS00.wav", "assets/mm/B01I1ES00.wav", "assets/mm/B01I1FS00.wav"
};

const char* s_sample_file_paths_mm_bank_2[] = {
    "assets/mm/B02I00S00.wav", "assets/mm/B02I01S00.wav", "assets/mm/B02I02S00.wav", "assets/mm/B02I03S00.wav",
    "assets/mm/B02I04S00.wav", "assets/mm/B02I05S00.wav", "assets/mm/B02I06S00.wav", "assets/mm/B02I07S00.wav",
    "assets/mm/B02I08S00.wav", "assets/mm/B02I09S00.wav", "assets/mm/B02I0AS00.wav", "assets/mm/B02I0BS00.wav",
    "assets/mm/B02I0CS00.wav", "assets/mm/B02I0DS00.wav", "assets/mm/B02I0ES00.wav", "assets/mm/B02I0FS00.wav",
    "assets/mm/B02I10S00.wav", "assets/mm/B02I11S00.wav", "assets/mm/B02I12S00.wav", "assets/mm/B02I13S00.wav",
    "assets/mm/B02I14S00.wav", "assets/mm/B02I15S00.wav", "assets/mm/B02I16S00.wav", "assets/mm/B02I17S00.wav",
    "assets/mm/B02I18S00.wav", "assets/mm/B02I19S00.wav", "assets/mm/B02I1AS00.wav", "assets/mm/B02I1BS00.wav",
    "assets/mm/B02I1CS00.wav", "assets/mm/B02I1DS00.wav", "assets/mm/B02I1ES00.wav", "assets/mm/B02I1FS00.wav",
    "assets/mm/B02I20S00.wav", "assets/mm/B02I21S00.wav", "assets/mm/B02I22S00.wav", "assets/mm/B02I23S00.wav",
    "assets/mm/B02I24S00.wav", "assets/mm/B02I25S00.wav", "assets/mm/B02I26S00.wav", "assets/mm/B02I27S00.wav",
    "assets/mm/B02I28S00.wav", "assets/mm/B02I29S00.wav", "assets/mm/B02I2AS00.wav", "assets/mm/B02I2BS00.wav",
    "assets/mm/B02I2CS00.wav", "assets/mm/B02I2DS00.wav", "assets/mm/B02I2ES00.wav", "assets/mm/B02I2FS00.wav",
    "assets/mm/B02I30S00.wav", "assets/mm/B02I31S00.wav", "assets/mm/B02I32S00.wav", "assets/mm/B02I33S00.wav",
    "assets/mm/B02I34S00.wav", "assets/mm/B02I35S00.wav", "assets/mm/B02I36S00.wav", "assets/mm/B02I37S00.wav",
    "assets/mm/B02I38S00.wav", "assets/mm/B02I39S00.wav", "assets/mm/B02I3AS00.wav", "assets/mm/B02I3BS00.wav",
    "assets/mm/B02I3CS00.wav", "assets/mm/B02I3DS00.wav", "assets/mm/B02I3ES00.wav", "assets/mm/B02I3FS00.wav",
    "assets/mm/B02I40S00.wav", "assets/mm/B02I41S00.wav", "assets/mm/B02I42S00.wav", "assets/mm/B02I43S00.wav",
    "assets/mm/B02I44S00.wav", "assets/mm/B02I45S00.wav", "assets/mm/B02I46S00.wav", "assets/mm/B02I47S00.wav",
    "assets/mm/B02I48S00.wav", "assets/mm/B02I49S00.wav", "assets/mm/B02I4AS00.wav", "assets/mm/B02I4BS00.wav",
    "assets/mm/B02I4CS00.wav", "assets/mm/B02I4DS00.wav", "assets/mm/B02I4ES00.wav", "assets/mm/B02I4FS00.wav",
    "assets/mm/B02I50S00.wav", "assets/mm/B02I51S00.wav", "assets/mm/B02I52S00.wav", "assets/mm/B02I53S00.wav",
    "assets/mm/B02I54S00.wav", "assets/mm/B02I55S00.wav", "assets/mm/B02I56S00.wav", "assets/mm/B02I57S00.wav",
    "assets/mm/B02I58S00.wav", "assets/mm/B02I59S00.wav", "assets/mm/B02I5AS00.wav", "assets/mm/B02I5BS00.wav",
    "assets/mm/B02I5CS00.wav", "assets/mm/B02I5DS00.wav", "assets/mm/B02I5ES00.wav", "assets/mm/B02I5FS00.wav",
    "assets/mm/B02I60S00.wav", "assets/mm/B02I61S00.wav", "assets/mm/B02I62S00.wav", "assets/mm/B02I63S00.wav",
    "assets/mm/B02I64S00.wav", "assets/mm/B02I65S00.wav", "assets/mm/B02I66S00.wav", "assets/mm/B02I67S00.wav",
    "assets/mm/B02I68S00.wav", "assets/mm/B02I69S00.wav", "assets/mm/B02I6AS00.wav", "assets/mm/B02I6BS00.wav",
    "assets/mm/B02I6CS00.wav", "assets/mm/B02I6DS00.wav", "assets/mm/B02I6ES00.wav", "assets/mm/B02I6FS00.wav",
    "assets/mm/B02I70S00.wav", "assets/mm/B02I71S00.wav", "assets/mm/B02I72S00.wav", "assets/mm/B02I73S00.wav",
    "assets/mm/B02I74S00.wav", "assets/mm/B02I75S00.wav", "assets/mm/B02I76S00.wav", "assets/mm/B02I77S00.wav",
    "assets/mm/B02I78S00.wav", "assets/mm/B02I79S00.wav", "assets/mm/B02I7AS00.wav", "assets/mm/B02I7BS00.wav",
    "assets/mm/B02I7CS00.wav", "assets/mm/B02I7DS00.wav", "assets/mm/B02I7ES00.wav", "assets/mm/B02I7FS00.wav",
    "assets/mm/B02I80S00.wav", "assets/mm/B02I81S00.wav", "assets/mm/B02I82S00.wav", "assets/mm/B02I83S00.wav",
    "assets/mm/B02I84S00.wav", "assets/mm/B02I85S00.wav", "assets/mm/B02I86S00.wav", "assets/mm/B02I87S00.wav",
    "assets/mm/B02I88S00.wav", "assets/mm/B02I89S00.wav", "assets/mm/B02I8AS00.wav", "assets/mm/B02I8BS00.wav",
    "assets/mm/B02I8CS00.wav", "assets/mm/B02I8DS00.wav", "assets/mm/B02I8ES00.wav", "assets/mm/B02I8FS00.wav",
    "assets/mm/B02I90S00.wav", "assets/mm/B02I91S00.wav", "assets/mm/B02I92S00.wav", "assets/mm/B02I93S00.wav",
    "assets/mm/B02I94S00.wav", "assets/mm/B02I95S00.wav", "assets/mm/B02I96S00.wav", "assets/mm/B02I97S00.wav",
    "assets/mm/B02I98S00.wav", "assets/mm/B02I99S00.wav", "assets/mm/B02I9AS00.wav", "assets/mm/B02I9BS00.wav",
    "assets/mm/B02I9CS00.wav", "assets/mm/B02I9DS00.wav", "assets/mm/B02I9ES00.wav", "assets/mm/B02I9FS00.wav",
    "assets/mm/B02IA0S00.wav", "assets/mm/B02IA1S00.wav", "assets/mm/B02IA2S00.wav", "assets/mm/B02IA3S00.wav",
    "assets/mm/B02IA4S00.wav", "assets/mm/B02IA5S00.wav", "assets/mm/B02IA6S00.wav", "assets/mm/B02IA7S00.wav",
    "assets/mm/B02IA8S00.wav", "assets/mm/B02IA9S00.wav", "assets/mm/B02IAAS00.wav", "assets/mm/B02IABS00.wav",
    "assets/mm/B02IACS00.wav", "assets/mm/B02IADS00.wav", "assets/mm/B02IAES00.wav", "assets/mm/B02IAFS00.wav",
    "assets/mm/B02IB0S00.wav", "assets/mm/B02IB1S00.wav", "assets/mm/B02IB2S00.wav", "assets/mm/B02IB3S00.wav",
    "assets/mm/B02IB4S00.wav", "assets/mm/B02IB5S00.wav", "assets/mm/B02IB6S00.wav", "assets/mm/B02IB7S00.wav",
    "assets/mm/B02IB8S00.wav", "assets/mm/B02IB9S00.wav", "assets/mm/B02IBAS00.wav", "assets/mm/B02IBBS00.wav",
    "assets/mm/B02IBCS00.wav", "assets/mm/B02IBDS00.wav", "assets/mm/B02IBES00.wav", "assets/mm/B02IBFS00.wav",
    "assets/mm/B02IC0S00.wav", "assets/mm/B02IC1S00.wav", "assets/mm/B02IC2S00.wav", "assets/mm/B02IC3S00.wav",
    "assets/mm/B02IC4S00.wav", "assets/mm/B02IC5S00.wav", "assets/mm/B02IC6S00.wav", "assets/mm/B02IC7S00.wav",
    "assets/mm/B02IC8S00.wav", "assets/mm/B02IC9S00.wav", "assets/mm/B02ICAS00.wav", "assets/mm/B02ICBS00.wav",
    "assets/mm/B02ICCS00.wav", "assets/mm/B02ICDS00.wav", "assets/mm/B02ICES00.wav", "assets/mm/B02ICFS00.wav",
    "assets/mm/B02ID0S00.wav", "assets/mm/B02ID1S00.wav"
};

const char** s_sample_file_paths_mm_banks[] = {
    s_sample_file_paths_mm_bank_0,
    s_sample_file_paths_mm_bank_1,
    s_sample_file_paths_mm_bank_2,
};

/* ----- .bss ----- */
u8 g_channel_index;

Channel g_channels[CHANNEL_COUNT];
Channel *g_channel;

u8 *g_sequence_data_needle;

void player_update_channels()
{
    // A bunch of modifier stuff... (Tempo/Fade/Volume)

    for (g_channel_index = 0; g_channel_index < CHANNEL_COUNT; g_channel_index++)
    {
        g_channel = &g_channels[g_channel_index];

        if (g_channels[g_channel_index].voice_status != 0)
        {
            if (--g_channels[g_channel_index].voice_status == 0)
            {
                alSynStopVoice(g_channel_index);
            }
        }

        if (g_channel->sequence_id != 0)
        {
            if (g_channel->initial_delay_counter == 0)
            {
                if (g_channel->sequence_id < 0x100)
                {
                    if (!g_paused)
                    {
                        g_channel->tempo += g_tempo_increase_amount;
                        player_update_channel();
                    }
                }
                else
                {
                    player_update_channel();
                }
            }
            else
            {
                g_channel->initial_delay_counter--;
            }
        }
    }
}

void player_update_channel()
{
    g_channel->tempo_counter = g_channel->tempo_counter + (g_channel->tempo >> 8);

    if (g_channel->tempo_counter < 0x100)
    {
        player_update_off_tick();
    }
    else
    {
        g_channel->tempo_counter &= 0xFF;
        g_channel->note_duration--;

        if (g_channel->note_duration == 0)
        {
            player_read_data();
            effects_set_pitch_bend();
        }
        else
        {
            player_update_on_tick();
        }

        if (g_channel->tempo_change_duration != 0)
        {
            effects_update_tempo_change();
        }

        if (g_channel->volume_change_duration != 0)
        {
            effects_update_volume_change();
        }

        if (g_channel->pan_change_duration != 0)
        {
            effects_update_pan_change();
        }
    }

    if (g_channel->random_rate != 0)
    {
        effects_update_random();
    }
}

extern u8 seq_data[];

/* for debug */
#include <stdio.h>

enum command_flags {
    is_percussion,
    has_percussion_instrument_lookup,
	has_duration,
	has_hold_ratio,
	has_velocity,
};

/* effect command to string lookup table */
const char *effect_command_strings[] =
{
    "effects_set_tempo", // 0xD0
    "effects_set_tempo_change", // 0xD1
    "effects_set_sound", // 0xD2
    "effects_set_volume_sound", // 0xD3
    "effects_set_volume_sound_pan", // 0xD4
    "effects_set_volume", // 0xD5
    "effects_set_volume_change", // 0xD6
    "effects_set_attack_decay", // 0xD7
    "effects_set_sustain", // 0xD8
    "effects_set_release", // 0xD9
    "effects_set_reverb", // 0xDA
    "effects_end", // 0xDB
    "effects_set_user_defined", // 0xDC
    "effects_set_pan", // 0xDD
    "effects_set_pan_change", // 0xDE
    "effects_set_transpose", // 0xDF
    "effects_set_detune", // 0xE0
    "effects_set_vibrato", // 0xE1
    "effects_set_vibrato_change", // 0xE2
    "effects_set_random", // 0xE3
    "effects_set_pitch_bend_dummy", // 0xE4
    "effects_set_pitch_bend_start", // 0xE5
    "effects_set_portamento", // 0xE6
    "effects_set_marker", // 0xE7
    "effects_go_to_marker", // 0xE8
    "effects_set_nested_marker", // 0xE9
    "effects_go_to_nested_marker", // 0xEA
    "effects_set_master_marker", // 0xEB
    "effects_go_to_master_marker", // 0xEC
    "effects_set_conditional_marker", // 0xED
    "effects_go_to_conditional_marker", // 0xEE
    "effects_set_echo", // 0xEF
    "effects_set_bank", // 0xF0
    "effects_set_nothing", // 0xF1
    "effects_rest", // 0xF2
    "effects_tie", // 0xF3
    "effects_end", // 0xF4
    "effects_end", // 0xF5
    "effects_end", // 0xF6
    "effects_end", // 0xF7
    "effects_F8", // 0xF8
    "effects_F9", // 0xF9
    "effects_FA", // 0xFA
    "effects_FB", // 0xFB
    "effects_FC", // 0xFC
    "effects_FD", // 0xFD
    "effects_FE", // 0xFE
    "effects_end", // 0xFF
};

const uint8_t effect_command_lengths[] =
{
    1, // 0xD0: effects_set_tempo
    2, // 0xD1: effects_set_tempo_change
    1, // 0xD2: effects_set_sound
    2, // 0xD3: effects_set_volume_sound
    3, // 0xD4: effects_set_volume_sound_pan
    1, // 0xD5: effects_set_volume
    2, // 0xD6: effects_set_volume_change
    2, // 0xD7: effects_set_attack_decay
    1, // 0xD8: effects_set_sustain
    1, // 0xD9: effects_set_release
    2, // 0xDA: effects_set_reverb
    0, // 0xDB: effects_end
    1, // 0xDC: effects_set_user_defined
    1, // 0xDD: effects_set_pan
    2, // 0xDE: effects_set_pan_change
    1, // 0xDF: effects_set_transpose
    1, // 0xE0: effects_set_detune
    3, // 0xE1: effects_set_vibrato
    1, // 0xE2: effects_set_vibrato_change
    3, // 0xE3: effects_set_random
    3, // 0xE4: effects_set_pitch_bend_dummy
    3, // 0xE5: effects_set_pitch_bend_start
    1, // 0xE6: effects_set_portamento
    0, // 0xE7: effects_set_marker
    3, // 0xE8: effects_go_to_marker
    0, // 0xE9: effects_set_nested_marker
    3, // 0xEA: effects_go_to_nested_marker
    0, // 0xEB: effects_set_master_marker
    0, // 0xEC: effects_go_to_master_marker
    0, // 0xED: effects_set_conditional_marker
    0, // 0xEE: effects_go_to_conditional_marker
    1, // 0xEF: effects_set_echo
    1, // 0xF0: effects_set_bank
    1, // 0xF1: effects_set_nothing
    1, // 0xF2: effects_rest
    2, // 0xF3: effects_tie
    0, // 0xF4: effects_end
    0, // 0xF5: effects_end
    0, // 0xF6: effects_end
    0, // 0xF7: effects_end
    3, // 0xF8: effects_F8
    2, // 0xF9: effects_F9
    1, // 0xFA: effects_FA
    2, // 0xFB: effects_FB
    3, // 0xFC: effects_FC
    3, // 0xFD: effects_FD
    3, // 0xFE: effects_FE
    0, // 0xFF: effects_end
};
/* end debug */

void player_read_data()
{
    u8 note_is_still_playing = FALSE;
    u8 sequence_data;
    u32 command;

    if (g_channel->note_hold_ratio == 0)
    {
        note_is_still_playing = TRUE;
    }
    else if (g_channel->voice_status != 0)
    {
        g_channel->voice_status = 0;

        alSynStopVoice(g_channel_index);
    }

    g_sequence_data_needle = g_channel->sequence_data_needle;
    sequence_data = *g_sequence_data_needle;

    while (TRUE)
    {
        command = sequence_data;
        g_sequence_data_needle++;

        /* printf("Channel %d: Command %X\n", g_channel_index, command); */

        if (command < 0xD0)
        {
            /* for debug */
            /* go back one byte because of previous load and store for the command byte */
			uint8_t* previous_sequence_data_needle = g_sequence_data_needle--;

            uint8_t command_flags = 0;
            uint8_t command_byte = 0;

            uint8_t note_pitch = 0;
            uint8_t percussion_instrument_lookup = 0;
            uint8_t note_duration = 0;
			uint8_t note_hold_ratio = 0;
			uint8_t note_velocity = 0;

            command_byte = *g_sequence_data_needle++;
            if (command_byte < 0x68)
            {
                /* note has a new duration */
                command_flags |= 1 << has_duration;

            	note_pitch = command_byte;
            } else {
				/* note uses previous duration */
            	note_pitch = command_byte - 0x68;
            }

            if (note_pitch < 0x48) {
                /* melodic note */
            } else {
                /* percussion note */
				command_flags |= 1 << is_percussion;

                if (note_pitch == 0x67) {
                    /* percussion instrument lookup */
					command_flags |= 1 << has_percussion_instrument_lookup;

					command_byte = *g_sequence_data_needle++;
                    percussion_instrument_lookup = command_byte;
                }
            }

            if (command_flags & (1 << has_duration)) {
                command_byte = *g_sequence_data_needle++;
                note_duration = command_byte;
			}

            command_byte = *g_sequence_data_needle++;
			if (command_byte < 0x80) {
				/* note has a new hold ratio */
                command_flags |= 1 << has_hold_ratio;

				note_hold_ratio = command_byte;
            }

			command_byte = *g_sequence_data_needle++;
			command_flags |= 1 << has_velocity;
			note_velocity = command_byte & 0x7F;

            printf(KGRN "Track %02d | " KYEL, g_channel_index);

            if (command_flags & (1 << is_percussion)) {
				printf("percussion");

                if (command_flags & (1 << has_percussion_instrument_lookup)) {
                    printf("_instrumentlookup");
				}
            } else {
                printf("key");
            }

            if (command_flags & (1 << has_duration)) {
				printf("_duration");
			}

			if (command_flags & (1 << has_hold_ratio)) {
                printf("_holdratio");
            }

            if (command_flags & (1 << has_velocity)) {
                printf("_velocity");
            }

            printf(KCYN);

			if (command_flags & (1 << has_percussion_instrument_lookup)) {
				printf(" %d", percussion_instrument_lookup);
			} else {
                printf(" %d", note_pitch);
			}

            if (command_flags & (1 << has_duration)) {
				printf(", %d", note_duration);
            }

            if (command_flags & (1 << has_hold_ratio)) {
                printf(", %d", note_hold_ratio);
            }

            if (command_flags & (1 << has_velocity)) {
                printf(", %d", note_velocity);
            }

            printf(KNRM "\n");

			g_sequence_data_needle = previous_sequence_data_needle;
        	/* end debug */

            // Set the previous pitch for the portamento effect.
            g_channel->pitch_change_amount = g_channel->pitch;

            if (command < 0x68)
            {
                g_channel->note_pitch = sequence_data;
            }
            else
            {
                g_channel->note_pitch = sequence_data - 0x68;
            }

            if (g_channel->note_pitch < 0x48)
            {
                g_channel->flags &= ~IS_DRUM;
            }
            else
            {
                if (g_channel->note_pitch == 0x67)
                {
                    g_channel->note_pitch = *g_sequence_data_needle;
                    g_sequence_data_needle++;
                }

                g_channel->flags |= IS_DRUM;

                effects_load_drum_sound();

                g_channel->note_pitch = 0x3C;
            }

            g_channel->pitch = g_channel->note_pitch << 8;
            g_channel->pitch += g_channel->sound_transpose_detune;
            g_channel->pitch += g_channel->effect_transpose;
            g_channel->pitch += g_channel->effect_detune;
            g_channel->pitch += g_channel->marker_transpose;
            g_channel->pitch += g_channel->nested_marker_transpose;

            /* special transpose mnsg -> gga banks */
            g_channel->pitch += g_channel->special_transpose;

            if (command < 0x68)
            {
                g_channel->previous_note_duration = *g_sequence_data_needle;
                g_sequence_data_needle++;
            }

            g_channel->note_duration = g_channel->previous_note_duration;

            sequence_data = *g_sequence_data_needle;
            g_sequence_data_needle++;

            if (sequence_data < 0x80)
            {
                g_channel->previous_note_hold_ratio = sequence_data;

                sequence_data = *g_sequence_data_needle;
                g_sequence_data_needle++;
            }

            g_channel->note_hold_ratio = g_channel->previous_note_hold_ratio;
            if (g_channel->note_hold_ratio == 0)
            {
                g_channel->note_hold_duration = 0;
                g_channel->note_velocity = sequence_data & 0x7F;
            }
            else
            {
                // Casting might have issues here.
                g_channel->note_hold_duration = (g_channel->note_duration * g_channel->note_hold_ratio) / 128;

                if (g_channel->note_hold_duration == 0)
                {
                    g_channel->note_hold_duration = 1;
                }

                g_channel->note_velocity = sequence_data & 0x7F;
            }

            g_channel->sequence_data_needle = g_sequence_data_needle;

            if (g_channel->pitch_change_next_note_parameter == 0)
            {
                g_channel->pitch_change_duration = 0;
            }
            else
            {
                effects_calculate_pitch_change_amount_this_note();
            }

            g_channel->vibrato_pitch = 0;
            if (g_channel->vibrato_depth_target != 0)
            {
                effects_initialize_vibrato();
            }

            if (!g_channel->disable) {
                player_set_pitch();

                if (note_is_still_playing)
                {
                    if (g_channel->envelope_phase_duration == 0)
                    {
                        player_set_volume();
                    }
                    else
                    {
                        player_update_envelope_phase();

                        if (g_channel->envelope_phase_duration != 0)
                        {
                            player_set_volume();
                        }
                    }
                }
                else
                {
                    player_start_envelope_attack_phase();

                    if (g_channel->bank < BANK_COUNT) {
                        alSynStartVoice(g_channel_index, CURRENT_SAMPLE_BANK[g_channel->bank][g_channel->sound]);
                    }
                }
            }

            return;
        }
        else
        {
            g_command_table[command - 0xD0]();

            /* for debug */
            uint8_t* previous_sequence_data_needle = g_sequence_data_needle--;

            printf(KGRN "Track %02d | " KBLU "%s ", g_channel_index, effect_command_strings[command - 0xD0]);

            for (size_t i = 0; i < effect_command_lengths[command - 0xD0]; ++i) {
                printf(KCYN "%d", *g_sequence_data_needle);
                g_sequence_data_needle++;

                if (i != effect_command_lengths[command - 0xD0] - 1) {
                    printf(KNRM ", ");
				}
			}

            printf(KNRM "\n");

            g_sequence_data_needle = previous_sequence_data_needle;
            /* end debug */
        }

        if (command == 0xF2 || command == 0xF3)
        {
            g_channel->sequence_data_needle = g_sequence_data_needle;
            return;
        }

        if (command == 0xFF)
        {
            return;
        }

        sequence_data = *g_sequence_data_needle;
    }
}

void player_update_on_tick()
{
    if (g_channel->pitch_change_duration != 0)
    {
        effects_update_pitch_change_on_tick();
    }

    if (g_channel->vibrato_depth_target != 0)
    {
        effects_update_vibrato_on_tick();
    }

    if ((g_channel->flags & PITCH_IS_CHANGING) != 0)
    {
        player_set_pitch();
    }

    if (g_channel->note_hold_ratio == 0 || g_channel->note_duration != 1)
    {
        if (g_channel->note_hold_duration != 0 && --g_channel->note_hold_duration == 0)
        {
            player_start_envelope_release_phase();
        }

        if (g_channel->envelope_phase_duration != 0)
        {
            player_update_envelope_phase();
        }

        if ((g_channel->flags & VOLUME_IS_CHANGING) == 0)
        {
            player_set_volume(); // is actually player_fdsdfsfsdf
        }
        else
        {
            player_set_volume();
        }
    }
    else
    {
        player_start_envelope_cutoff_phase();
    }
}

void player_fdsdfsfsdf()
{

}

void player_update_off_tick()
{
    if (g_channel->pitch_change_duration != 0)
    {
        effects_update_pitch_change_off_tick();
    }

    if (g_channel->vibrato_depth_target != 0)
    {
        effects_update_vibrato();
    }

    if ((g_channel->flags & PITCH_IS_CHANGING) != 0)
    {
        player_set_pitch();
    }

    if (g_channel->envelope_phase_duration != 0)
    {
        player_update_envelope_phase();
    }

    if (g_channel->flags & VOLUME_IS_CHANGING)
    {
        player_set_volume();
    }
}

void player_set_pitch()
{
    s32 intended_pitch;

    g_channel->flags &= ~PITCH_IS_CHANGING;

    intended_pitch = g_channel->pitch + g_channel->random_pitch + g_channel->vibrato_pitch;

    //DIFFERENCE: In Mahjong Master this normalizes the pitch by repeadetly adding a value in a for loop, MNSG and after just do this.
    if (intended_pitch < (0 << 8))
    {
        intended_pitch = 0;
    }
    else if (intended_pitch > (72 << 8))
    {
        intended_pitch = (72 << 8);
    }

    //DIFFERENCE: In MNSG, it checks if the previous pitch is equal to this, whereas Mahjong Master doesn't even set a previous pitch.
    alSynSetPitch(g_channel_index, (((f32) (intended_pitch & 0xFF) * (g_pitch_table[(intended_pitch >> 8) + 1] - g_pitch_table[intended_pitch >> 8])) / 256.0 + g_pitch_table[intended_pitch >> 8]));
}

void player_set_volume()
{
    s32 intended_volume;

    g_channel->flags &= ~VOLUME_IS_CHANGING;

    intended_volume = (g_channel->note_velocity + g_channel->marker_volume + g_channel->nested_marker_volume) * g_channel->volume * g_channel->vol_mult;
    //intended_volume = (g_channel->note_velocity + g_channel->marker_volume + g_channel->nested_marker_volume) * g_channel->volume;

    if (intended_volume < 0x800000)
    {
        intended_volume = intended_volume >> 8;

        if (intended_volume < 0x0000)
        {
            intended_volume = 0x0000;
        }
    }
    else
    {
        intended_volume = 0x7FFF;
    }

    intended_volume = (g_channel->envelope_volume_target * intended_volume) >> 8;

    if (g_channel->sequence_id < 0x100)
    {
        if (g_fade_amount != 0)
        {
            intended_volume = (g_fade_amount * intended_volume) >> 8;
        }

        // Something else? Find out what it is.
    }

    // Also something else. What is it?
    alSynSetVol(g_channel_index, intended_volume, g_channel->envelope_phase_duration * CALLBACK_TIME);
}

void player_start_envelope_attack_phase()
{
    g_channel->envelope_phase_duration = 0;

    if (g_channel->envelope_attack_duration == 0)
    {
        g_channel->envelope_volume_target = 0x100;
        player_set_volume();

        player_start_envelope_decay_phase();
    }
    else
    {
        g_channel->envelope_volume_target = 0x20;
        player_set_volume();

        g_channel->envelope_phase = ATTACK;
        g_channel->envelope_volume_target = 0x100;
        g_channel->envelope_phase_duration = g_channel->envelope_attack_duration;
        player_set_volume();
    }
}

void player_start_envelope_decay_phase()
{
    if (g_channel->envelope_decay_volume == 0)
    {
        player_start_envelope_sustain_phase();
    }
    else
    {
        g_channel->envelope_phase = DECAY;
        g_channel->envelope_volume_target -= g_channel->envelope_decay_volume;
        g_channel->envelope_phase_duration = g_channel->envelope_decay_duration;
        player_set_volume();
    }
}

void player_start_envelope_sustain_phase()
{
    g_channel->envelope_phase = SUSTAIN;

    if (g_channel->envelope_sustain_duration != 0)
    {
        g_channel->envelope_volume_target = 0;
        g_channel->envelope_phase_duration = g_channel->envelope_sustain_duration;
        player_set_volume();
    }
}

void player_start_envelope_release_phase()
{
    g_channel->envelope_phase = RELEASE;
    g_channel->envelope_volume_target = 0;
    g_channel->envelope_phase_duration = g_channel->envelope_release_duration;
    player_set_volume();
}

void player_start_envelope_cutoff_phase()
{
    g_channel->voice_status = 1;
    g_channel->envelope_phase = RELEASE;
    g_channel->envelope_volume_target = 0;
    g_channel->envelope_phase_duration = 1;
    player_set_volume();
}

void player_update_envelope_phase()
{
    g_channel->envelope_phase_duration--;
    if (g_channel->envelope_phase_duration == 0)
    {
        if (g_channel->envelope_phase == ATTACK)
        {
            player_start_envelope_decay_phase();
        }

        if (g_channel->envelope_phase == DECAY)
        {
            player_start_envelope_sustain_phase();
        }
    }
}
