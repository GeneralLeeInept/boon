#pragma once

namespace vga
{
void Init();

void SetMode13H();
void SetModeX();
void SetModeY();

void DrawTestPattern();
}