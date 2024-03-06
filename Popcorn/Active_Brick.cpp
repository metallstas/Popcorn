#include "Active_Brick.h"

HPEN AActive_Brick::Faiding_Blue_Brick_Pens[Max_Fade_Step];
HPEN AActive_Brick::Faiding_Pink_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Faiding_Pink_Brick_Brushes[Max_Fade_Step];
HBRUSH AActive_Brick::Faiding_Blue_Brick_Brushes[Max_Fade_Step];


AActive_Brick::AActive_Brick(EBrick_Type brick_type)
   :Fade_Step(0), Brick_Rect{}, Brick_Type(brick_type), Pen_Active_Brick{}, Brush_Active_Brick{}
{
   Setup_Colors();
}

unsigned char AActive_Brick::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
   return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}

void AActive_Brick::Setup_Colors()
{
   
   for (int i = 0; i < Max_Fade_Step; i++)
   {
      Get_Faiding_Color(AsConfig::Pink_Brick, i, Faiding_Pink_Brick_Pens[i], Faiding_Pink_Brick_Brushes[i]);

      Get_Faiding_Color(AsConfig::Blue_Brick, i, Faiding_Blue_Brick_Pens[i], Faiding_Blue_Brick_Brushes[i]);

   }
}

void AActive_Brick::Get_Faiding_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush)
{
   unsigned char r, g, b;

   r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
   g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
   b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);

   AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
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

   switch(Brick_Type)
   {
      case EBT_Blue:
         Pen_Active_Brick = Faiding_Blue_Brick_Pens[Fade_Step];
         Brush_Active_Brick = Faiding_Blue_Brick_Brushes[Fade_Step];
         break;
      case EBT_Pink: 
         Pen_Active_Brick = Faiding_Pink_Brick_Pens[Fade_Step];
         Brush_Active_Brick = Faiding_Pink_Brick_Brushes[Fade_Step];
         break;
      default:
         Pen_Active_Brick;
         Brush_Active_Brick;
   }   

   SelectObject(hdc, Pen_Active_Brick);
   SelectObject(hdc, Brush_Active_Brick);

   Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);

}
