#pragma once

#include "Config.h"

enum EPlatform_State
{
   EPS_Missing,
   EPS_Normal,
   EPS_Meltdown,
   EPS_Roll_In,
   EPS_Expand_Roll_In
};

class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Act();
   void Redraw();
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

   static const int Rolling_Platform_Speed = 3;
   static const int Max_Rolling_Step = 8;
   static const int Circle_Size = 7;
   static const int Normal_Width = 28;
   static const int Height = 7;
   static const int Roll_In_Platform_End_X_Pos = AsConfig::Max_X_Pos / 2 - 1;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   int Inner_Width;
   int Rolling_Step;

   void Clear_BG(HDC hdc);
   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
   void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
   void Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area);
   void Draw_Circle_Highlight(HDC hdc, RECT &paint_area, int x, int y);

};
