.global _start

.section .text.start
.type _start,@function
_start:
    /* Setup stack */
    movl $stack_top, %esp

	/* Save bootinfo addr */
	movl %ebx, _bootInfo

	sti
    call kmain

    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
stack_base:
    .skip 0x4000    /* 16KiB stack */
stack_top:
