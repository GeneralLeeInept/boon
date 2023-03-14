.section .text
.global DivideByZero
.type DivideByZero, @function
DivideByZero:
    /*
    pushal
    push %ds
    push %es
    mov $0x10, %ax
    mov %ds, %ax
    mov %es, %ax
    pushl $0x00
    call ExceptionHandler
    pop %ax
    pop %es
    pop %ds
    popal
    */
    cli
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    cld

    /* call ExceptionHandler */

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    iret
