ENTRY(_start)
SECTIONS
{
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.text.start)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
}