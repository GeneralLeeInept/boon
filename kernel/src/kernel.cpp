#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "monitor.h"
#include "port.h"

#include <stdint.h>

extern "C" void _init();

extern "C" void KernelInit(BootParams* bootParams)
{
    monitor::Init(*bootParams);
    idt::Init();
    isr::Init();
    irq::Init();
    asm volatile ("sti");

    // TODO: malloc / free support

    _init();

    keyboard::Init();
}

extern "C" void kmain()
{
    monitor::Print("Hello from kernel!\tThere's a tab behind me\n");

    for (;;) {}
}

extern "C" void __cxa_finalize(void*);
extern "C" void _fini();

extern "C" void KernelExit()
{
    __cxa_finalize(0);
    _fini();
    monitor::Print("Exiting kernel.\n");
}
