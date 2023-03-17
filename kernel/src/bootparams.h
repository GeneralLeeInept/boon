#pragma once

#include <stdint.h>

struct BootParams
{
    uint8_t cursorColumn;
    uint8_t cursorRow;
} __attribute__((packed));
