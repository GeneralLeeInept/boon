/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 
/* 
Declare a multiboot header that marks the program as a kernel. These are magic
values that are documented in the multiboot standard. The bootloader will
search for this signature in the first 8 KiB of the kernel file, aligned at a
32-bit boundary. The signature is in its own section so the header can be
forced to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.global _start
.section .text.start
.type _start,@function
_start:
    /* Setup stack */
    movl $stack_top, %esp
    and $-16, %esp
    push $0
    push $0
 
	lgdt gdtinfo_pm
	ljmp $0x8,$start_kernel

start_kernel:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    push %ebx
    call KernelInit
    add $0x4, %esp

    call kmain

    call KernelExit

epilog:
    cli
1:  hlt
    jmp 1b

.size _start, . - _start

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

.p2align 2
gdt_pm:
.nulldesc:
	.quad 0
.codedesc:
	.word 0xffff		/* Limit bits 0..15 */
	.word 0x0000		/* Base bits 0..15 */
	.byte 0x00			/* Base bits 16..23 */
	.byte 0b10011010	/* Access: P=1, DPL=00, S=1, E=1, DC=0, RW=1, A=0 */
	.byte 0b11001111	/* Low nibble: Limit bits 16..19; High nibble Flags: G=1, DB=1, L=0, Reserved=0 */
	.byte 0x00			/* Base bits 24..31 */
.datadesc:
	.word 0xffff
	.word 0x0000
	.byte 0x00
	.byte 0b10010010
	.byte 0b11001111
	.byte 0x00
gdt_pm_end:

gdtinfo_pm:
	.word gdt_pm_end - gdt_pm - 1
	.long gdt_pm

.section .bss
.align 16
stack_base:
    .skip 0x4000    /* 16KiB stack */
stack_top:
