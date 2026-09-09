/* ============================================================
 * CalculusOS - login.c
 * A minimal text login screen drawn on the VESA framebuffer,
 * driven by the keyboard driver.
 *
 * Change CALC_USER / CALC_PASS below to set the credentials.
 * (There's no filesystem or user database yet - this is the
 * "everything ready to build" skeleton, not the finished OS.)
 * ============================================================ */

#include "login.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "string.h"

#define CALC_USER "admin"
#define CALC_PASS "calculus"
#define FIELD_MAX 32

static const color_t COL_BG     = {10, 10, 20};
static const color_t COL_TITLE  = {80, 200, 255};
static const color_t COL_TEXT   = {230, 230, 230};
static const color_t COL_PROMPT = {150, 150, 160};
static const color_t COL_ERR    = {255, 80, 80};
static const color_t COL_OK     = {90, 230, 120};

static void read_line(uint32_t x, uint32_t y, char *out, int mask) {
    int len = 0;
    out[0] = '\0';

    for (;;) {
        /* redraw the field each keystroke (simple, no cursor logic needed) */
        char shown[FIELD_MAX + 1];
        for (int i = 0; i < len; i++) shown[i] = mask ? '*' : out[i];
        shown[len] = '\0';
        fb_put_string(x, y, "                                ", COL_TEXT, COL_BG); /* clear */
        fb_put_string(x, y, shown, COL_TEXT, COL_BG);

        char c = keyboard_getchar();

        if (c == '\n') {
            out[len] = '\0';
            return;
        } else if (c == '\b') {
            if (len > 0) len--;
        } else if (len < FIELD_MAX && c >= 32 && c < 127) {
            out[len++] = c;
        }
    }
}

void login_run(void) {
    char user[FIELD_MAX + 1];
    char pass[FIELD_MAX + 1];

    for (;;) {
        fb_clear(COL_BG);
        fb_put_string(40, 40, "CalculusOS", COL_TITLE, COL_BG);
        fb_put_string(40, 60, "login system", COL_PROMPT, COL_BG);

        fb_put_string(40, 100, "username:", COL_PROMPT, COL_BG);
        read_line(160, 100, user, 0);

        fb_put_string(40, 120, "password:", COL_PROMPT, COL_BG);
        read_line(160, 120, pass, 1);

        if (k_strcmp(user, CALC_USER) == 0 && k_strcmp(pass, CALC_PASS) == 0) {
            fb_put_string(40, 160, "login OK - welcome to CalculusOS", COL_OK, COL_BG);
            return;
        } else {
            fb_put_string(40, 160, "invalid credentials, try again...", COL_ERR, COL_BG);
            for (volatile int i = 0; i < 30000000; i++) { } /* crude delay */
        }
    }
}
