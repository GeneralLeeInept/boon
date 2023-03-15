#include <stdint.h>

struct IDTR
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

typedef struct IDTR IDTR_t;

struct IDTEntry
{
    uint16_t offsetLo;
    uint16_t selector;
    uint8_t reserved0;
    uint8_t gateType:4;
    uint8_t reserved1:1;
    uint8_t dpl:2;
    uint8_t present:1;
    uint16_t offsetHi;
} __attribute__((packed));

typedef struct IDTEntry IDTEntry_t;

__attribute__((aligned(0x10)))
static IDTEntry_t s_idt[256] = { 0 };

static IDTR_t s_idtr =
{
    .size = sizeof(s_idt) - 1,
    .offset = (uint32_t)s_idt
};

extern void kprint(const char* msg);

static void SetIDTEntry(unsigned int index, unsigned int offset, uint16_t selector, uint8_t gateType, uint8_t dpl)
{
    IDTEntry_t* entry = &s_idt[index];
    entry->offsetLo = (uint16_t)(offset & 0xffff);
    entry->offsetHi = (uint16_t)((offset >> 16) & 0xffff);
    entry->selector = selector;
    entry->gateType = gateType;
    entry->dpl = dpl;
    entry->present = 1;
}

void ExceptionHandler()
{
    /* Stack:
        ebp + 0x3c: error
        ebp + 0x38: code
        ebp + 0x34: eax
        ebp + 0x30: ecx
        ebp + 0x2c: edx
        ebp + 0x28: ebx
        ebp + 0x24: esp
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
    uint8_t error;
    asm volatile ("mov 0x3C(%%ebp), %0" : "=g" (error));

    uint8_t code;
    asm volatile ("mov 0x38(%%ebp), %0" : "=g" (code));

    char* msg = "Exception 0x00 (error = 0x00)\n";
    const char* hex="0123456789ABCDEF";
    msg[12] = hex[(code & 0xF0) >> 4];
    msg[13] = hex[code & 0x0F];
    msg[26] = hex[(error & 0xF0) >> 4];
    msg[27] = hex[error & 0x0F];
    kprint(msg);
    asm volatile ("hlt");
}

extern unsigned int _isr_stub_table[];

void InitIDT()
{
    for (unsigned int index = 0; index < 32; index++)
    {
       SetIDTEntry(index, _isr_stub_table[index], 0x08, 0x0E, 0); 
    }

    asm volatile ("lidt %0" : : "m"(s_idtr));
    asm volatile ("sti");
}