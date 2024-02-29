#pragma once

#include "Config.h"

class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);


   int X_Step;
   int Width;
   int X_Pos;

private:
   HPEN Pen_Pink, Pen_Blue, Pen_Black, Pen_White;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Size = 7;

   int Inner_Width;
   static const int Height = 7;  



};
