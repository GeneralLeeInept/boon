#include <stdint.h>

static uint8_t sCursorRow;
static uint8_t sCursorCol;

static inline void koutb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port) );
}

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
    koutb(0x3D4, 0x0F);
    koutb(0x3D5, (uint8_t) (cursorPos & 0xFF));
    koutb(0x3D4, 0x0E);
    koutb(0x3D5, (uint8_t) ((cursorPos >> 8) & 0xFF));
}

struct BootInfo
{
    uint8_t cursorCol;
    uint8_t cursorRow;
} __attribute__((packed));

typedef struct BootInfo BootInfo;
BootInfo* _bootInfo = 0;

void kmain()
{
    sCursorCol = _bootInfo->cursorCol;
    sCursorRow = _bootInfo->cursorRow;
    kprint("Hello from kernel!\n");
}