OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS
{
    . = 0x10000;

    .text.start : { obj/start.o }

    .text : ALIGN(CONSTANT(MAXPAGESIZE)) {
        *(.text)
    }

    .rodata : ALIGN(CONSTANT(MAXPAGESIZE)) {
        *(.rodata)
    }

    .data : ALIGN(CONSTANT(MAXPAGESIZE)) {
        *(.data)
    }

    .bss : ALIGN(CONSTANT(MAXPAGESIZE)) {
        *(.bss)
    }
}