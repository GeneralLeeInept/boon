#pragma once

#include <stdint.h>

namespace irq
{
using Handler = void(*)();

void Init();
void InstallHandler(uint8_t irq, Handler handler);
}