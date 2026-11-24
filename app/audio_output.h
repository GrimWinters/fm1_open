/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception */
#ifndef FM1_AUDIO_OUTPUT_H
#define FM1_AUDIO_OUTPUT_H
#include "fm1.h"
/* Reset before start or while the audio ISR is excluded. */
void fm1_output_reset(void);
/* Finish one synthesized block: master gain, USB, Bluetooth, USB recording.
 * Audio ISR only; output holds FM1_BLOCK_WORDS Q23 samples. */
FM1_AUDIO_CODE void fm1_output_mix(int32_t *output);
/* Brick mode host values, CC 117-122 and 85-87 on channel 16 (brick_mode.md),
 * stored by the MIDI router: [0] mute, [1] dry (>= 64 on; read once per audio
 * block), [2] bright LED, [3] [4] dim LED mask (read by the panel),
 * [5] keys play locally (>= 64; read by the router), [6] [7] the scale's
 * pitch classes (C = bit 0), [8] its root + 1 (0 = keys light as usual).
 * fm1_brick_on: brick mode is on (set by the panel). */
extern volatile uint8_t fm1_brick_cc[10];   /* [9]: the host's CC 116 + 1 (128 hello, 65 alive, 1 bye) */
extern unsigned fm1_brick_on;
#endif
