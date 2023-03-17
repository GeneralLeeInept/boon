#pragma once

#include <stdint.h>

namespace idt
{
void Init();
void Install(uint8_t index, uint32_t handler);
}