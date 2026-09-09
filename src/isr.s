.global idt_load
idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

.extern irq_dispatch

.global irq1
irq1:
    pusha
    push $1
    call irq_dispatch
    add $4, %esp
    popa
    iret
