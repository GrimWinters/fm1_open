/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception */
#ifndef FM1_AUDIO_OUTPUT_H
#define FM1_AUDIO_OUTPUT_H
#include "fm1.h"
/* Reset before start or while the audio ISR is excluded. */
void fm1_output_reset(void);
/* Finish one synthesized block: master gain, USB, Bluetooth, USB recording.
 * Audio ISR only; output holds FM1_BLOCK_WORDS Q23 samples. */
FM1_AUDIO_CODE void fm1_output_mix(int32_t *output);
/* Brick mode (Brickwave on a TrimUI Brick, see brick_mode.md):
 * dry  - USB recording gets the synth only, without the USB playback that
 *        comes back from the host (no feedback loop when the host processes
 *        the FM-1 and plays it back).
 * mute - the FM-1's own synth is not heard locally; only USB playback is.
 * Set from the control/routing task; read once per block by the audio ISR. */
void fm1_output_set_brick_dry(unsigned on);
void fm1_output_set_brick_mute(unsigned on);
#endif
