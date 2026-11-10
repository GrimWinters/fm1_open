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

Planned, host to FM-1: CC 117 (mute the FM-1's own sound locally) and CC 118
(USB recording without the USB playback, for effect mode). A first version
changed the audio interrupt for this and caused audible crackles, even with
brick mode off, so it was taken out; it will come back outside the audio
path.

## Code

- `app/screen.c`: `brick_input()` reads the panel before the pages do.
- Nothing else changes: the audio path is the same as in FM1 Open.

License as FM1 Open: GPL-3.0-only with the FM1 SDK linking exception.
