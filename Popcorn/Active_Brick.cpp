#include "Active_Brick.h"

HPEN AActive_Brick::Faiding_Blue_Brick_Pens[Max_Fade_Step];
HPEN AActive_Brick::Faiding_Pink_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Faiding_Pink_Brick_Brushes[Max_Fade_Step];
HBRUSH AActive_Brick::Faiding_Blue_Brick_Brushes[Max_Fade_Step];


AActive_Brick::AActive_Brick(EBrick_Type brick_type)
   :Fade_Step(0), Brick_Rect{}, brick_type(brick_type)
{
   Setup_Colors();

}

void AActive_Brick::Setup_Colors()
{
   int max_step = Max_Fade_Step - 1;
   unsigned char r, g, b;

   for (int i = 0; i < Max_Fade_Step; i++)
   {
      r = AsConfig::Blue_Brick.R - i * ((AsConfig::Blue_Brick.R - AsConfig::BG_Color.R) / max_step);
      g = AsConfig::Blue_Brick.G - i * ((AsConfig::Blue_Brick.G - AsConfig::BG_Color.G) / max_step);
      b = AsConfig::Blue_Brick.B - i * ((AsConfig::Blue_Brick.B - AsConfig::BG_Color.B) / max_step);

      AsConfig::Create_Pen_Brush(r, g, b, Faiding_Blue_Brick_Pens[i], Faiding_Blue_Brick_Brushes[i]);

      r = AsConfig::Pink_Brick.R - i * ((AsConfig::Pink_Brick.R - AsConfig::BG_Color.R) / max_step);
      g = AsConfig::Pink_Brick.G - i * ((AsConfig::Pink_Brick.G - AsConfig::BG_Color.G) / max_step);
      b = AsConfig::Pink_Brick.B - i * ((AsConfig::Pink_Brick.B - AsConfig::BG_Color.B) / max_step);

      AsConfig::Create_Pen_Brush(r, g, b, Faiding_Pink_Brick_Pens[i], Faiding_Pink_Brick_Brushes[i]);

   }


}

void AActive_Brick::Act(HWND hwnd)
{
   if (Fade_Step < Max_Fade_Step - 1)
   {
      ++Fade_Step;

      InvalidateRect(hwnd, &Brick_Rect, FALSE);

   }

}

void AActive_Brick::Draw(HDC hdc, RECT &paint_area)
{
   HPEN pen;
   HBRUSH brush;

   switch(brick_type)
   {
      case EBT_Blue:
         pen = Faiding_Blue_Brick_Pens[Fade_Step];
         brush = Faiding_Blue_Brick_Brushes[Fade_Step];
         break;
      case EBT_Pink: 
         pen = Faiding_Pink_Brick_Pens[Fade_Step];
         brush = Faiding_Pink_Brick_Brushes[Fade_Step];
         break;
      default:
         pen = 0;
         brush = 0;

   }   

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);

}
