# Brick mode

Brick mode turns the M-VAVE FM-1 into a controller and sound source for
[Brickwave](https://github.com/GrimWinters/Brickwave), a synth for the TrimUI
Brick, over a single USB cable. It is an addition to FM1 Open; everything else
works as before.

## Using it

- Connect the FM-1 to the Brick's rear USB-C port.
- Press **OCT-** and **OCT+** together: while brick mode is on, the status
  line shows `BRICK MODE` each time you turn an encoder or press a button.
- In brick mode the seven encoders and the panel buttons control Brickwave
  instead of the FM-1's pages. The note keys, the master knob and the FM-1's
  sound engine keep working as usual.
- Press OCT- and OCT+ together again to leave.

## Protocol (USB MIDI, channel 16)

FM-1 to host:

| Message | Meaning |
| --- | --- |
| CC 20-26 | encoders, relative: 64 = no change, 65 = +1 step, 63 = -1 step, up to +/-63. 20 Select, 21 Algorithm, 22-25 knobs 1-4, 26 Presets |
| CC 102-115 | panel buttons: 127 pressed, 0 released. 102 OCT-, 103 OCT+, 104 FX, 105 SEL, 106 ENV, 107 LFO, 108 EDIT, 109 GLO, 110 HOME, 111 SAVE, 112 ARP, 113 SEQ, 114 PLAY/STOP, 115 REC |
| CC 116 | brick mode: 127 on, 0 off |

Notes from the keys go out as before (KEY OUT, see the SEQ MIDI pages).

The master knob is not an encoder: it keeps setting the FM-1's volume.

Host to FM-1 (any MIDI input; all values reset to 0 when brick mode is
switched on or off, so the host sends its state after CC 116):

| Message | Meaning |
| --- | --- |
| CC 117 | mute: >= 64, the FM-1's own sound leaves its local outputs; only USB playback is heard |
| CC 118 | dry: >= 64, USB recording carries the FM-1's sound alone, before the master volume and without the USB playback (no feedback loop when the host processes it and plays it back) |
| CC 119 | bright button LED: 0 none, 1-14 = button 0-13 (same order as CC 102-115) |
| CC 120 | dim button LEDs, buttons 0-6: bit n = button n |
| CC 121 | dim button LEDs, buttons 7-13: bit n = button 7 + n |

In brick mode the host owns the 14 button LEDs. The LEDs only switch on and
off, so dim is a software PWM: on 1 ms in 8.

A first version of the audio switches changed every sample of the audio
interrupt; this one decides once per block, and with both switches off the
mix is FM1 Open's own.

## Code

- `app/screen.c`: `brick_input()` reads the panel before the pages do.
- `app/audio_output.c`: `fm1_brick_cc`, and mute / dry once per block in `fm1_output_mix()`.
- `app/midi_router.c`: stores CC 117-121 on channel 16 from any MIDI input.

License as FM1 Open: GPL-3.0-only with the FM1 SDK linking exception.
