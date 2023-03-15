#pragma once

#include <stdint.h>

namespace isr
{
using handler = void(*)();

void init();
void install(uint8_t index, handler handler);
}