#pragma once

#include <stdint.h>

namespace idt
{
void install(uint8_t index, uint32_t handler);
void init();
}