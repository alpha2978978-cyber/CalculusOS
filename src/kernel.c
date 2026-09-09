/* ============================================================
 * CalculusOS - kernel.c
 * Entry point called from boot.s once GRUB has done its job.
 *
 * No applications live here on purpose - this is the driver +
 * login skeleton you asked for: VESA HD-color framebuffer driver,
 * (USB-legacy) keyboard driver, and a login screen. That's it.
 * ============================================================ */

#include <stdint.h>
#include "multiboot.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "login.h"

extern void gdt_init(void);
extern void idt_init(void);

void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    (void)magic; /* could check against 0x36d76289 if you want to be strict */

    gdt_init();
    idt_init();

    fb_info_t fb = multiboot_get_framebuffer(mb_info_addr);
    if (fb.valid) {
        fb_init(fb);
    }

    keyboard_init();

    if (fb.valid) {
        login_run();

        color_t bg = {10, 10, 20};
        color_t ok = {90, 230, 120};
        fb_put_string(40, 200, "system idle - no applications installed.", ok, bg);
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
