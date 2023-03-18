#pragma once

#include <stdint.h>

namespace vga
{
void Init();

void SetMode13H();
void SetModeX();
void SetModeY();

void DrawTestPattern();

void SetPixel(uint16_t x, uint16_t y, uint8_t c);
}