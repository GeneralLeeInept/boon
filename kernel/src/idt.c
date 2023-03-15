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

extern void DivideByZero();

void ExceptionHandler()
{
    kprint("Exception\n");
    asm volatile ("hlt");

    // if (code == 0)
    // {
    //     kprint("Divide by zero\n");
    // }
}

extern unsigned int _isr_stub_table[];

void InitIDT()
{
    for (unsigned int index = 0; index < 32; index++)
    {
       SetIDTEntry(index, _isr_stub_table[index], 0x08, 0x0E, 0); 
    }

    asm volatile ("lidt %0" : : "m"(s_idtr));
}