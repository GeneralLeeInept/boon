#include "keyboard.h"

#include "irq.h"
#include "monitor.h"
#include "port.h"

#include <stdint.h>

namespace keyboard
{
static constexpr uint8_t IrqNo = 1;
static constexpr uint16_t Port = 0x60;

static constexpr char KeyMap[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  // Left control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0,  // Left shift
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,  // Right shift
    '*', 
    0,  // Left alt
    ' ',
    9,  // Caps lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1..F10
    0,  // NumLock
    0,  // ScrLk
    '7', '8', '9', '-',
    '4', '5', '6', '+',
    '1', '2', '3',
    '0', '.',
    0, 0, 0,    // Scan codes 0x54..0x56
    0, 0        // F11..F12
};

static constexpr uint8_t KeyMapCount = sizeof(KeyMap);

void Handler()
{
    uint8_t scanCode = inb(Port);

    if (scanCode < KeyMapCount && KeyMap[scanCode])
    {
        monitor::PrintChar(KeyMap[scanCode]);
    }
}

void Init()
{
    irq::InstallHandler(IrqNo, Handler);
}
}