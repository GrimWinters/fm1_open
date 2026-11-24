/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception */
/* Brickwave Edition (build.bat --brickwave): the FM-1 as a controller and
 * sound card for Brickwave on the TrimUI Brick, nothing else. It starts in
 * brick mode (brick_mode.md) and stays there: encoders, buttons and keys go
 * to the host over USB MIDI, the host drives the LEDs, the FM-1 plays the
 * host's audio. The FM-1's own instrument is linked (the SDK needs it) but
 * silent; its pages, projects, presets, samples and DX7 library are left
 * out. Bluetooth audio (a phone) is mixed in and also goes to the host as
 * its input, for Brickwave's effect mode. */
#ifdef FM1_BRICKWAVE
#include "instrument.h"
#include "audio_output.h"
#include "fm_patch.h"
#include "version.h"
#include <string.h>

/* ---- What the SDK expects from the parts left out ---------------------- */
static struct groove_project scratch;
void groove_project_init(void) {}
void groove_project_poll(void) {}
void groove_project_name(char name[13]) { memcpy(name, "BRICKWAVE   ", 12); name[12] = 0; }
void groove_project_metadata(const struct groove_project *p) { (void)p; }
struct groove_project *groove_project_scratch(void) { return &scratch; }
void groove_samples_init(void) {}
void groove_samples_poll(void) {}
void groove_transfer_init(void) {}
void groove_transfer_poll(void) {}
void groove_fm_library_init(void) {}
FM1_DSP_CODE void groove_fm_program(uint8_t patch[156], unsigned program)
{
    (void)program;
    for (unsigned i = 0; i < 156; ++i) patch[i] = 0;
}
/* fm1_app_sysex: in app/transfer.c, which the build compiles without LTO
 * (LTO drops application callbacks that the SDK has weak defaults for). */
/* app/brickwave_keep.c: the SDK parts the normal firmware uses, kept linked. */
extern const void *const brickwave_keep[];
extern const unsigned brickwave_keep_count;

/* ---- Screen: a few lines of large text ------------------------------- */
enum { BACK = 0x0000, TEXT = 0xef5d, DIM = 0x738e, ACCENT = 0x07ff, AMBER = 0xfd20 };
/* Five columns per glyph, bit 0 at the top: digits, then A-Z. */
static const uint8_t font[36][5] = {
    {0x3e, 0x51, 0x49, 0x45, 0x3e}, {0, 0x42, 0x7f, 0x40, 0},       {0x42, 0x61, 0x51, 0x49, 0x46},
    {0x21, 0x41, 0x45, 0x4b, 0x31}, {0x18, 0x14, 0x12, 0x7f, 0x10}, {0x27, 0x45, 0x45, 0x45, 0x39},
    {0x3c, 0x4a, 0x49, 0x49, 0x30}, {0x01, 0x71, 0x09, 0x05, 0x03}, {0x36, 0x49, 0x49, 0x49, 0x36},
    {0x06, 0x49, 0x49, 0x29, 0x1e}, {0x7e, 0x11, 0x11, 0x11, 0x7e}, {0x7f, 0x49, 0x49, 0x49, 0x36},
    {0x3e, 0x41, 0x41, 0x41, 0x22}, {0x7f, 0x41, 0x41, 0x22, 0x1c}, {0x7f, 0x49, 0x49, 0x49, 0x41},
    {0x7f, 0x09, 0x09, 0x09, 0x01}, {0x3e, 0x41, 0x49, 0x49, 0x7a}, {0x7f, 0x08, 0x08, 0x08, 0x7f},
    {0, 0x41, 0x7f, 0x41, 0},       {0x20, 0x40, 0x41, 0x3f, 0x01}, {0x7f, 0x08, 0x14, 0x22, 0x41},
    {0x7f, 0x40, 0x40, 0x40, 0x40}, {0x7f, 0x02, 0x0c, 0x02, 0x7f}, {0x7f, 0x04, 0x08, 0x10, 0x7f},
    {0x3e, 0x41, 0x41, 0x41, 0x3e}, {0x7f, 0x09, 0x09, 0x09, 0x06}, {0x3e, 0x41, 0x51, 0x21, 0x5e},
    {0x7f, 0x09, 0x19, 0x29, 0x46}, {0x46, 0x49, 0x49, 0x49, 0x31}, {0x01, 0x01, 0x7f, 0x01, 0x01},
    {0x3f, 0x40, 0x40, 0x40, 0x3f}, {0x1f, 0x20, 0x40, 0x20, 0x1f}, {0x3f, 0x40, 0x38, 0x40, 0x3f},
    {0x63, 0x14, 0x08, 0x14, 0x63}, {0x07, 0x08, 0x70, 0x08, 0x07}, {0x61, 0x51, 0x49, 0x45, 0x43}};
static uint16_t glyph[6 * 4 * 7 * 4];   /* one character at scale 4 */

/* One line of text, left-aligned at x (centred if x is ~0U), cleared to the
 * right edge. Characters other than digits, letters, '-' and '.' are spaces. */
static void line(unsigned x, unsigned y, const char *s, unsigned scale, uint16_t color)
{
    const unsigned w = 6U * scale, h = 7U * scale, n = (unsigned)strlen(s);
    if (x == ~0U) x = n * w < FM1_SCREEN_WIDTH ? (FM1_SCREEN_WIDTH - n * w) / 2U : 0;
    fm1_screen_fill(0, y, FM1_SCREEN_WIDTH, h, BACK);
    for (; *s && x + w <= FM1_SCREEN_WIDTH; ++s, x += w) {
        static const uint8_t dash[5] = {8, 8, 8, 8, 8}, dot[5] = {0, 0x60, 0x60, 0, 0}, blank[5] = {0};
        const char c = *s >= 'a' && *s <= 'z' ? (char)(*s - 32) : *s;
        const uint8_t *shape = c >= '0' && c <= '9' ? font[c - '0']
                             : c >= 'A' && c <= 'Z' ? font[10 + c - 'A'] : c == '-' ? dash : c == '.' ? dot : blank;
        for (unsigned r = 0; r < h; ++r)
            for (unsigned col = 0; col < w; ++col)
                glyph[r * w + col] = col / scale < 5U && (shape[col / scale] >> (r / scale) & 1U) ? color : BACK;
        (void)fm1_screen_blit(x, y, w, h, glyph, sizeof glyph / sizeof glyph[0]);
    }
}

/* ---- Brick mode ------------------------------------------------------- */
enum { KEYS = 27, BASE_NOTE = 53 };   /* the keys are F3..G5 */
static uint32_t last_keys, last_buttons, key_leds, hello_at, host_at, pass_at, pass_x16, shown_at;
static unsigned pass_count;   /* passes since start (for diagnostics) */
static uint16_t button_leds;
static unsigned host_seen, shown;

static void send(uint8_t status, uint8_t a, uint8_t b)
{
    const uint8_t m[3] = {status, a, b};
    (void)fm1_midi_send_port(FM1_MIDI_USB, m, 3);
}

static void show(unsigned state)
{
    if (state == shown) return;
    shown = state;
    line(~0U, 60, "BRICKWAVE", 4, ACCENT);
    line(~0U, 110, "FM-1 CONTROLLER", 2, DIM);
    line(~0U, 160, state == 2 ? "CONNECTED" : "WAITING FOR HOST", 2, state == 2 ? TEXT : AMBER);
}

void groove_screen_init(void)
{
    /* Read the table once, so the linker cannot drop it (and what it lists). */
    for (unsigned i = 0; i < brickwave_keep_count; ++i)
        (void)*(const void *const volatile *)&brickwave_keep[i];
    fm1_controls_init();
    for (unsigned k = 0; k < 41; ++k) fm1_led_set(k, 0);
    fm1_screen_init();
    fm1_screen_fill(0, 0, FM1_SCREEN_WIDTH, FM1_SCREEN_HEIGHT, BACK);
    memset((void *)fm1_brick_cc, 0, sizeof fm1_brick_cc);
    fm1_brick_cc[0] = 127;   /* the FM-1's own instrument stays silent */
    fm1_brick_on = 1;
    last_keys = last_buttons = key_leds = 0;
    button_leds = 0;
    host_seen = shown = 0;
    hello_at = host_at = pass_at = shown_at = fm1_millis();
    pass_x16 = 16U;
    pass_count = 0;
    (void)fm1_bluetooth_configure(FM1_BT_AUDIO);   /* a phone can stream in */
    show(1);
}

void groove_screen_control(void)
{
    struct fm1_controls c;
    fm1_controls_read(&c);
    const uint32_t now = fm1_millis();
    (void)groove_master_update(c.knob);   /* the master knob: the FM-1's volume */

    /* The host: CC 116 127 hello (answer: brick mode is on), 64 every second
     * while it runs, 0 when it quits. Gone (goodbye, or 3.5 s of silence):
     * its LEDs go dark and we ask every two seconds again. */
    const unsigned msg = fm1_brick_cc[9];
    fm1_brick_cc[9] = 0;
    if (msg == 128) send(0xbf, 116, 127);
    if (msg > 1) { host_seen = 1; host_at = now; }
    if (host_seen && (msg == 1 || now - host_at >= 3500U)) {
        host_seen = 0;
        for (unsigned i = 2; i < 9; ++i) fm1_brick_cc[i] = 0;
    }
    if (!host_seen && now - hello_at >= 2000U) {
        hello_at = now;
        send(0xbf, 116, 127);
    }
    fm1_brick_cc[0] = 127;
    show(host_seen ? 2 : 1);

    for (unsigned i = 0; i < 7; ++i) {   /* encoders, relative: 64 = no change */
        int s = c.encoders[i];
        if (!s) continue;
        s = s > 63 ? 63 : s < -63 ? -63 : s;
        send(0xbf, (uint8_t)(20 + i), (uint8_t)(64 + s));
    }
    for (unsigned b = 0, x = c.buttons ^ last_buttons; x; ++b, x >>= 1)
        if (x & 1U) send(0xbf, (uint8_t)(102 + b), (c.buttons >> b & 1U) ? 127 : 0);
    last_buttons = c.buttons;
    for (unsigned k = 0, x = c.keys ^ last_keys; x && k < KEYS; ++k, x >>= 1)
        if (x & 1U) send((c.keys >> k & 1U) ? 0x90 : 0x80, (uint8_t)(BASE_NOTE + k), (c.keys >> k & 1U) ? 100 : 0);
    last_keys = c.keys;

    /* How long one pass takes (smoothed, ms x 16), shown on the screen. */
    pass_x16 += (now - pass_at) - pass_x16 / 16U;
    pass_at = now;
    if (now - shown_at >= 1000U) {
        shown_at = now;
        char t[16] = "LOOP 00.0 MS";
        const unsigned tenths = (pass_x16 * 10U + 8U) / 16U;
        t[5] = (char)('0' + tenths / 100U % 10U);
        t[6] = (char)('0' + tenths / 10U % 10U);
        t[8] = (char)('0' + tenths % 10U);
        if (t[5] == '0') t[5] = ' ';
        line(~0U, 214, t, 1, DIM);
    }

    /* LEDs, only on or off: a pass takes ~19 ms, too slow to dim them without
     * visible flicker. Buttons: the bright one (CC 119) lights; the dim mask
     * (CC 120-121) is not shown. Keys: the host's scale (CC 85-87) lights,
     * its root blinks once a second, a pressed key flips its light. */
    ++pass_count;
    unsigned on = fm1_brick_cc[2] ? 1U << (fm1_brick_cc[2] - 1U) : 0;
    on &= 0x3fffU;
    for (unsigned k = 0, x = on ^ button_leds; x; ++k, x >>= 1)
        if (x & 1U) fm1_led_set(k, on >> k & 1U);
    button_leds = (uint16_t)on;
    uint32_t keys = 0;
    if (fm1_brick_cc[8]) {
        const unsigned scale = fm1_brick_cc[6] | fm1_brick_cc[7] << 7, root = fm1_brick_cc[8] - 1U;
        const unsigned blink = now % 1000U < 650U;
        for (unsigned k = 0; k < KEYS; ++k) {
            const unsigned pc = (BASE_NOTE + k) % 12U;
            if (pc == root ? blink : (scale >> pc & 1U)) keys |= 1U << k;
        }
    }
    keys ^= c.keys;
    for (unsigned k = 0, x = keys ^ key_leds; x; ++k, x >>= 1)
        if (x & 1U) fm1_led_set(k + 14, keys >> k & 1U);
    key_leds = keys;
}
#endif
