/* ============================================================
 * CalculusOS - keyboard.c
 * The "USB keyboard" driver.
 *
 * IMPORTANT NOTE ON USB:
 * Almost every PC BIOS/UEFI has a feature called "USB Legacy
 * Support" (on by default) which makes the USB host controller
 * present any attached USB keyboard/mouse to software as if it
 * were a classic PS/2 device on I/O ports 0x60 / 0x64. This is
 * exactly how every hobby OS (and DOS, and BIOS setup screens)
 * "supports" USB keyboards without writing an xHCI/UHCI/EHCI
 * driver. That's what this driver talks to.
 *
 * A *native* USB HID driver (real xHCI ring buffers, device
 * enumeration, HID report parsing) is a huge subsystem on its
 * own - see usb_stub.c for where that would plug in later.
 * ============================================================ */

#include "keyboard.h"
#include "idt.h"
#include "io.h"
#include <stdint.h>

#define KBD_DATA_PORT   0x60
#define KBD_BUF_SIZE    256

static volatile char buf[KBD_BUF_SIZE];
static volatile uint32_t buf_head = 0;
static volatile uint32_t buf_tail = 0;
static uint8_t shift_down = 0;

/* US QWERTY scancode set 1 -> ASCII (unshifted) */
static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
};

static const char scancode_ascii_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,  'A','S','D','F','G','H','J','K','L',':','"','~',
    0,  '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' ', 0,
};

static void keyboard_irq_handler(void) {
    uint8_t scancode = inb(KBD_DATA_PORT);

    if (scancode == 0x2A || scancode == 0x36) { shift_down = 1; return; }        /* shift down */
    if (scancode == 0xAA || scancode == 0xB6) { shift_down = 0; return; }        /* shift up   */
    if (scancode & 0x80) return;                                                /* other key-up */

    char c = shift_down ? scancode_ascii_shift[scancode] : scancode_ascii[scancode];
    if (!c) return;

    uint32_t next = (buf_head + 1) % KBD_BUF_SIZE;
    if (next != buf_tail) {
        buf[buf_head] = c;
        buf_head = next;
    }
}

void keyboard_init(void) {
    irq_install_handler(1, keyboard_irq_handler);
}

char keyboard_getchar(void) {
    while (buf_head == buf_tail) {
        __asm__ volatile ("hlt"); /* sleep until next interrupt */
    }
    char c = buf[buf_tail];
    buf_tail = (buf_tail + 1) % KBD_BUF_SIZE;
    return c;
}
