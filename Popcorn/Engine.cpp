#include "Engine.h"


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
   
   Level.Draw(hdc, paint_area);
   
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



//------------------------------------------------------------------------------------------------------------


