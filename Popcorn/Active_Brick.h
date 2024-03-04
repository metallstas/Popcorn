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
   AActive_Brick(EBrick_Type);
   EBrick_Type brick_type;

   static const int Max_Fade_Step = 20;
   static HPEN Faiding_Blue_Brick_Pens[Max_Fade_Step];
   static HPEN Faiding_Pink_Brick_Pens[Max_Fade_Step];
   static HBRUSH Faiding_Pink_Brick_Brushes[Max_Fade_Step];
   static HBRUSH Faiding_Blue_Brick_Brushes[Max_Fade_Step];

   int Fade_Step;
   RECT Brick_Rect;

   static void Setup_Colors();
   void Act(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);

};
