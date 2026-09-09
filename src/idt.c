/* CalculusOS - idt.c : Interrupt Descriptor Table + PIC remap + IRQ dispatch */
#include "idt.h"
#include "io.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static void (*irq_handlers[16])(void) = { 0 };

extern void idt_load(uint32_t);
extern void irq1(void); /* keyboard ISR stub, defined in isr.s */

void idt_set_gate(uint8_t num, uint32_t handler, uint16_t selector, uint8_t flags) {
    idt[num].base_low  = handler & 0xFFFF;
    idt[num].base_high = (handler >> 16) & 0xFFFF;
    idt[num].selector   = selector;
    idt[num].zero        = 0;
    idt[num].flags       = flags;
}

static void pic_remap(void) {
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20);   /* master PIC vector offset -> 0x20 */
    outb(0xA1, 0x28);   /* slave  PIC vector offset -> 0x28 */
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);    /* unmask everything for now */
    outb(0xA1, 0x0);
}

void irq_install_handler(int irq, void (*handler)(void)) {
    irq_handlers[irq] = handler;
}

/* Called from the assembly IRQ stub in isr.s */
void irq_dispatch(int irq) {
    if (irq_handlers[irq]) irq_handlers[irq]();

    if (irq >= 8) outb(0xA0, 0x20); /* EOI to slave  */
    outb(0x20, 0x20);               /* EOI to master */
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) idt_set_gate(i, 0, 0, 0);

    pic_remap();

    idt_set_gate(0x21, (uint32_t)irq1, 0x08, 0x8E); /* IRQ1 = keyboard */

    idt_load((uint32_t)&idtp);
    __asm__ volatile ("sti");
}
