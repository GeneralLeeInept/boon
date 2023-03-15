#include "port.h"

#include <stdint.h>

// PIC1 I/O ports
static constexpr uint16_t   PIC1_BASE = 0x20;
static constexpr uint16_t   PIC1_COMMAND = PIC1_BASE;
static constexpr uint16_t   PIC1_DATA = PIC1_BASE + 1;

// PIC2 I/O ports
static constexpr uint16_t   PIC2_BASE = 0xA0;
static constexpr uint16_t   PIC2_COMMAND = PIC2_BASE;
static constexpr uint16_t   PIC2_DATA = PIC2_BASE + 1;

// PIC commands
static constexpr uint8_t    ICW1_ICW4       = 0x01;
static constexpr uint8_t    ICW1_SINGLE     = 0x02;
static constexpr uint8_t    ICW1_INTERVAL4  = 0x04;
static constexpr uint8_t    ICW1_LEVEL      = 0x08;
static constexpr uint8_t    ICW1_INIT       = 0x10;

static constexpr uint8_t    ICW4_8086       = 0x01;
static constexpr uint8_t    ICW4_AUTO       = 0x02;
static constexpr uint8_t    ICW4_BUF_SLAVE  = 0x04;
static constexpr uint8_t    ICW4_BUF_MASTER = 0x08;
static constexpr uint8_t    ICW4_SFNM       = 0x10;

static constexpr uint8_t    PIC_EOI         = 0x20;

static void PIC_Remap()
{
    uint8_t pic1Mask = inb(PIC1_DATA);
    uint8_t pic2Mask = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, 0xff);
    // outb(PIC2_DATA, pic2Mask);
    io_wait();

    pic1Mask = inb(PIC1_DATA);
    pic2Mask = inb(PIC2_DATA);
}

static void PIC_SendEOI(uint8_t irq)
{
    if (irq > 7)
    {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_Init()
{
    PIC_Remap();

}