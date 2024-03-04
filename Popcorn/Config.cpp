﻿#include "Config.h"

const AColor AsConfig::Pink_Brick(255, 85, 255);
const AColor AsConfig::Blue_Brick(85, 255, 255);
const AColor AsConfig::Ball_Color(255, 255, 255);
const AColor AsConfig::BG_Color(0, 0, 0);

AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
   :R(r), G(g), B(b)
{
   
}


void AsConfig::Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
   brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}

void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}