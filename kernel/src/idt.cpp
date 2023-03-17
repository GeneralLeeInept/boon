#include "idt.h"

namespace idt
{
struct Idtr
{
    uint16_t size{};
    uint32_t offset{};
} __attribute__((packed));


struct IdtEntry
{
    uint16_t offsetLo{};
    uint16_t selector{};
    uint8_t reserved0{};
    uint8_t gateType:4{};
    uint8_t reserved1:1{};
    uint8_t dpl:2{};
    uint8_t present:1{};
    uint16_t offsetHi{};
} __attribute__((packed));

__attribute__((aligned(0x10)))
static IdtEntry s_idt[256]{};

static Idtr s_idtr =
{
    .size = sizeof(s_idt) - 1,
    .offset = (uint32_t)s_idt
};

static void SetIDTEntry(uint8_t index, uint32_t offset, uint16_t selector, uint8_t gateType, uint8_t dpl)
{
    IdtEntry* entry = &s_idt[index];
    entry->offsetLo = (uint16_t)(offset & 0xffff);
    entry->offsetHi = (uint16_t)((offset >> 16) & 0xffff);
    entry->selector = selector;
    entry->gateType = gateType;
    entry->dpl = dpl;
    entry->present = 1;
}

void Install(uint8_t index, uint32_t handler)
{
    constexpr uint16_t KERNEL_CODE_SELECTOR = 0x08;
    constexpr uint8_t GATETYPE_INTERRUPT_32 = 0xE;
    constexpr uint8_t GATETYPE_TRAP_32 = 0xF;
    constexpr uint8_t RING_0_DPL = 0x0;
    uint32_t gateType = index < 32 ? GATETYPE_TRAP_32 : GATETYPE_INTERRUPT_32;
    SetIDTEntry(index, handler, KERNEL_CODE_SELECTOR, gateType, RING_0_DPL);
}

void Init()
{
    asm volatile ("lidt %0" : : "m"(s_idtr));
}
}
