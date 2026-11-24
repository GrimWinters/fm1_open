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

Notes from the keys go out over USB in brick mode (see CC 122 below).

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
| CC 122 | keys play locally: >= 64, the note keys play the FM-1's own sound as well; below, they only go out as MIDI |
| CC 85 | scale, pitch classes C-F#: bit n = the note n semitones above C belongs to the scale |
| CC 86 | scale, pitch classes G-B: bit n = the note 7 + n semitones above C |
| CC 87 | scale root: 0 = off (the keys light as usual), 1-12 = C-B |
| CC 116 | 127 hello: an FM-1 in brick mode answers with CC 116 = 127 (so the host finds it whichever started first); 64 every second while the host runs; 0 when it quits. The Brickwave Edition shows WAITING FOR HOST and turns the host's LEDs off after a goodbye or 3.5 s without one |

In brick mode the note keys always go out over USB, whatever KEY OUT, OUT
PORT and LOCAL say; CC 122 takes LOCAL's place. Leaving brick mode restores
the MIDI page settings.

In brick mode the host owns the 14 button LEDs. The LEDs only switch on and
off, so dim is a software PWM: on 1 ms in 8.

With a scale set (CC 87 not 0), the note keys light it: the root bright,
the scale's other notes dim, a pressed key bright (the Brickwave Edition: the scale lit, its root blinking, a pressed key flipped; its panel loop is too slow to dim). Each key's note comes
from the FM-1's own octave and track, so the lights follow its keyboard.

A first version of the audio switches changed every sample of the audio
interrupt; this one decides once per block, and with both switches off the
mix is FM1 Open's own.

## Brickwave Edition

`build.bat --brickwave` builds a firmware that is only a controller and sound
card for Brickwave (`app/brickwave.c`). It starts in brick mode and stays
there; the screen shows whether a host is connected. The FM-1's own
instrument is linked (the SDK needs it) but always silent; its pages,
projects, presets, samples, DX7 library and workbench transfers are left out
(`#ifndef FM1_BRICKWAVE` around those files). The master knob still sets
the volume. Keys send notes on channel 1 (F3-G5, velocity 100). Bluetooth
audio from a phone is played and is also the only thing in the USB
recording, so Brickwave's effect mode has an input and no feedback loop.
To go back, flash a normal build (or the original V15) the same way.

Its screen shows what Brickwave's knobs K1-K4 turn: the page, each
parameter's name, value and bar in its slot colour (the selected one marked
on the left), and a status line (preset, key, motion count-in / REC).
Brickwave sends it as SysEx when it changes, and every two seconds:
`F0 7D 'B' 'W' 01`, page name, 0, status line, 0, then per knob: flags
(1 used, 2 inactive, 4 bipolar, 8 selected, 16 a list line without bar), bar 0-127, zero 0-127, colour
RGB565 in three 7-bit bytes (bits 0-6, 7-13, 14-15), name, 0, value, 0; `F7`.
Text is ASCII, names and values up to 16 characters; at most 192 bytes. On the preset list the four lines are the highlighted slot and the three after it.

## Code

- `app/screen.c`: `brick_input()` reads the panel before the pages do.
- `app/brickwave.c`: the Brickwave Edition's whole panel and screen.
- `app/audio_output.c`: `fm1_brick_cc`, and mute / dry once per block in `fm1_output_mix()`.
- `app/midi_router.c`: stores CC 117-122 and 85-87 on channel 16 from any MIDI input; in brick mode the keys follow CC 122.

License as FM1 Open: GPL-3.0-only with the FM1 SDK linking exception.
