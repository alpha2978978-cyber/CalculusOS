#ifndef CALCULUSOS_IDT_H
#define CALCULUSOS_IDT_H

#include <stdint.h>

void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t handler, uint16_t selector, uint8_t flags);
void irq_install_handler(int irq, void (*handler)(void));

#endif
