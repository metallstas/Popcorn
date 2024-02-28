#pragma once

#include <Windows.h>
#include "Config.h"

class AsBorder
{
public:
   HPEN Border_Pen_Blue, Border_Pen_White;
   HBRUSH Border_Brush_Blue, Border_Brush_White;

   AsBorder();
   void Init();

   void Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush);

private:

   void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN pen_black, HBRUSH brush_black);
};