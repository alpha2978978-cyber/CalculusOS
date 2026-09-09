#include "multiboot.h"

struct mb_tag {
    uint32_t type;
    uint32_t size;
};

struct mb_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t  bpp;
    uint8_t  fb_type;
    uint16_t reserved;
};

fb_info_t multiboot_get_framebuffer(uint32_t mb_info_addr) {
    fb_info_t fb;
    fb.valid = 0;

    uint32_t total_size = *(uint32_t *)(uintptr_t)mb_info_addr;
    uint8_t *ptr = (uint8_t *)(uintptr_t)(mb_info_addr + 8); /* skip size+reserved */
    uint8_t *end = (uint8_t *)(uintptr_t)(mb_info_addr + total_size);

    while (ptr < end) {
        struct mb_tag *tag = (struct mb_tag *)ptr;
        if (tag->type == 0) break;              /* end tag */

        if (tag->type == 8) {                    /* framebuffer tag */
            struct mb_tag_framebuffer *f = (struct mb_tag_framebuffer *)ptr;
            fb.addr   = f->addr;
            fb.pitch  = f->pitch;
            fb.width  = f->width;
            fb.height = f->height;
            fb.bpp    = f->bpp;
            fb.type   = f->fb_type;
            fb.valid  = 1;
            return fb;
        }

        /* tags are 8-byte aligned */
        ptr += (tag->size + 7) & ~7u;
    }
    return fb;
}
