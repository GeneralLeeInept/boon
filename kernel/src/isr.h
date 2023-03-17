#pragma once

#include <stdint.h>

namespace isr
{
using Handler = void(*)(int intNo, int err);

void Init();
void InstallHandler(uint8_t intNo, Handler handler);
}