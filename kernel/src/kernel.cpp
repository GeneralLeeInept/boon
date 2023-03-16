#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "port.h"

#include <stdint.h>

static uint8_t sCursorRow;
static uint8_t sCursorCol;

void kprint(const char* msg)
{
    static char* vram = (char*)0xB8000;

    for (int i = 0; msg && msg[i]; i++)
    {
        if (msg[i] == '\r')
        {
            sCursorCol = 0;
            continue;
        }

        if (msg[i] == '\n')
        {
            sCursorCol = 0;
            sCursorRow++;
            continue;
        }

        if (sCursorCol >= 80)
        {
            sCursorCol = 0;
            sCursorRow++;
        }

        if (sCursorRow >= 25)
        {
            return;
        }

        uint16_t cursorPos = sCursorRow * 80 + sCursorCol;
        vram[cursorPos * 2] = msg[i];
        sCursorCol++;
    }

    uint16_t cursorPos = sCursorRow * 80 + sCursorCol;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (cursorPos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((cursorPos >> 8) & 0xFF));
}

void kprintHex(uint8_t b)
{
    const char* hex="0123456789ABCDEF";
    char buf[3]{};
    buf[0] = hex[(b & 0xF0) >> 4];
    buf[1] = hex[b & 0x0F];
    kprint(buf);
}

struct BootInfo
{
    uint8_t cursorCol;
    uint8_t cursorRow;
} __attribute__((packed));

extern "C" void _init();

extern "C" void KernelInit(BootInfo* bootInfo)
{
    sCursorCol = bootInfo->cursorCol;
    sCursorRow = bootInfo->cursorRow;

    isr::init();
    idt::init();
    irq::init();
    asm volatile ("sti");

    // TODO: malloc / free support

    _init();
}

void keyhandler()
{
    kprint("#");
}

extern "C" void kmain()
{
    irq::installHandler(0x01, keyhandler);
    kprint("Hello from kernel!\n");

    for (;;) {}
}

extern "C" void __cxa_finalize(void*);
extern "C" void _fini();

extern "C" void KernelExit()
{
    __cxa_finalize(0);
    _fini();
    kprint("Exiting kernel.\n");
}
