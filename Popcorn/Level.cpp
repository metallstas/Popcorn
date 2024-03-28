#include "Level.h"

char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

ALevel::ALevel()
	: Active_Brick(EBT_Pink), Letter_Pen(0), Level_Pen_Pink(0), Level_Pen_Blue(0), Level_Brush_Pink(0), Level_Brush_Blue(0), Level_Rect{}
{                                                                                         


}

void ALevel::Init()
{

	AsConfig::Create_Pen_Brush(AsConfig::Pink_Brick, Level_Pen_Pink, Level_Brush_Pink);
	AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick, Level_Pen_Blue, Level_Brush_Blue);

	Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));

	Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Level_Rect.right = Level_Rect.left * AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
	Level_Rect.bottom = Level_Rect.top * AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;
	Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
}

void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;
   case EBT_Pink:
      pen = Level_Pen_Pink;
      brush = Level_Brush_Pink;
      break;
   case EBT_Blue:
      pen = Level_Pen_Blue;
      brush = Level_Brush_Blue;
      break;
   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}

//------------------------------------------------------------------------------------------------------------

void ALevel::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = Level_Pen_Pink;
      front_brush = Level_Brush_Pink;

      back_pen = Level_Pen_Blue;
      back_brush = Level_Brush_Blue;
   }
   else
   {
      front_pen = Level_Pen_Blue;
      front_brush = Level_Brush_Blue;

      back_pen = Level_Pen_Pink;
      back_brush = Level_Brush_Pink;
   }
}

//------------------------------------------------------------------------------------------------------------

void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{//Вывод падающей буквы
   bool swith_color;
   double offset;
   double rotation_angle; //преобразование шага в угол поворота
   int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
   int offset_back_part;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;

   XFORM xform, old_xform;
   // Rotate

   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);


   if(!(brick_type == EBT_Blue || brick_type == EBT_Pink))
      return; 

   if(rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == EBT_Blue)
         swith_color = true;
      else
         swith_color = false;
   }
   else 
   {
      if (brick_type == EBT_Pink)
         swith_color = true;
      else
         swith_color = false;
   }

   Set_Brick_Letter_Color(swith_color, front_pen, back_pen, front_brush, back_brush);


   if (rotation_step == 4 || rotation_step == 12)
   {
      //Фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height);

      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height + AsConfig::Global_Scale - 1);

   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_height;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      //Фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      offset_back_part = (int)round(offset);

      Rectangle(hdc, 0, 0 - brick_half_height - offset_back_part, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height - (int)offset);
      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, 0 - brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, (0 + 5) * AsConfig::Global_Scale, -2 * AsConfig::Global_Scale, (0 + 10) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}

//------------------------------------------------------------------------------------------------------------

void ALevel::Draw(HDC hdc, RECT &paint_area)
{//Вывод уровня
   int i, j;
   RECT intersection_rect;


   if(IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      return;

   for (i = 0; i < AsConfig::Level_Height; i++)
   {
      for (j = 0; j < AsConfig::Level_Width; j++)
      {
         Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Level_01[i][j] );
      }
   }
      
   Active_Brick.Draw(hdc, paint_area);

}

bool ALevel::Hit_Circle_On_Line(double y, double next_x_pos, double radius, double left_x, double right_x)
{//Проверяет пересечение горизонтального отрезка от left_x до right_x окружностью с радиусом radius

   double x;
   double min_x, max_x;

   if (y > radius)
      return false;

   x = sqrt(radius * radius - y * y);

   max_x = next_x_pos + x;
   min_x = next_x_pos - x;

   if (max_x >= left_x && max_x <= right_x || min_x >= left_x && min_x <= right_x)
      return true;
   else
      return false;
}

bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
   double left_x, right_x;
   double direction = ball->Get_Direction();
   //Отражение от кирпичей
   double brick_top_y, brick_low_y;

   for (int i = AsConfig::Level_Height - 1; i >= 0; i--)
   {
      brick_top_y = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
      brick_low_y = brick_top_y + AsConfig::Brick_Height;

      for (int j = 0; j < AsConfig::Level_Width; j++)
      {
         
         if (Level_01[i][j] == 0)
            continue;

         left_x = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
         right_x = left_x + AsConfig::Brick_Width;

         //Проверяем поподание в верхнюю грань кирпича
         if (direction >= 0 && direction < M_PI)
         {

            if (Hit_Circle_On_Line(next_y_pos - brick_low_y, next_x_pos, ball->Radius, left_x, right_x))
            {
               ball->Reflect(true);
               return true;
            }
         }

         //Проверяем поподание в нижнюю грань кирпича
         if (direction >= M_PI && direction <= M_PI * 2)
         {
            if (Hit_Circle_On_Line(next_y_pos - brick_top_y, next_x_pos, ball->Radius, left_x, right_x))
            {
               ball->Reflect(true);
               return true;
            }
         }
         //Проверяем поподание в левую грань кирпича
         if (direction >= 0.0 && direction < M_PI_2 || direction >= M_PI + M_PI_2 && direction <= M_PI * 2)
         {

            if (Hit_Circle_On_Line(left_x - next_x_pos, next_y_pos, ball->Radius, brick_top_y, brick_low_y))
            {
               ball->Reflect(false);
               return true;
            }
         }
         //Проверяем поподание в правую грань кирпича

         if (direction > M_PI_2 && direction <= M_PI + M_PI_2)
         {

            if (Hit_Circle_On_Line(right_x - next_x_pos, next_y_pos, ball->Radius, brick_top_y, brick_low_y))
            {
               ball->Reflect(false);
               return true;
            }
         }
      }
   }

   return false;
}
