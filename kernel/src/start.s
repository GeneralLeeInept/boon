    .code32
    .section .text.start

    .global _start
_start:
    movl $stack, %esp
    call kmain

    .bss
    .align 32
stack_base:
    .space 0x4000
stack:
