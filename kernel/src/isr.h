#pragma once

#include <stdint.h>

namespace isr
{
using handler = void(*)(int intNo, int err);

void init();
void installHandler(uint8_t intNo, handler handler);
}