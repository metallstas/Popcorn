#include "Engine.h"

char Level_01[ALevel::Level_Height][ALevel::Level_Width] =
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
   : Ball_Rect{}, Prev_Ball_Rect{}, Ball_Pen_White(0), Ball_Brush_White(0), Ball_X_Pos(20), Ball_Y_Pos(180), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
   
}
//------------------------------------------------------------------------------------------------------------


void ABall::Init()
{
   AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen_White, Ball_Brush_White);
}

//------------------------------------------------------------------------------------------------------------

void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;

   //Отчищаем фон
   SelectObject(hdc, engine->BG_Pen_Black);
   SelectObject(hdc, engine->BG_Brush_Black);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
   //Рисуем шарик
   SelectObject(hdc, Ball_Pen_White);
   SelectObject(hdc, Ball_Brush_White);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right, Ball_Rect.bottom);
}

//------------------------------------------------------------------------------------------------------------

void ABall::Move(ALevel *level, AsPlatform *platform, AsEngine *engine)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + int(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - int(Ball_Speed * sin(Ball_Direction));

   //Корректируем позицию при отражении от рамки
   if(next_x_pos < AsBorder::Border_X_Offset)
   {
      next_x_pos = ALevel::Level_X_Offset - (next_x_pos - ALevel::Level_X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if(next_y_pos < AsBorder::Border_Y_Offset)
   {
      next_y_pos = AsBorder::Border_Y_Offset - (next_y_pos - AsBorder::Border_Y_Offset);
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
      if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = -Ball_Direction;
      }
   }

   level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}


void ALevel::Init()
{

   AsConfig::Create_Pen_Brush(255, 85, 255, Pen_Pink, Brush_Pink);
   AsConfig::Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);


   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));

   Level_Rect.left = Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left * Cell_Width * Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top * Cell_Height * Level_Height * AsConfig::Global_Scale;
   Level_Rect.top = Level_Y_Offset * AsConfig::Global_Scale;
}

ALevel::ALevel()
   : Letter_Pen(0), Pen_Pink(0), Pen_Blue(0), Brush_Pink(0), Brush_Blue(0), Level_Rect{} 
{                                                                                         


}

//------------------------------------------------------------------------------------------------------------

AsPlatform::AsPlatform()
   :X_Pos(AsBorder::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2), Width(28), Inner_Width(21), Pen_Pink(0), Pen_Blue(0), Brush_Pink(0),
   Brush_Blue(0), Pen_Black(0), Brush_Black(0), Brush_White(0), Pen_White(0), Platform_Rect{}, Prev_Platform_Rect{}
{

}
void AsPlatform::Init()
{
   AsConfig::Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);
   AsConfig::Create_Pen_Brush(255, 255, 255, Pen_White, Brush_White);
   AsConfig::Create_Pen_Brush(255, 85, 255, Pen_Pink, Brush_Pink);
   AsConfig::Create_Pen_Brush(0, 0, 0, Pen_Black, Brush_Black);
}

//------------------------------------------------------------------------------------------------------------


AsEngine::AsEngine()
   :Hwnd(0), BG_Pen_Black(0), BG_Brush_Black(0) 
{
   
}



//------------------------------------------------------------------------------------------------------------


void AsEngine::Init_Engine(HWND hWnd)
{//Настройка игры при старте

   Hwnd = hWnd;

   AsConfig::Create_Pen_Brush(0, 0, 0, BG_Pen_Black, BG_Brush_Black);

   Level.Init();
   Ball.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw(this);

   SetTimer(Hwnd, Timer_ID, 50, 0);

}

//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//Отрисовка экрана игры
   
   Level.Draw(hdc, paint_area, this);
   
   Platform.Draw(hdc, paint_area);
   
   //for (int i = 0; i < 16; i++)
   //{
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area, this);

   Border.Draw(hdc, paint_area, BG_Pen_Black, BG_Brush_Black);

}


//------------------------------------------------------------------------------------------------------------


int AsEngine::On_Key_Down(EKey_Type key_type)
{

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;
      if (Platform.X_Pos <= AsBorder::Border_X_Offset)
         Platform.X_Pos = AsBorder::Border_X_Offset;
      Platform.Redraw(this);
      break;
   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;
      if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = Max_X_Pos - Platform.Width + 1;
      Platform.Redraw(this);
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

   Ball.Move(&Level, &Platform, this);

   return 0;

}

//------------------------------------------------------------------------------------------------------------

void AsPlatform::Redraw(AsEngine *engine)
{//Отрисовка экрана игры

   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = (Platform_Rect.left + Width) * AsConfig::Global_Scale;
   Platform_Rect.bottom = (Platform_Rect.top + Height) * AsConfig::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);

 }

//------------------------------------------------------------------------------------------------------------


void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type, AsEngine *engine)
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

   RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Brick_Width) * AsConfig::Global_Scale, (y + Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}

//------------------------------------------------------------------------------------------------------------

void ALevel::Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
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

void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{//Вывод падающей буквы
   bool swith_color;
   double offset;
   double rotation_angle; //преобразование шага в угол поворота
   int brick_half_height = Brick_Height * AsConfig::Global_Scale / 2;
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

      Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + Brick_Width * AsConfig::Global_Scale, y + brick_half_height);

      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * AsConfig::Global_Scale, y + brick_half_height + AsConfig::Global_Scale - 1);

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

      Rectangle(hdc, 0, 0 - brick_half_height - offset_back_part, Brick_Width * AsConfig::Global_Scale, brick_half_height - (int)offset);
      //Передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, 0 - brick_half_height, Brick_Width * AsConfig::Global_Scale, brick_half_height);

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

void ALevel::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{//Вывод уровня
   int i, j;
   RECT intersection_rect;

   if(IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      return;

   for (i = 0; i < Level_Height; i++)
   {
      for (j = 0; j < Level_Width; j++)
      {
         this->Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j], engine );
      }
   }
}

//------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   int x = X_Pos;
   int y = Y_Pos;
   //Отрисовка Платформы
   //Рисуем боковые шарики
   if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   SelectObject(hdc, Pen_Black);
   SelectObject(hdc, Brush_Black);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   SelectObject(hdc, Pen_Pink);
   SelectObject(hdc, Brush_Pink);

   Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Inner_Width + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
   
   //Рисуем Блик на шарике
   SelectObject(hdc, Pen_White);

   Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x - 1 + Circle_Size) * AsConfig::Global_Scale, (y - 1 + Circle_Size) * AsConfig::Global_Scale,
      (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 2 + 1) * AsConfig::Global_Scale);

   //Рисуем центральную частьплатформы
   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

}

//------------------------------------------------------------------------------------------------------------

void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction)
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
            ball_direction = -ball_direction;
         }
      }
      brick_y_pos -= Cell_Height;
   }
}

//------------------------------------------------------------------------------------------------------------


