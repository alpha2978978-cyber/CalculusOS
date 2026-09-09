#ifndef CALCULUSOS_FRAMEBUFFER_H
#define CALCULUSOS_FRAMEBUFFER_H

#include <stdint.h>
#include "multiboot.h"

typedef struct {
    uint32_t r, g, b;
} color_t;

void fb_init(fb_info_t info);
void fb_clear(color_t color);
void fb_put_pixel(uint32_t x, uint32_t y, color_t color);
void fb_put_char(uint32_t x, uint32_t y, char c, color_t fg, color_t bg);
void fb_put_string(uint32_t x, uint32_t y, const char *s, color_t fg, color_t bg);
uint32_t fb_width(void);
uint32_t fb_height(void);

#endif
