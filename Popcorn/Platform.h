#pragma once

#include "Config.h"

enum EPlatform_State
{
   EPS_Missing,
   EPS_Normal,
   EPS_Meltdown,
   EPS_Roll_In
};

class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Act(HWND hwnd);
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);
   void Set_State(EPlatform_State state);

   int X_Step;
   int Width;
   int X_Pos;

private:
   EPlatform_State Platform_State;

   HPEN Pen_Pink, Pen_Blue, Pen_Black, Pen_White;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Size = 7;

   static const int Normal_Width = 28;
   int Inner_Width;
   static const int Height = 7;  
   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
   void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
};
