/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception */
#ifndef FM1_AUDIO_OUTPUT_H
#define FM1_AUDIO_OUTPUT_H
#include "fm1.h"
/* Reset before start or while the audio ISR is excluded. */
void fm1_output_reset(void);
/* Finish one synthesized block: master gain, USB, Bluetooth, USB recording.
 * Audio ISR only; output holds FM1_BLOCK_WORDS Q23 samples. */
FM1_AUDIO_CODE void fm1_output_mix(int32_t *output);
/* Brick mode (brick_mode.md). DRY: USB recording carries the synth only,
 * without the USB playback (no feedback loop). MUTE: the synth is not heard
 * locally, only USB playback. Control/routing task; read per audio block. */
enum { FM1_BRICK_DRY = 1, FM1_BRICK_MUTE = 2 };
void fm1_output_set_brick(unsigned flags);
unsigned fm1_output_brick(void);
#endif
