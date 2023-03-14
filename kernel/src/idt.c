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

static IDTEntry_t s_idtTable[256] = { 0 };

static IDTR_t s_idtr =
{
    .size = sizeof(s_idtTable) - 1,
    .offset = (uint32_t)s_idtTable
};

extern void kprint(const char* msg);

static void SetIDTEntry(unsigned int index, unsigned int offset, uint16_t selector, uint8_t gateType, uint8_t dpl)
{
    IDTEntry_t* entry = &s_idtTable[index];
    entry->offsetLo = (uint16_t)(offset & 0xffff);
    entry->offsetHi = (uint16_t)((offset >> 16) & 0xffff);
    entry->selector = selector;
    entry->gateType = gateType;
    entry->dpl = dpl;
    entry->present = 1;

    uint8_t access = 0x80 | ((dpl & 3) << 5) | (gateType & 0xf);
    uint8_t* check = &(entry->reserved0) + 1;
    if (access != *check)
    {
        kprint("SetIDTEntry: you made a mistake\n");
    }
}

extern void DivideByZero();

void ExceptionHandler(int code)
{
    if (code == 0)
    {
        kprint("Divide by zero\n");
    }
}

void InitIDT()
{
/*     SetIDTEntry(0, 0, 0x10, 0x6, 0x2);
    SetIDTEntry(1, 0, 0x10, 0x7, 0x3);
    SetIDTEntry(2, 0, 0x10, 0xF, 0x1); */
    SetIDTEntry(0, (unsigned int)DivideByZero, 0x08, 0xE, 0x0);
    asm volatile ("lidt %0" : : "m"(s_idtr));
}