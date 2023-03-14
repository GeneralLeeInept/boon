#include <stdint.h>

typedef struct IDTR
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

typedef struct IDTEntry
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


