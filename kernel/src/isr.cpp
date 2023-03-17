#include "isr.h"

#include "idt.h"
#include "monitor.h"

namespace isr
{
static constexpr int handlers_count = 48;   // 32 exceptions + 16 IRQs

static Handler s_handlers[handlers_count];
extern "C" uint32_t _isr_stub_table[];

void Exception(int code, int error)
{
    monitor::Print("Exception 0x");
    monitor::PrintHex(code);
    monitor::Print(" (error = 0x");
    monitor::PrintHex(error);
    monitor::Print(")\n");

    asm volatile ("hlt");
}

extern "C" void RouteIsr()
{
    // Call this from the isr stubs
    /* Stack:
        ebp + 0x3c: error
        ebp + 0x38: code
        ebp + 0x34: eax
        ebp + 0x30: ecx
        ebp + 0x2c: edx
        ebp + 0x28: ebx
        ebp + 0x24: eo
        ebp + 0x20: ebp
        ebp + 0x1c: esi
        ebp + 0x18: edi
        ebp + 0x14: ds
        ebp + 0x10: es
        ebp + 0x0c: fs
        ebp + 0x08: gs
        ebp + 0x04: return address
        ebp + 0x00: caller's ebp
    */
    uint8_t code;
    asm volatile ("mov 0x38(%%ebp), %0" : "=g" (code));

    uint8_t error;
    asm volatile ("mov 0x3C(%%ebp), %0" : "=g" (error));
    s_handlers[code](code, error);
}

void InstallHandler(uint8_t intNo, Handler handler)
{
   s_handlers[intNo] = handler; 
}

void Init()
{
    for (uint8_t i = 0; i < handlers_count; ++i)
    {
        idt::Install(i, _isr_stub_table[i]);
    }

    for (uint8_t i = 0; i < 32; ++i)
    {
        InstallHandler(i, Exception);
    }
} 
}