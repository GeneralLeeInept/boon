#include "irq.h"

#include "isr.h"
#include "port.h"

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

static constexpr uint8_t    IRQ_BASE        = 0x20;

static void PIC_Remap()
{
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, IRQ_BASE);
    io_wait();
    outb(PIC2_DATA, IRQ_BASE + 8);
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

static void PIC_SetMask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

static void PIC_ClearMask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;
 
    if(irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

namespace irq
{
static handler s_handlers[16]{};

static void stub(int intNo, int err)
{
    (void)(err);

    if (intNo >= IRQ_BASE && (intNo - IRQ_BASE) < 16)
    {
        int irq = intNo - IRQ_BASE;

        if (s_handlers[irq])
        {
            s_handlers[irq]();
        }
    }

    if (intNo >= IRQ_BASE+8)
    {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void init()
{
    PIC_Remap();

    for (uint8_t i = 0; i < 16; ++i)
    {
        isr::installHandler(i + IRQ_BASE, stub);
    }
}

void installHandler(uint8_t irq, handler handler)
{
    asm volatile ("cli");
    s_handlers[irq] = handler;
    PIC_ClearMask(irq);
    asm volatile ("sti");
}
}