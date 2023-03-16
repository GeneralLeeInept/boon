#pragma once

#include <stdint.h>

namespace irq
{
using handler = void(*)();

void init();
void installHandler(uint8_t irq, handler handler);
}