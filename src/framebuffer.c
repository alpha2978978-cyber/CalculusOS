/* ============================================================
 * CalculusOS - framebuffer.c
 * The "VESA HD color" driver.
 *
 * GRUB (via the multiboot2 framebuffer tag in boot.s) already
 * asked the VESA BIOS to switch the card into a linear-framebuffer
 * true-color mode before our kernel ever runs. This file just
 * takes the resulting physical address / pitch / bpp and gives us
 * pixel and text drawing primitives on top of it.
 * ============================================================ */

#include "framebuffer.h"
#include "font8x8.h"

static uint8_t  *fb_addr;
static uint32_t  fb_pitch;
static uint32_t  fb_w;
static uint32_t  fb_h;
static uint8_t   fb_bpp;

void fb_init(fb_info_t info) {
    fb_addr  = (uint8_t *)(uintptr_t)info.addr;
    fb_pitch = info.pitch;
    fb_w     = info.width;
    fb_h     = info.height;
    fb_bpp   = info.bpp;
}

uint32_t fb_width(void)  { return fb_w; }
uint32_t fb_height(void) { return fb_h; }

void fb_put_pixel(uint32_t x, uint32_t y, color_t color) {
    if (x >= fb_w || y >= fb_h) return;
    uint32_t bytes_per_pixel = fb_bpp / 8;
    uint8_t *pixel = fb_addr + y * fb_pitch + x * bytes_per_pixel;

    if (bytes_per_pixel == 4) {
        pixel[0] = (uint8_t)color.b;
        pixel[1] = (uint8_t)color.g;
        pixel[2] = (uint8_t)color.r;
        pixel[3] = 0;
    } else if (bytes_per_pixel == 3) {
        pixel[0] = (uint8_t)color.b;
        pixel[1] = (uint8_t)color.g;
        pixel[2] = (uint8_t)color.r;
    } else if (bytes_per_pixel == 2) {
        uint16_t val = (uint16_t)(((color.r >> 3) << 11) |
                                   ((color.g >> 2) << 5)  |
                                   (color.b >> 3));
        *(uint16_t *)pixel = val;
    }
}

void fb_clear(color_t color) {
    for (uint32_t y = 0; y < fb_h; y++)
        for (uint32_t x = 0; x < fb_w; x++)
            fb_put_pixel(x, y, color);
}

void fb_put_char(uint32_t x, uint32_t y, char c, color_t fg, color_t bg) {
    if (c < FONT_FIRST_CHAR || c >= FONT_LAST_CHAR) c = ' ';
    const uint8_t *glyph = font8x8_basic[(int)c - FONT_FIRST_CHAR];

    for (int row = 0; row < 8; row++) {
        uint8_t bits = glyph[row];
        for (int col = 0; col < 8; col++) {
            color_t px = (bits & (1 << col)) ? fg : bg;
            fb_put_pixel(x + col, y + row, px);
        }
    }
}

void fb_put_string(uint32_t x, uint32_t y, const char *s, color_t fg, color_t bg) {
    uint32_t cx = x, cy = y;
    for (; *s; s++) {
        if (*s == '\n') {
            cx = x;
            cy += 10;
            continue;
        }
        fb_put_char(cx, cy, *s, fg, bg);
        cx += 8;
    }
}
