#pragma once

#include "Config.h"
#include "Level.h"

class ABall
{
public:
   ABall();

   double Ball_Direction;

   void Init();
   void Draw(HDC hdc, RECT &paint_area, HPEN &bg_pen, HBRUSH &bg_brush);
   void Move(HWND hwnd, int platform_x_pos, int platform_width, ALevel *level);

private:
   RECT Ball_Rect, Prev_Ball_Rect;
   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   double Ball_Speed;

   int Ball_X_Pos;
   int Ball_Y_Pos;

};
