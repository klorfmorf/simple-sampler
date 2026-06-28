#include "driver.h"

#define printf(...) do { } while (0)

void *alGlobals = NULL;

/* ----- externs ----- */
extern void *g_al_bank_files[BANK_COUNT]; // ALBankFile changed!
extern u8 g_mono;

extern u8 g_channel_index;
extern Channel g_channels[CHANNEL_COUNT];
extern Channel *g_channel;
extern u8 *g_sequence_data_needle;

/* ----- .data ----- */
u16 g_envelope_table[] =
{
       0,      1,      2,      2,      3,      4,      5,      6,
       7,      8,      9,     10,     11,     11,     12,     13,
      14,     15,     16,     17,     18,     19,     20,     21,
      22,     24,     26,     28,     30,     31,     32,     34,
      36,     38,     40,     42,     44,     46,     50,     54,
      58,     62,     66,     70,     74,     77,     80,     84,
      88,     94,    102,    110,    118,    124,    132,    140,
     148,    154,    160,    168,    176,    192,    208,    224,
     240,    254,    268,    284,    300,    314,    328,    344,
     360,    390,    420,    450,    480,    504,    528,    554,
     580,    610,    640,    670,    700,    760,    820,    880,
     940,   1000,   1060,   1120,   1180,   1240,   1300,   1360,
    1420,   1520,   1640,   1760,   1880,   2000,   2120,   2260,
    2400,   2500,   2600,   2700,   2800,   3040,   3280,   3540,
    3800,   4040,   4280,   4540,   4800,   5040,   5280,   5540,
    5800,   6200,   6600,   7000,   7600,   8200,   8800,   9600,
};

u8 g_vibrato_table[] =
{
    0,   32,  56,  80,
   104, 128, 144, 160,
   176, 192, 208, 224,
   232, 240, 248, 252,
   255, 252, 248, 240,
   232, 224, 208, 192,
   176, 160, 144, 128,
   104,  80,  56,  32,
};

u8 g_random_table[] = {
    0x9F, 0x3C, 0xB2, 0x52, 0xAF, 0x45, 0xC7, 0x89,
    0x10, 0x7F, 0xE0, 0x9D, 0xDC, 0x1F, 0x61, 0x16,
    0x39, 0xC9, 0x9C, 0xEB, 0x57, 0x08, 0x66, 0xF8,
    0x5A, 0x24, 0xBF, 0x0E, 0x3E, 0x15, 0x4B, 0xDB,
    0xAB, 0xF5, 0x31, 0x0C, 0x43, 0x02, 0x55, 0xDE,
    0x41, 0xDA, 0xBD, 0xAE, 0x19, 0xB0, 0x48, 0x57,
    0xBA, 0xA3, 0x36, 0x0B, 0xF9, 0xDF, 0x17, 0xA8,
    0x04, 0x0C, 0xE0, 0x91, 0x18, 0x5D, 0xDD, 0xD3,
    0x28, 0x8A, 0xF2, 0x11, 0x59, 0x6F, 0x06, 0x0A,
    0x34, 0x2A, 0x79, 0xAC, 0x5E, 0xA7, 0x83, 0xC6,
    0x39, 0xC1, 0xB4, 0x3A, 0x3F, 0xFE, 0x4F, 0xEF,
    0x1F, 0x00, 0x30, 0x99, 0x4C, 0x28, 0x83, 0xED,
    0x8A, 0x2F, 0x2C, 0x66, 0x3F, 0xD6, 0x6C, 0xB7,
    0x49, 0x22, 0xBC, 0x65, 0xFA, 0xCF, 0x02, 0xB1,
    0x46, 0xF0, 0x9A, 0xD7, 0xE2, 0x0F, 0x11, 0xC5,
    0x74, 0xF6, 0x7A, 0x2C, 0x8F, 0xFB, 0x19, 0x6A,
};

// TEMPORARY
u8 g_instrument_sounds_table[];
u8 g_sound_effect_sounds_table[];

Sound *g_sound_banks_table[2] =
{
    (Sound *)g_instrument_sounds_table,
    (Sound *)g_sound_effect_sounds_table,
};

// #include "includes/instrument_sounds_table.mnsg.h"
// #include "includes/instrument_sounds_table.mnsg_custom.h"
// #include "includes/instrument_sounds_table.mnsg_custom_2.h"
// #include "includes/instrument_sounds_table.mnsg.no_piano_tuning.h"
#include "includes/instrument_sounds_table.gga.h"
// #include "includes/instrument_sounds_table.gms.h"
// #include "includes/instrument_sounds_table.mnsg_to_gga.h"
// #include "includes/instrument_sounds_table.iss64.h"
// #include "includes/instrument_sounds_table.jws3.h"
// #include "includes/instrument_sounds_table.iss2000.h"
// #include "includes/instrument_sounds_table.cv.h"
// #include "includes/instrument_sounds_table.mm.h"

// #include "includes/sound_effect_sounds_table.mnsg.h"
#include "includes/sound_effect_sounds_table.gga.h"
// #include "includes/sound_effect_sounds_table.cv.h"
// #include "includes/sound_effect_sounds_table.mm.h"

Sound g_default_sound = {
    0, 0, 127, 0, 0, 31, 64,
};

// #include "includes/drum_sounds_table.mnsg.h"
#include "includes/drum_sounds_table.gga.h"
// #include "includes/drum_sounds_table.mnsg_to_gga.h"
// #include "includes/drum_sounds_table.gms.h"
// #include "includes/drum_sounds_table.iss64.h"
// #include "includes/drum_sounds_table.jws3.h"
// #include "includes/drum_sounds_table.iss2000.h"
// #include "includes/drum_sounds_table.cv.h"
// #include "includes/drum_sounds_table.mm.h"

void effects_set_tempo()
{
    g_channel->tempo = *g_sequence_data_needle << 8;
    g_sequence_data_needle++;

    g_channel->tempo_change_duration = 0;
}

void effects_set_tempo_change()
{
    s32 tempo_change_delta;

    g_channel->tempo_change_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->tempo_change_target = *g_sequence_data_needle;
    g_sequence_data_needle++;

    tempo_change_delta = (g_channel->tempo_change_target << 8) - g_channel->tempo;

    g_channel->tempo_change_amount = tempo_change_delta / g_channel->tempo_change_duration;
}

void effects_update_tempo_change()
{
    s32 new_tempo;

    g_channel->tempo_change_duration--;

    if (g_channel->tempo_change_duration != 0)
    {
        new_tempo = g_channel->tempo + g_channel->tempo_change_amount;

        if (new_tempo < 0x0100)
        {
            g_channel->tempo = new_tempo;
        }
        else if (new_tempo >= 0xFF00)
        {
            new_tempo = 0xFF00;
        }

        g_channel->tempo = new_tempo;
    }
    else
    {
        g_channel->tempo = g_channel->tempo_change_target << 8;
    }
}

void effects_set_sound()
{
    Sound *sound;

    // Something to do with panning here?

    g_channel->sound = *g_sequence_data_needle;
    g_sequence_data_needle++;

    sound = &g_default_sound;

    if (g_channel->bank < 2)
    {
        sound = g_sound_banks_table[g_channel->bank] + g_channel->sound;
    }

    effects_load_sound(sound);
}

void effects_load_sound(Sound *sound)
{
    g_channel->wave_table = NULL;

    g_channel->sound_transpose_detune = sound->transpose << 8;
    g_channel->sound_transpose_detune += sound->detune << 2;

    g_channel->envelope_attack_duration = g_envelope_table[0x7F - sound->attack];

    g_channel->envelope_decay_duration = g_envelope_table[0x3D - ((sound->decay & 0xF0) >> 2)];
    g_channel->envelope_decay_volume = (sound->decay & 0x0F) << 2;

    if (sound->sustain == 0)
    {
        g_channel->envelope_sustain_duration = 0;
    }
    else
    {
        g_channel->envelope_sustain_duration = g_envelope_table[0x7F - sound->sustain] + 1;
    }

    g_channel->envelope_release_duration = g_envelope_table[0x7D - (sound->release << 2)];

    if (g_channel->pan < 0x8000)
    {
        g_channel->pan = sound->pan << 8;
        g_channel->pan_change_duration = 0;
    }

    effects_set_pan_();
}

void effects_load_drum_sound()
{
#ifdef USE_OLD_INST_DRUM_FORMAT
    g_channel->bank = ((DrumSound *)g_drum_sounds_table)[g_channel->note_pitch - 0x48].bank; // Early versions only.
#else
    g_channel->bank = 0; // Seems to fix Goemon's Great Adventure Game Over
#endif
    g_channel->sound = ((DrumSound *)g_drum_sounds_table)[g_channel->note_pitch - 0x48].id;

    effects_load_sound(&((DrumSound *)g_drum_sounds_table)[g_channel->note_pitch - 0x48].sound);
}

void effects_set_volume_sound()
{
    effects_set_volume();
    effects_set_sound();
}

void effects_set_volume_sound_pan()
{
    effects_set_volume();
    effects_set_sound();
    effects_set_pan();
}

void effects_set_volume()
{
    u8 volume;

    volume = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if ((g_channel->sequence_id >> 24) == 0)
    {
        g_channel->volume = volume << 8;
    }
    else
    {
        g_channel->volume = (u16)volume * (g_channel->sequence_id >> 24);
    }

    g_channel->volume_change_duration = 0;
}

void effects_set_volume_change()
{
    s32 volume_change_delta;

    g_channel->volume_change_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->volume_change_target = *g_sequence_data_needle;
    g_sequence_data_needle++;

    volume_change_delta = (g_channel->volume_change_target << 8) - g_channel->volume;

    g_channel->volume_change_amount = volume_change_delta / g_channel->volume_change_duration;
}

void effects_update_volume_change()
{
    s32 new_volume;

    g_channel->flags |= VOLUME_IS_CHANGING;

    g_channel->volume_change_duration--;
    if (g_channel->volume_change_duration != 0)
    {
        new_volume = g_channel->volume + g_channel->volume_change_amount;

        if (new_volume < 0x0000)
        {
            g_channel->volume = new_volume;
        }
        else if (new_volume > 0xFF00)
        {
            new_volume = 0xFF00;
        }

        g_channel->volume = new_volume;
    }
    else
    {
        g_channel->volume = g_channel->volume_change_target << 8;
    }
}

void effects_set_attack_decay()
{
    g_channel->envelope_attack_duration = g_envelope_table[0x7F - *g_sequence_data_needle];
    g_sequence_data_needle++;

    g_channel->envelope_decay_duration = g_envelope_table[0x3D - ((*g_sequence_data_needle & 0xF0) >> 2)];
    g_channel->envelope_decay_volume = (*g_sequence_data_needle & 0x0F) << 2;
    g_sequence_data_needle++;
}

void effects_set_sustain()
{
    u8 sustain;

    sustain = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (sustain == 0)
    {
        g_channel->envelope_sustain_duration = 0;

        if ((g_channel->note_hold_ratio == 0) && (g_channel->envelope_phase == SUSTAIN))
        {
            g_channel->envelope_phase_duration = 0x7FFF;
        }
    }
    else
    {
        g_channel->envelope_sustain_duration = g_envelope_table[0x7F - sustain] + 1;

        if ((g_channel->note_hold_ratio == 0) && (g_channel->envelope_phase == SUSTAIN))
        {
            g_channel->envelope_phase_duration = g_channel->envelope_sustain_duration;
        }
    }
}

void effects_set_release()
{
    g_channel->envelope_release_duration = g_envelope_table[0x7D - (*g_sequence_data_needle << 2)];
    g_sequence_data_needle++;

    if ((g_channel->note_hold_ratio == 0) && (g_channel->envelope_phase == RELEASE))
    {
        g_channel->envelope_phase_duration = g_channel->envelope_release_duration;
    }
}

//TEMPROARY
extern s32 g_custom_parameters[];
extern s32 g_custom_parameters_gga[];

void effects_set_reverb()
{
    u8 parameter_1;
    u8 parameter_2;
    s32 parameters[2];

    if (g_channel->dry_wet_mix != 0)
    {
        alSynSetFXMix(g_channel_index, 0);
    }

    parameter_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    parameters[0] = parameter_1 * 40;
    if (parameters[0] == 0)
    {
        parameters[0] = g_custom_parameters[3]; /* Change for later versions */
    }

    alSynSetFXParam(3, parameters);

    parameter_2 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    parameters[0] = parameter_2 * 128;
    if (parameters[0] == 0)
    {
        parameters[0] = g_custom_parameters[4]; /* Change for later versions */
    }

    alSynSetFXParam(4, parameters);

    if (g_channel->dry_wet_mix != 0)
    {
        alSynSetFXMix(g_channel_index, g_channel->dry_wet_mix);
    }
}

void effects_set_user_defined()
{
    g_sequence_data_needle++;
}

void effects_set_pan()
{
    g_channel->pan_change_duration = 0;

    g_channel->pan = *g_sequence_data_needle << 8;
    g_sequence_data_needle++;

    effects_set_pan_();
}

void effects_set_pan_()
{
    u8 pan;

    if (g_mono == 0)
    {
        pan = (g_channel->pan >> 8) & 0x7F;
    }
    else
    {
        pan = 0x40;
    }

    alSynSetPan(g_channel_index, pan);
}

void effects_set_pan_change()
{
    s32 pan_change_delta;

    g_channel->pan_change_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->pan_change_target = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if ((g_channel->pan > 0x7FFF) || ((g_channel->flags & IS_DRUM) == 0))
    {
        pan_change_delta = (g_channel->pan_change_target << 8) - (g_channel->pan & 0x7FFF);

        g_channel->pan_change_amount = pan_change_delta / g_channel->pan_change_duration;
    }
}

void effects_update_pan_change()
{
    s32 new_pan;
    u16 old_pan;

    g_channel->flags |= VOLUME_IS_CHANGING;

    old_pan = g_channel->pan;

    g_channel->pan_change_duration--;
    if (g_channel->pan_change_duration != 0)
    {
        new_pan = (g_channel->pan & 0x7FFF) + g_channel->pan_change_amount;

        if (new_pan < 0x0000)
        {
            new_pan = 0;
        }
        else if (new_pan > 0x7FFF)
        {
            new_pan = 0x7F00;
        }

        g_channel->pan = new_pan;
    }
    else
    {
        g_channel->pan = g_channel->pan_change_target << 8;
    }

    g_channel->pan |= old_pan & 0x8000;

    effects_set_pan_();
}

void effects_set_transpose()
{
    g_channel->effect_transpose = (s8)*g_sequence_data_needle << 8;
    g_sequence_data_needle++;
}

void effects_set_detune()
{
    g_channel->effect_detune = (s8)*g_sequence_data_needle << 2;
    g_sequence_data_needle++;
}

void effects_set_vibrato()
{
    u8 delay;
    u8 rate;

    delay = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (delay < 100)
    {
        g_channel->vibrato_intended_delay_duration = delay;
    }
    else
    {
        g_channel->vibrato_intended_delay_duration = 0;
        g_channel->vibrato_intended_start_duration = delay - 100;
    }

    rate = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (rate < 0x20)
    {
        rate = ((rate & 0x1F) << 3);
        g_channel->vibrato_oscillation_step_size = 1;
    }
    else if (rate < 0x40)
    {
        rate = ((rate & 0x3F) << 2);
        g_channel->vibrato_oscillation_step_size = 2;
    }
    else if (rate < 0x80)
    {
        rate = ((rate & 0x7F) << 1);
        g_channel->vibrato_oscillation_step_size = 4;
    }
    else if (rate < 0xFF)
    {
        g_channel->vibrato_oscillation_step_size = 8;
    }
    else if (rate == 0xFF)
    {
        g_channel->vibrato_oscillation_step_size = 16;
    }

    g_channel->vibrato_rate = rate;

    g_channel->vibrato_depth_target = *g_sequence_data_needle << 8;
    g_sequence_data_needle++;

    if (g_channel->vibrato_intended_start_duration != 0)
    {
        g_channel->vibrato_depth_change_amount = g_channel->vibrato_depth_target / g_channel->vibrato_intended_start_duration;
    }
}

void effects_initialize_vibrato()
{
    g_channel->vibrato_delay_duration = g_channel->vibrato_intended_delay_duration;
    g_channel->vibrato_start_duration = g_channel->vibrato_intended_start_duration;

    if (g_channel->vibrato_intended_start_duration == 0)
    {
        g_channel->vibrato_depth = g_channel->vibrato_depth_target;
    }
    else
    {
        g_channel->vibrato_depth = 0;
    }

    g_channel->vibrato_counter = 0;
    g_channel->vibrato_oscillation = g_channel->vibrato_counter;
}

void effects_update_vibrato_on_tick()
{
    if (g_channel->vibrato_delay_duration == 0)
    {
        if (g_channel->vibrato_start_duration != 0)
        {
            g_channel->vibrato_start_duration--;

            if (g_channel->vibrato_start_duration == 0)
            {
                g_channel->vibrato_depth = g_channel->vibrato_depth_target;
            }
            else
            {
                g_channel->vibrato_depth += g_channel->vibrato_depth_change_amount;
            }
        }

        effects_update_vibrato();
    }
    else
    {
        g_channel->vibrato_delay_duration--;
    }
}

void effects_update_vibrato()
{
    if (g_channel->vibrato_delay_duration == 0)
    {
        g_channel->vibrato_counter += g_channel->vibrato_rate;

        if (g_channel->vibrato_counter >= 0x100)
        {
            g_channel->vibrato_counter &= 0xFF;

            g_channel->flags |= PITCH_IS_CHANGING;

            g_channel->vibrato_oscillation += g_channel->vibrato_oscillation_step_size;
            g_channel->vibrato_oscillation &= 0x3F;

            if (g_channel->vibrato_depth < 0x8000)
            {
                g_channel->vibrato_pitch = g_channel->vibrato_depth >> 7;
                g_channel->vibrato_pitch = (g_vibrato_table[g_channel->vibrato_oscillation & 0x1F] * g_channel->vibrato_pitch) >> 8;
            }
            else
            {
                g_channel->vibrato_pitch = (g_channel->vibrato_depth >> 8) & 0x7F;
                g_channel->vibrato_pitch = g_channel->vibrato_pitch + 2;
                g_channel->vibrato_pitch = (g_vibrato_table[g_channel->vibrato_oscillation & 0x1F] * g_channel->vibrato_pitch) >> 1;
            }

            if (g_channel->vibrato_oscillation >= 0x20)
            {
                g_channel->vibrato_pitch = -g_channel->vibrato_pitch;
            }
        }
    }
}

void effects_set_vibrato_change()
{
    g_channel->vibrato_intended_start_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (g_channel->vibrato_intended_start_duration != 0)
    {
        g_channel->vibrato_depth_change_amount = g_channel->vibrato_depth_target / g_channel->vibrato_intended_start_duration;
    }

}

void effects_set_random()
{
    g_channel->random_rate = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->random_pitch_mask = *g_sequence_data_needle << 8;
    g_sequence_data_needle++;

    g_channel->random_pitch_mask += *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->random_index = 0;
    g_channel->random_counter = g_channel->random_index;

    if (g_channel->random_rate == 0)
    {
        g_channel->random_pitch = 0;
    }
}

void effects_update_random()
{
    g_channel->random_counter += g_channel->random_rate;

    if (g_channel->random_counter >= 0x100)
    {
        g_channel->random_counter &= 0xFF;

        g_channel->flags |= PITCH_IS_CHANGING;

        g_channel->random_pitch = g_random_table[g_channel->random_index];

        g_channel->random_index++;
        g_channel->random_index &= 0x7F;

        g_channel->random_pitch += g_random_table[g_channel->random_index] << 8;
        g_channel->random_pitch &= g_channel->random_pitch_mask;
    }
}

void effects_set_pitch_bend_dummy()
{
    g_sequence_data_needle += 3;
}

void effects_calculate_pitch_change_amount()
{
    s32 pitch_change_delta;
    s32 pitch_change_duration;

    pitch_change_delta = g_channel->pitch_change_target - g_channel->pitch;
    pitch_change_duration = ((((g_channel->pitch_change_duration << 16) / g_channel->tempo) & 0xFFFF) + 1) & 0xFFFF;

    g_channel->pitch_change_amount = pitch_change_delta / pitch_change_duration;

    if (g_channel->pitch_change_amount == 0)
    {
        if (pitch_change_delta < 0)
        {
            g_channel->pitch_change_amount = -1;
        }
        else
        {
            g_channel->pitch_change_amount = 1;
        }
    }
}

/* for debug */
#include <stdio.h>
/* end debug */

void effects_set_pitch_bend()
{
    u8 *next_sequence_data_needle;
    u8 pitch_target;

    next_sequence_data_needle = g_sequence_data_needle + 1;

    if (*g_sequence_data_needle == 0xE4)
    {
        if (g_channel->pitch_change_next_note_parameter == 0)
        {
            g_sequence_data_needle++;

            g_channel->pitch_change_type = FIXED_LENGTH;

            g_channel->pitch_change_delay_duration = *g_sequence_data_needle;
            g_sequence_data_needle++;

            g_channel->pitch_change_duration = *g_sequence_data_needle;
            g_sequence_data_needle++;

            pitch_target = *g_sequence_data_needle;
            g_sequence_data_needle++;

            g_channel->pitch_change_target = g_channel->pitch + ((pitch_target - g_channel->note_pitch) << 8);

            /* for debug */
            printf(KGRN "Track %02d | " KBLU "effects_set_pitch_bend " KCYN "%d" KNRM ", " KCYN "%d" KNRM ", " KCYN "%d" KNRM "\n", g_channel_index, g_channel->pitch_change_delay_duration, g_channel->pitch_change_duration, pitch_target);
            /* end debug */

            effects_calculate_pitch_change_amount();
        }
        else
        {
            g_sequence_data_needle += 4;
        }

        g_channel->sequence_data_needle = g_sequence_data_needle;
        next_sequence_data_needle = g_sequence_data_needle;
    }

    g_sequence_data_needle = next_sequence_data_needle;
}

void effects_update_pitch_change_on_tick()
{
    if (g_channel->pitch_change_type == FIXED_LENGTH)
    {
        if (g_channel->pitch_change_delay_duration == 0)
        {
            g_channel->flags |= PITCH_IS_CHANGING;

            g_channel->pitch_change_duration--;
            if (g_channel->pitch_change_duration == 0)
            {
                g_channel->pitch = g_channel->pitch_change_target;
            }
            else
            {
                g_channel->pitch += g_channel->pitch_change_amount;
            }
        }
        else
        {
            g_channel->pitch_change_delay_duration--;
        }
    }
    else
    {
        effects_update_portamento();
    }
}

void effects_update_pitch_change_off_tick()
{
    if (g_channel->pitch_change_type == FIXED_LENGTH)
    {
        if (g_channel->pitch_change_delay_duration == 0)
        {
            g_channel->flags |= PITCH_IS_CHANGING;

            g_channel->pitch += g_channel->pitch_change_amount;
        }
    }
    else
    {
        effects_update_portamento();
    }
}

void effects_set_pitch_bend_start()
{
    g_channel->pitch_change_type = FIXED_LENGTH;

    g_channel->pitch_change_next_note_delay_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->pitch_change_next_note_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->pitch_change_next_note_parameter = ((s8)*g_sequence_data_needle << 8);
    g_sequence_data_needle++;
}

void effects_calculate_pitch_change_amount_this_note()
{
    if (g_channel->pitch_change_type == FIXED_LENGTH)
    {
        g_channel->pitch_change_delay_duration = g_channel->pitch_change_next_note_delay_duration;
        g_channel->pitch_change_duration = g_channel->pitch_change_next_note_duration;
        g_channel->pitch_change_target = g_channel->pitch;
        g_channel->pitch -= g_channel->pitch_change_next_note_parameter;
        effects_calculate_pitch_change_amount();
    }
    else
    {
        g_channel->pitch_change_target = g_channel->pitch;
        g_channel->pitch = g_channel->pitch_change_amount;
        g_channel->pitch_change_duration = 1;
    }
}

void effects_set_portamento()
{
    g_channel->pitch_change_type = VARIABLE_LENGTH;

    g_channel->pitch_change_next_note_parameter = *g_sequence_data_needle;
    g_sequence_data_needle++;
}

void effects_update_portamento()
{
    s32 pitch_delta;
    s32 pitch_delta_absolute;
    s32 pitch_change_amount;

    g_channel->flags |= PITCH_IS_CHANGING;

    pitch_delta = g_channel->pitch_change_target - g_channel->pitch;
    pitch_delta_absolute = pitch_delta;
    if (pitch_delta < 0)
    {
        pitch_delta_absolute = -pitch_delta;
    }

    pitch_change_amount = (g_channel->pitch_change_next_note_parameter * pitch_delta_absolute) >> 8;
    if (pitch_change_amount == 0)
    {
        pitch_change_amount = 1;
    }

    if (pitch_delta < 0)
    {
        g_channel->pitch = g_channel->pitch - pitch_change_amount;

        if (g_channel->pitch < g_channel->pitch_change_target)
        {
            g_channel->pitch = g_channel->pitch_change_target;
        }
    }
    else
    {
        g_channel->pitch = g_channel->pitch + pitch_change_amount;

        if (g_channel->pitch > g_channel->pitch_change_target)
        {
            g_channel->pitch = g_channel->pitch_change_target;
        }
    }

    if (g_channel->pitch == g_channel->pitch_change_target)
    {
        g_channel->pitch_change_duration = 0;
    }
}

void effects_set_marker()
{
    g_channel->marker_loop_counter = 0;
    g_channel->marker_needle = g_sequence_data_needle;
    g_channel->marker_volume = 0;
    g_channel->marker_transpose = g_channel->marker_volume;
}

void effects_go_to_marker()
{
    u8 count;

    count = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->marker_loop_counter++;

    if ((count != 0) && (g_channel->marker_loop_counter == count))
    {
        g_channel->marker_volume = 0;
        g_channel->marker_transpose = g_channel->marker_volume;

        g_sequence_data_needle += 2;
    }
    else
    {
        g_channel->marker_volume += (s8)*g_sequence_data_needle;
        g_sequence_data_needle++;

        g_channel->marker_transpose += (s8)*g_sequence_data_needle << 3;
        g_sequence_data_needle++;

        /* printf("Channel %d: Sequence data needle set to %p.\n", g_channel_index, g_channel->marker_needle); */

        g_sequence_data_needle = g_channel->marker_needle;
    }
}

void effects_set_nested_marker()
{
    g_channel->nested_marker_loop_counter = 0;
    g_channel->nested_marker_needle = g_sequence_data_needle;
    g_channel->nested_marker_volume = 0;
    g_channel->nested_marker_transpose = g_channel->nested_marker_volume;
}

void effects_go_to_nested_marker()
{
    u8 count;

    count = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->nested_marker_loop_counter++;

    if ((count != 0) && (g_channel->nested_marker_loop_counter == count))
    {
        g_channel->nested_marker_volume = 0;
        g_channel->nested_marker_transpose = (s8)g_channel->nested_marker_volume;

        g_sequence_data_needle += 2;
    }
    else
    {
        g_channel->nested_marker_volume += *g_sequence_data_needle;
        g_sequence_data_needle++;

        g_channel->nested_marker_transpose += (s8)*g_sequence_data_needle << 3;
        g_sequence_data_needle++;

        g_sequence_data_needle = g_channel->nested_marker_needle;
    }
}

void effects_set_master_marker()
{
    g_channel->master_marker_needle = g_sequence_data_needle;
}

void effects_go_to_master_marker()
{
    g_sequence_data_needle = g_channel->master_marker_needle;
}

void effects_set_conditional_marker()
{
    g_channel->conditional_marker_flag = 0;
    g_channel->conditional_marker_needle_1 = g_sequence_data_needle;
}

void effects_go_to_conditional_marker()
{
    switch (g_channel->conditional_marker_flag)
    {
    case 0:
        g_channel->conditional_marker_flag = 1;
        break;

    case 1:
        g_channel->conditional_marker_flag++;
        g_channel->conditional_marker_needle_2 = g_sequence_data_needle;
        g_sequence_data_needle = g_channel->conditional_marker_needle_1;
        break;

    case 2:
        g_channel->conditional_marker_flag--;
        g_sequence_data_needle = g_channel->conditional_marker_needle_2;
        break;
    }
}

void effects_set_echo()
{
    u8 dry_wet_mix;

    dry_wet_mix = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (g_channel->dry_wet_mix != dry_wet_mix)
    {
        g_channel->dry_wet_mix = dry_wet_mix;
        alSynSetFXMix(g_channel_index, dry_wet_mix);
    }
}

void effects_set_bank()
{
    g_channel->bank = *g_sequence_data_needle;
    g_sequence_data_needle++;
}

#define MNSG_USE

extern u8 g_next_part_sequence_id;
extern u8 g_next_part_can_start;
extern u8 g_set_next_sequence;

void effects_set_nothing()
{
#ifdef MNSG_USE
    u8 value = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (value & 1) {
        if ((u8)g_channel->sequence_id < g_next_part_sequence_id) {
            g_channel->disable = 0;
        } else {
            g_channel->disable = 1;
        }
    } else {
        if ((u8)g_channel->sequence_id != g_next_part_sequence_id) {
            g_next_part_can_start = 1;
        } else {
            g_set_next_sequence++;
        }
    }
#endif
}

void effects_rest()
{
    alSynSetVol(g_channel_index, 0, CALLBACK_TIME);

    g_channel->voice_status = 1;

    g_channel->note_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->note_hold_ratio = 1;

    g_channel->sequence_data_needle = g_sequence_data_needle;
}

void effects_tie()
{
    g_channel->note_duration = *g_sequence_data_needle;
    g_sequence_data_needle++;

    g_channel->note_hold_ratio = *g_sequence_data_needle;
    g_sequence_data_needle++;

    if (g_channel->note_hold_ratio == 0)
    {
        g_channel->note_hold_duration = 0;
    }
    else
    {
        g_channel->note_hold_duration = (g_channel->note_duration * g_channel->note_hold_ratio) / 128;

        if (g_channel->note_hold_duration == 0)
        {
            g_channel->note_hold_duration = 1;
        }
    }

    g_channel->sequence_data_needle = g_sequence_data_needle;

    if (g_channel->envelope_phase_duration == 0)
    {
        player_set_volume(); // is actually player_fdsdfsfsdf
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

void effects_F8() {
    u8 param_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_2 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_3 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    printf("Channel %d: 0xF8(Param 1 = %d, Param 2 = %d, Param 3 = %d)\n", g_channel_index, param_1, param_2, param_3);
}

void effects_F9() {
    u8 param_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_2 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    printf("Channel %d: 0xF9(Param 1 = %d, Param 2 = %d)\n", g_channel_index, param_1, param_2);
}

void effects_FA() {
    u8 param_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    printf("Channel %d: 0xFA(Param 1 = %d)\n", g_channel_index, param_1);
}

void effects_FB() {
    u8 param_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_2 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    printf("Channel %d: 0xFB(Param 1 = %d, Param 2 = %d)\n", g_channel_index, param_1, param_2);
}

void effects_FC() {
    effects_FC_FD_FE_sub(2);
}

void effects_FD() {
    effects_FC_FD_FE_sub(3);
}

void effects_FE() {
    effects_FC_FD_FE_sub(4);
}

void effects_FC_FD_FE_sub(u8 arg) {
    u8 param_1 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_2 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    u8 param_3 = *g_sequence_data_needle;
    g_sequence_data_needle++;

    printf("Channel %d: 0xFC_FD_FE(Param 1 = %d, Param 2 = %d, Param 3 = %d, Arg = %d)\n", g_channel_index, param_1, param_2, param_3, arg);
}


void effects_end()
{
    alSynSetVol(g_channel_index, 0, CALLBACK_TIME);

    g_channel->voice_status = 1;
    g_channel->sequence_id = 0;
    g_channel->priority = g_channel->sequence_id;
}
