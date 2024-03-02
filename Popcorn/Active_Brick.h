#pragma once

#include "Config.h"

class AActive_Brick
{
public:
   AActive_Brick();

   static const int Max_Fade_Step = 40;

   int Fade_Step;
   RECT Brick_Rect;

   void Act(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);

};
