#pragma once

#include "Config.h"


enum EBrick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

class AActive_Brick
{
public:
   EBrick_Type Brick_Type;

   AActive_Brick(EBrick_Type brick_type);

   static void Setup_Colors();
   void Act(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);

private:

   HPEN Pen_Active_Brick;
   HBRUSH Brush_Active_Brick;

   static const int Max_Fade_Step = 20;
   static HPEN Faiding_Blue_Brick_Pens[Max_Fade_Step];
   static HPEN Faiding_Pink_Brick_Pens[Max_Fade_Step];
   static HBRUSH Faiding_Pink_Brick_Brushes[Max_Fade_Step];
   static HBRUSH Faiding_Blue_Brick_Brushes[Max_Fade_Step];

   int Fade_Step;
   RECT Brick_Rect;

   static void Get_Faiding_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush);
   static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);


};
