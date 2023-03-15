#include "isr.h"

#include "idt.h"

void kprint(const char*);
void kprintHex(uint8_t);

namespace isr
{
static constexpr int handlers_count = 48;   // 32 exceptions + 16 IRQs

static handler s_handlers[handlers_count];
extern "C" uint32_t _isr_stub_table[];

void exception(int code, int error)
{
    kprint("Exception 0x");
    kprintHex(code);
    kprint(" (error = 0x");
    kprintHex(error);
    kprint(")\n");

    asm volatile ("hlt");
}

extern "C" void route_isr()
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
    
    if (code < 32)
    {
        uint8_t error;
        asm volatile ("mov 0x3C(%%ebp), %0" : "=g" (error));
        exception(code, error);
    }
    else if (s_handlers[code])
    {
        s_handlers[code]();
    }
}

void install(uint8_t index, handler handler)
{
   s_handlers[index] = handler; 
}

void init()
{
    for (unsigned int i = 0; i < handlers_count; ++i)
    {
        idt::install(i, _isr_stub_table[i]);   
    }
} 
}