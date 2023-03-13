ENTRY(_start)
SECTIONS
{
    . = 0x7C00;

    .text : {
        *(.text)
        FILL(0)
        . = 0x1FE;
        SHORT(0xAA55)
    }    
}