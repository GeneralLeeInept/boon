.global _start

.section .text.start
.type _start,@function
_start:
    /* Zero BSS - TODO: Is this needed? */
    call ClearBss

    /* Setup stack */
    movl $stack_top, %esp

    push %ebx
    call KernelInit
    add $0x4, %esp

	sti
    call kmain

    call KernelExit

epilog:
    cli
1:  hlt
    jmp 1b

.type ClearBss,@function
ClearBss:
    movl $ebss, %eax
    subl $sbss, %eax
    mov %eax, %ecx
    movl $sbss, %edi
    movl $0, %eax

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

1:  ret

.section .bss
.align 16
stack_base:
    .skip 0x4000    /* 16KiB stack */
stack_top:
