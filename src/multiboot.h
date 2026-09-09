#ifndef CALCULUSOS_MULTIBOOT_H
#define CALCULUSOS_MULTIBOOT_H

#include <stdint.h>

typedef struct {
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  type;
    uint8_t  valid;
} fb_info_t;

/* Walks the multiboot2 info structure GRUB hands us in ebx and
 * pulls out the framebuffer tag (type 8) it set up for us. */
fb_info_t multiboot_get_framebuffer(uint32_t mb_info_addr);

#endif
