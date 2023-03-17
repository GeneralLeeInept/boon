#pragma once

#include <stdint.h>

struct BootParams;

namespace monitor
{
void Init(const BootParams& bootParams);

void PrintChar(char c);
void Print(const char* msg);
void PrintHex(uint8_t byte);
void PrintNum(int num);
}