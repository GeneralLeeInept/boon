    .code16
    .org 0
    .text
    .global _start

_start:
    mov %cs, %ax
    mov %ax, %ds
    movw $sector2_msg, %si

.print:
	xor %bx, %bx
	movb $0x0E, %ah

	lodsb
	cmpb $0, %al
	je .stop

	int $0x10
	jmp .print

.stop:
    hlt
    jmp .stop

sector2_msg:
    .string "Hello from 0x10000\r\n"
