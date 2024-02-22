#include "Engine.h"

char Level_01[AsEngine::Level_Height][AsEngine::Level_Width] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------

ABall::ABall()
   : Ball_X_Pos(20), Ball_Y_Pos(180), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
   
}


//------------------------------------------------------------------------------------------------------------

void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;

   //Отчищаем фон
   SelectObject(hdc, engine->Pen_Black);
   SelectObject(hdc, engine->Brush_Black);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
   //Рисуем шарик
   SelectObject(hdc, engine->Pen_White);
   SelectObject(hdc, engine->Brush_White);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}

//------------------------------------------------------------------------------------------------------------

void ABall::Move(AsEngine *engine)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int platform_y_pos = AsEngine::Platform_Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + int(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - int(Ball_Speed * sin(Ball_Direction));

   //Корректируем позицию при отражении от рамки
   if(next_x_pos < AsEngine::Border_X_Offset)
   {
      next_x_pos = AsEngine::Level_X_Offset - (next_x_pos - AsEngine::Level_X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if(next_y_pos < AsEngine::Border_Y_Offset)
   {
      next_y_pos = AsEngine::Border_Y_Offset - (next_y_pos - AsEngine::Border_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > AsEngine::Max_Y_Pos)
   {
      next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
      Ball_Direction = -Ball_Direction;
   }
   //Отражение от платформы

   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= engine->Platform_X_Pos && next_x_pos <= engine->Platform_X_Pos + engine->Platform_Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = -Ball_Direction;
      }
   }

   engine->Check_Level_Brick_Hit(next_y_pos);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * AsEngine::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsEngine::Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * AsEngine::Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsEngine::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}

//------------------------------------------------------------------------------------------------------------


AsEngine::AsEngine()
   :Inner_Width(21), Platform_X_Pos(100), Platform_X_Step(6), Platform_Width(28)
{
   
}



//------------------------------------------------------------------------------------------------------------


void AsEngine::Init_Engine(HWND hWnd)
{//Настройка игры при старте

   Hwnd = hWnd;

   Create_Pen_Brush(255, 85, 255, Pen_Pink, Brush_Pink);
   Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);
   Create_Pen_Brush(255, 255, 255, Pen_White, Brush_White);
   Create_Pen_Brush(0, 0, 0, Pen_Black, Brush_Black);

   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));

   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_Rect.left * Cell_Width * Level_Width * Global_Scale;
   Level_Rect.bottom = Level_Rect.top * Cell_Height * Level_Height * Global_Scale;

   Redraw_Platform();

   SetTimer(Hwnd, Timer_ID, 50, 0);

}

//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//Отрисовка экрана игры

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
   {
      Draw_Level(hdc);
   }

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
   {
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);
   }


   //for (int i = 0; i < 16; i++)
   //{
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area, this);

   Draw_Bounds(hdc, paint_area);

}


//------------------------------------------------------------------------------------------------------------


int AsEngine::On_Key_Down(EKey_Type key_type)
{

   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      if (Platform_X_Pos <= Border_X_Offset)
         Platform_X_Pos = Border_X_Offset;
      Redraw_Platform();
      break;
   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      if (Platform_X_Pos >= Max_X_Pos - Platform_Width + 1)
         Platform_X_Pos = Max_X_Pos - Platform_Width + 1;
      Redraw_Platform();
      break;
   case EKT_Space:
      break;
   default:
      break;
   }

   return 0;
}


//------------------------------------------------------------------------------------------------------------

int AsEngine::On_Timer()
{

   Ball.Move(this);

   return 0;

}
//------------------------------------------------------------------------------------------------------------


void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}

//------------------------------------------------------------------------------------------------------------

void AsEngine::Redraw_Platform()
{//Отрисовка экрана игры

   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = (Platform_Rect.left + Platform_Width) * Global_Scale;
   Platform_Rect.bottom = (Platform_Rect.top + Platform_Height) * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);

 }

//------------------------------------------------------------------------------------------------------------


void AsEngine::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;
   case EBT_Pink:
      pen = Pen_Pink;
      brush = Brush_Pink;
      break;
   case EBT_Blue:
      pen = Pen_Blue;
      brush = Brush_Blue;
      break;
   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}

//------------------------------------------------------------------------------------------------------------

void AsEngine::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = Pen_Pink;
      front_brush = Brush_Pink;

      back_pen = Pen_Blue;
      back_brush = Brush_Blue;
   }
   else
   {
      front_pen = Pen_Blue;
      front_brush = Brush_Blue;

      back_pen = Pen_Pink;
      back_brush = Brush_Pink;
   }
}

//------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{//Вывод падающей буквы
   bool swith_color;
   double offset;
   double rotation_angle; //преобразование шага в угол поворота
   int brick_half_height = Brick_Height * Global_Scale / 2;
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

      Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);

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

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      offset_back_part = (int)round(offset);

      Rectangle(hdc, 0, 0 - brick_half_height - offset_back_part, Brick_Width * Global_Scale, brick_half_height - (int)offset);
      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, 0 - brick_half_height, Brick_Width * Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
         SelectObject(hdc, Letter_Pen);
         Ellipse(hdc, (0 + 5) * Global_Scale, -2 * Global_Scale, (0 + 10) * Global_Scale, 2 * Global_Scale);
         }
      }
      
      SetWorldTransform(hdc, &old_xform);
   }
}

//------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Level(HDC hdc)
{//Вывод уровня
   int i, j;

   for (i = 0; i < Level_Height; i++)
   {
      for (j = 0; j < Level_Width; j++)
      {
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
      }
   }
}

//------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Platform(HDC hdc, int x, int y)
{
   //Отрисовка Платформы
   //Рисуем боковые шарики

   SelectObject(hdc, Pen_Black);
   SelectObject(hdc, Brush_Black);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   SelectObject(hdc, Pen_Pink);
   SelectObject(hdc, Brush_Pink);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   
   //Рисуем Блик на шарике
   SelectObject(hdc, Pen_White);

   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x - 1 + Circle_Size) * Global_Scale, (y - 1 + Circle_Size) * Global_Scale,
      (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 2 + 1) * Global_Scale);

   //Рисуем центральную частьплатформы
   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);

}


//------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{//Рисуем элемент рамки

   //Основная линия рамки
   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   if(top_border)
      Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale, (4 + x) * Global_Scale, (4 + y) * Global_Scale);
   else
      Rectangle(hdc, (1 + x) * Global_Scale, y * Global_Scale, (4 + x) * Global_Scale, (4 + y) * Global_Scale);

   //Белая койма
   SelectObject(hdc, Pen_White);
   SelectObject(hdc, Brush_White);

   if(top_border)
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (4 + x) * Global_Scale, (1 + y) * Global_Scale);
   else
      Rectangle(hdc, (0 + x) * Global_Scale, (0 + y) * Global_Scale, (1 + x) * Global_Scale, (4 + y) * Global_Scale);

   //Черная точка

   SelectObject(hdc, Pen_Black);
   SelectObject(hdc, Brush_Black);

   if (top_border)
      Rectangle(hdc, (2 + x) * Global_Scale, (2 + y) * Global_Scale, (3 + x) * Global_Scale, (3 + y) * Global_Scale);
   else
      Rectangle(hdc, (2 + x) * Global_Scale, (1 + y) * Global_Scale, (3 + x) * Global_Scale, (2 + y) * Global_Scale);

}

//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Bounds(HDC hdc, RECT paint_area)
{//Рисуем все элементы рамки

   //Рамка слева
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 2, 1 + i * 4, false);
   }
   //Рамка справа
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 201, 1 + i * 4, false);
   }

   //Рамка сxверху
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 3 + i * 4, 0, true);
   }
}



//------------------------------------------------------------------------------------------------------------

void AsEngine::Check_Level_Brick_Hit(int &next_y_pos)
{
   //Отражение от кирпичей
   int brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;

   for (int i = Level_Height - 1; i >= 0; i--)
   {

      for (int j = 0; j < Level_Width; j++)
      {
         if (Level_01[i][j] == 0)
            continue;
         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            Ball.Ball_Direction = -Ball.Ball_Direction;
         }
      }
      brick_y_pos -= Cell_Height;
   }
}

//------------------------------------------------------------------------------------------------------------


