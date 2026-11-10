# Brick mode

Brick mode turns the M-VAVE FM-1 into a controller and sound source for
[Brickwave](https://github.com/GrimWinters/Brickwave), a synth for the TrimUI
Brick, over a single USB cable. It is an addition to FM1 Open; everything else
works as before.

## Using it

- Connect the FM-1 to the Brick's rear USB-C port.
- Press **OCT-** and **OCT+** together: the FM-1 shows `BRICK MODE ON`, and
  while it is on, the status line reads `BRICK MODE  OCT- + OCT+ EXITS`.
- In brick mode the seven encoders and the panel buttons control Brickwave
  instead of the FM-1's pages. The note keys, the master knob and the FM-1's
  sound engine keep working as usual.
- Press OCT- and OCT+ together again to leave. Leaving also switches the two
  audio options below back off.

## Protocol (USB MIDI, channel 16)

FM-1 to host:

| Message | Meaning |
| --- | --- |
| CC 20-26 | encoders 0-6, relative: 64 = no change, 65 = +1 step, 63 = -1 step, up to +/-63 |
| CC 102-115 | panel buttons 0-13: 127 pressed, 0 released (OCT-, OCT+, FX, SCL, ENV, LFO, EDIT, GLO, HOME, SAVE, ARP, SEQ, PLAY, REC) |
| CC 116 | brick mode: 127 on, 0 off |

Notes from the keys go out as before (KEY OUT, see the SEQ MIDI pages).

Host to FM-1:

| Message | Meaning |
| --- | --- |
| CC 117 | mute local synth: >= 64 the FM-1's own sound is not heard at its outputs, only USB playback is |
| CC 118 | dry USB recording: >= 64 USB recording carries the synth only, without the USB playback (no feedback loop when the host processes the FM-1 and plays it back) |

Both audio options start off at power-up and switch off when brick mode is left.

## Code

- `app/screen.c`: `brick_input()` reads the panel before the pages do.
- `app/audio_output.c`: dry USB recording and local mute in `fm1_output_mix()`.
- `app/midi_router.c`: CC 117 / 118 on channel 16 from any MIDI input.

License as FM1 Open: GPL-3.0-only with the FM1 SDK linking exception.
