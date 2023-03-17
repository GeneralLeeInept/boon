#include "idt.h"
#include "irq.h"
#include "isr.h"
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
}

void keyhandler()
{
    monitor::PrintChar('#');
    inb(0x60);
}

extern "C" void kmain()
{
    irq::InstallHandler(0x01, keyhandler);
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
