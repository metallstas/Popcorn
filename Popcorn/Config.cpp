#include "Config.h"

const AColor AsConfig::Pink_Brick(255, 85, 255);
const AColor AsConfig::Blue_Brick(85, 255, 255);
const AColor AsConfig::Ball_Color(255, 255, 255);
const AColor AsConfig::BG_Color(0, 0, 0);

int AsConfig::Current_Timer_Tick = 0;

HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;

AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
   :R(r), G(g), B(b)
{
   
}


void AsConfig::Setup_Colors()
{
   AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);
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

int AsConfig::Rand(int range)
{//Вычисляет псевдослучайное число в диапазоне [0... range - 1] 
   return rand() * range / RAND_MAX;
}
