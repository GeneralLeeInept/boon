#include "idt.h"

namespace idt
{
struct idtr_t
{
    uint16_t size{};
    uint32_t offset{};
} __attribute__((packed));


struct idt_entry_t
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
static idt_entry_t s_idt[256]{};

static idtr_t s_idtr =
{
    .size = sizeof(s_idt) - 1,
    .offset = (uint32_t)s_idt
};

static void SetIDTEntry(uint8_t index, uint32_t offset, uint16_t selector, uint8_t gateType, uint8_t dpl)
{
    idt_entry_t* entry = &s_idt[index];
    entry->offsetLo = (uint16_t)(offset & 0xffff);
    entry->offsetHi = (uint16_t)((offset >> 16) & 0xffff);
    entry->selector = selector;
    entry->gateType = gateType;
    entry->dpl = dpl;
    entry->present = 1;
}

void install(uint8_t index, uint32_t handler)
{
    constexpr uint16_t KERNEL_CODE_SELECTOR = 0x08;
    constexpr uint8_t GATETYPE_INTERRUPT_32 = 0xE;
    constexpr uint8_t RING_0_DPL = 0x0;
    SetIDTEntry(index, handler, KERNEL_CODE_SELECTOR, GATETYPE_INTERRUPT_32, RING_0_DPL);
}

void init()
{
    asm volatile ("lidt %0" : : "m"(s_idtr));
}
}
