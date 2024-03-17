#include "Ball.h"

const double ABall::Start_Ball_Y_Pos = 181.0;


ABall::ABall()
   : Ball_State(EBS_Normal), Ball_Rect{}, Prev_Ball_Rect{}, Ball_Pen_White(0), Ball_Brush_White(0), Ball_X_Pos(0), 
     Ball_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(0), Ball_Direction(0)
{
   Reset(0);
}
//------------------------------------------------------------------------------------------------------------


void ABall::Init()
{
   AsConfig::Create_Pen_Brush(AsConfig::Ball_Color, Ball_Pen_White, Ball_Brush_White);
}

void ABall::Reset(int x_pos)
{
   Ball_X_Pos = x_pos - AsConfig::Ball_Size / 2;
   Ball_Y_Pos = Start_Ball_Y_Pos;
   Ball_State = EBS_Normal;
   Ball_Speed = 3.0;
   Ball_Direction = M_PI - M_PI_4;
}

void ABall::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
   {

      //Отчищаем фон
      SelectObject(hdc, AsConfig::BG_Pen);
      SelectObject(hdc, AsConfig::BG_Brush);

      Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

   }
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
   {
      //Рисуем шарик
      SelectObject(hdc, Ball_Pen_White);
      SelectObject(hdc, Ball_Brush_White);

      Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
   }
}

//------------------------------------------------------------------------------------------------------------

void ABall::Move(int platform_x_pos, int platform_width, ALevel *level)
{
   double next_x_pos, next_y_pos;
   int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
   int max_y_pos = AsConfig::Max_Y_Pos - AsConfig::Ball_Size;
   int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;

   if (Ball_State != EBS_Normal)
      return;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (Ball_Speed * sin(Ball_Direction));

   //Корректируем позицию при отражении от рамки
   if(next_x_pos < AsConfig::Border_X_Offset)
   {
      next_x_pos = AsConfig::Level_X_Offset - (next_x_pos - AsConfig::Level_X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if(next_y_pos < AsConfig::Border_Y_Offset)
   {
      next_y_pos = AsConfig::Border_Y_Offset - (next_y_pos - AsConfig::Border_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > max_y_pos)
   {
      if (level->Has_Floor)
      {
         next_y_pos = max_y_pos - (next_y_pos - AsConfig::Max_Y_Pos);
         Ball_Direction = -Ball_Direction;
      }
      else
      {
         if (next_y_pos > max_y_pos + AsConfig::Ball_Size * 2)
         Ball_State = EBS_Lost;
      }

   }
   //Отражение от платформы

   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = -Ball_Direction;
      }
   }

   level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = (int)Ball_X_Pos * AsConfig::Global_Scale;
   Ball_Rect.top = (int)Ball_Y_Pos * AsConfig::Global_Scale;
   Ball_Rect.right = Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Global_Scale;
    
   InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
