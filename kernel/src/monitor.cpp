#include "monitor.h"

#include "bootparams.h"
#include "port.h"

namespace monitor
{
static constexpr uint8_t VgaWidth = 80;
static constexpr uint8_t VgaHeight = 25;

static uint16_t* s_vram = (uint16_t*)0xB8000;
static uint8_t s_cursorX;
static uint8_t s_cursorY;

static void UpdateCursor()
{
    uint16_t cursorPos = s_cursorY * VgaWidth + s_cursorX;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (cursorPos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((cursorPos >> 8) & 0xFF));
}

static void Scroll()
{
    if (s_cursorY < VgaHeight)
    {
        return;
    }

    while (s_cursorY >= VgaHeight)
    {
        for (int row = 0; row < VgaHeight - 1; row++)
        {
            for (int col = 0; col < VgaWidth; ++col)
            {
                uint16_t destPos = row * VgaWidth + col;
                uint16_t srcPos = (row + 1) * VgaWidth + col;
                s_vram[destPos] = s_vram[srcPos];
            }
        }

        s_cursorY--;
    }

    static constexpr uint16_t blank = 0x0720;

    for (int col = 0; col < VgaWidth; ++col)
    {
        uint16_t pos = (VgaHeight - 1) * VgaWidth + col;
        s_vram[pos] = blank;
    }
}

void Init(const BootParams& bootParams)
{
    s_cursorX = bootParams.cursorColumn;
    s_cursorY = bootParams.cursorRow;
}

void PrintChar(char c)
{
    static constexpr uint16_t attribute = 0x0700;   // white on black

    if (c == '\r')
    {
        s_cursorX = 0;
    }
    else if (c == '\n')
    {
        s_cursorX = 0;
        s_cursorY++;
    }
    else if (c == '\t')
    {
        s_cursorX = (s_cursorX + 4) & ~3;
    }
    else if (c >= ' ')
    {
        uint16_t cursorPos = s_cursorY * VgaWidth + s_cursorX;
        s_vram[cursorPos] = attribute | c;

        s_cursorX++;
    }

    if (s_cursorX >= VgaWidth)
    {
        s_cursorX = 0;
        s_cursorY++;
    }

    Scroll();
    UpdateCursor();
}

void Print(const char* msg)
{
    for (int i = 0; msg && msg[i]; i++)
    {
        PrintChar(msg[i]);
    }
}

void PrintHex(uint8_t byte)
{
    const char* hex="0123456789ABCDEF";
    PrintChar((char)((byte & 0xF0) >> 4));
    PrintChar((char)(byte & 0x0F));
}

void PrintNum(int num)
{
    // TODO:
    ((void)num);
}
}