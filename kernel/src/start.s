.global _start

.section .text.start
.type _start,@function
_start:
    /* Zero BSS */
    movl $ebss, %eax
    subl $sbss, %eax
    mov %eax, %ecx
    movl $sbss, %edi
    movl $0xcdcdcdcd, %eax
.clearl_loop:
    cmp $8, %ecx
    jl .clearb_loop
    movl %eax, %es:(%edi)
    sub $8, %ecx
    add $8, %edi
    jmp .clearl_loop
    rep stosb
.clearb_loop:
    cmp $0, %ecx
    je 1f
    movb %al, %es:(%edi)
    dec %ecx
    inc %edi
    jmp .clearb_loop
1:

    /* Setup stack */
    movl $stack_top, %esp

	/* Save bootinfo addr */
	movl %ebx, _bootInfo

	sti
    call kmain
    leave

    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
stack_base:
    .skip 0x4000    /* 16KiB stack */
stack_top:
