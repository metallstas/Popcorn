#pragma once

#include "Config.h"

enum EPlatform_State
{
   EPS_Missing,
   EPS_Normal,
   EPS_Meltdown
};

class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Act(HWND hwnd);
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);

   int X_Step;
   int Width;
   int X_Pos;

private:
   EPlatform_State Platform_State;

   HPEN Pen_Pink, Pen_Blue, Pen_Black, Pen_White;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Size = 7;

   int Inner_Width;
   int Meltdown_Y_Pos;
   static const int Height = 7;  

   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
};
