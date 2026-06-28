#include "driver.h"

#include <string.h>

#define ms * (((s32) ((f32) FREQUENCY / 1000)) & ~0x7)

#include <stdio.h>

#include "includes/gga_sequences/160.kseq.h"
// #include "includes/mnsg_sequences/13.kseq.h"
// #include "includes/mm_sequences/1.kseq.h"
// #include "includes/cv64_sequences/65.kseq.h"
// #include "includes/gms_sequences/19.kseq.h"
// #include "includes/other_sequences/seq_0e_credits.h"

u8 has_checksum = TRUE;

u16 seq_id = 1;

extern Channel g_channels[CHANNEL_COUNT];
extern Channel *g_channel;

/* ----- .data ----- */
// MNSG
s32 g_custom_parameters[10] = {
    1, 300 ms,
    0, 179 ms, 0x2EE0, 0, 0x7FFF, 0, 0, 0
};

// GGA
s32 g_custom_parameters_gga[10] = {
    1, 300 ms,
    0, 179 ms, 0x3200, 0, 0x7FFF, 0, 0, 0
};

/* ----- .bss ----- */
u8 g_next_part_sequence_id;
u8 g_next_part_can_start;

// custom
u8 g_set_next_sequence = 0;

u8 g_audio_heap[AUDIO_HEAP_SIZE];

void *g_al_bank_files[BANK_COUNT]; // ALBankFile changed!

u8 g_initial_delay_counter;
u8 g_disable_request_handling;
u16 g_counter;
u64 g_counter_time;
u8 g_voice_index;
u8 g_paused;
u8 g_mono;
u8 g_fade_counter_reset_value;
u8 g_fade_counter;
u8 g_fade_amount;
u8 g_tempo_increase_amount;
u8 g_tempo_increase_reference;
// ...
// ...
// ... [1]
// ... [1]
// ...
u8 g_request_count;
u32 g_requests[REQUESTS_COUNT];
u32 g_request;

void external_initialize_player(void *player) {
    g_initial_delay_counter = INITIAL_DELAY;
    // ...
    g_paused = FALSE;
    g_mono = FALSE;
    // ...
    g_counter = 0;

    memset(g_channels, 0, sizeof(g_channels));

}

s32 external_player_handler(void *player) {
    if (g_initial_delay_counter == 0) {

        if ((g_counter & 1) == 0 && !g_disable_request_handling)
        {
            // Handle requests here...
        }

        // Load sequences here...

        player_update_channels();

        g_counter++;
        g_counter_time++;

#if 1
        if (g_counter_time == ((130 * 1000000) / 5000)) {
        	start_fade_out(10 * 1000000);
            printf("Master   | Fade Out\n");
        }
#endif
    }
    else {
        if (g_initial_delay_counter == INITIAL_DELAY) {
            external_initialize_channels(seq_data, seq_tracks, seq_id);
        }

        g_initial_delay_counter--;
    }

    return CALLBACK_TIME;
}

u8 external_is_finished(void) {
    s32 index;

    if (g_initial_delay_counter != 0) {
        return FALSE;
    }

    for (index = 0; index < CHANNEL_COUNT; index++) {
        if (g_channels[index].sequence_id != 0 || g_channels[index].voice_status != 0) {
            return FALSE;
        }
    }

    return TRUE;
}

void external_initialize_channels(u8 *data, u8 tracks, u16 id) {
    g_counter_time = 0;

    //TEMPORARY: Should not be here, do it properly later.
    u8 track_count = tracks;
    u8 channel_offset = 0;
    u8 priority = 30;
    u8 bank = 0;
    u8 initial_delay = 0;
    u8 *tracks_sequence_data[CHANNEL_COUNT];
    s32 index;

    if (has_checksum)
    {
	    track_count = track_count - 2;
    }

    for (index = 0; index < track_count; index++)
    {
        u16 track_offset = (data[(index * sizeof(u16))] << 8 | data[(index * sizeof(u16)) + 1]);

        if (track_offset >= sizeof(seq_data)) {
            tracks_sequence_data[index] = NULL;
            continue;
        }

        u8* track_data = data + track_offset;

        tracks_sequence_data[index] = track_data;
    }

    g_request = id;
    g_next_part_sequence_id = id;

    if (track_count != 0)
    {
        for (index = 0; index < track_count; index++)
        {
            if (tracks_sequence_data[index] == NULL) continue;

            // if (g_channels[index + channel_offset].dry_wet_mix != 0)
            // {
                //g_channels[index + channel_offset].dry_wet_mix = 0;
            // }

            if (g_request >= 0x100)
            {
                g_channels[index + channel_offset].voice_status = 2;
            }

            g_channels[index + channel_offset].initial_delay_counter = initial_delay;
            g_channels[index + channel_offset].sequence_id = g_request;
            g_channels[index + channel_offset].priority = priority;
            g_channels[index + channel_offset].bank = bank;
            g_channels[index + channel_offset].random_pitch = 0;
            g_channels[index + channel_offset].effect_detune = 0;
            g_channels[index + channel_offset].nested_marker_transpose = 0;
            g_channels[index + channel_offset].nested_marker_volume = 0;
            g_channels[index + channel_offset].pitch_change_next_note_parameter = 0;
            g_channels[index + channel_offset].tempo = 0x8000;
            g_channels[index + channel_offset].tempo_counter = 0;
            g_channels[index + channel_offset].note_duration = 1;
            g_channels[index + channel_offset].note_hold_ratio = 1;
            g_channels[index + channel_offset].tempo_change_duration = 0;
            g_channels[index + channel_offset].volume_change_duration = 0;
            g_channels[index + channel_offset].pan_change_duration = 0;
            g_channels[index + channel_offset].pan = 0;
            g_channels[index + channel_offset].sequence_data_needle = tracks_sequence_data[index];
            g_channels[index + channel_offset].random_rate = 0; // is set to volume?
            g_channels[index + channel_offset].vibrato_depth_target = 0; // this too
            g_channels[index + channel_offset].effect_transpose = 0;
            g_channels[index + channel_offset].marker_transpose = 0;
            g_channels[index + channel_offset].marker_volume = 0;
            g_channels[index + channel_offset].pitch_change_duration = 0;
            g_channels[index + channel_offset].vol_mult = 1;
			g_channels[index + channel_offset].previous_bank = 0;
			g_channels[index + channel_offset].special_bank_used = 0;
        }
    }
}
