#include "Platform.h"

AsPlatform::AsPlatform()
   :X_Pos(AsConfig::Border_X_Offset), Meltdown_Platform_Y_Pos{}, X_Step(AsConfig::Global_Scale * 2), Platform_State(EPS_Normal), 
   Width(Normal_Width), Inner_Width(21), Pen_Pink(0), Pen_Blue(0), Brush_Pink(0), Rolling_Step(0),
   Brush_Blue(0), Pen_Black(0), Brush_Black(0), Brush_White(0), Pen_White(0), Platform_Rect{}, Prev_Platform_Rect{}
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
void AsPlatform::Init()
{
   AsConfig::Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);
   AsConfig::Create_Pen_Brush(255, 255, 255, Pen_White, Brush_White);
   AsConfig::Create_Pen_Brush(255, 85, 255, Pen_Pink, Brush_Pink);
   AsConfig::Create_Pen_Brush(0, 0, 0, Pen_Black, Brush_Black);
}

void AsPlatform::Act()
{

   if (Platform_State == EPS_Meltdown || Platform_State == EPS_Roll_In || Platform_State == EPS_Expand_Roll_In)
   {
      Redraw();
   }
}

EPlatform_State AsPlatform::Get_State()
{
   return Platform_State;
}

void AsPlatform::Set_State(EPlatform_State new_state)
{
   int length;
   int i;

   if (Platform_State == new_state)
      return;

   switch (new_state)
   {
   case EPS_Missing:
      break;

   case EPS_Meltdown:
      length = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

      for (i = 0; i < length; i++)
      {
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
      }
      break;

   case EPS_Roll_In:
      X_Pos = AsConfig::Max_X_Pos - 1;
      Rolling_Step = Max_Rolling_Step - 1;
      break;

   default:
      break;
   }

   Platform_State = new_state;
}

void AsPlatform::Redraw()
{//Отрисовка экрана игры

   Prev_Platform_Rect = Platform_Rect;
   int platform_width;

   if (Platform_State == EPS_Roll_In)
      platform_width = Circle_Size;
   else
      platform_width = Width;

      Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
      Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
      Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::Global_Scale;
      Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;
  

   if(Platform_State == EPS_Meltdown)
      Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);

}


//------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Circle_Highlight(HDC hdc, RECT &paint_area, int x, int y)
{
   //Рисуем Блик на шарике
   int circle_size = Circle_Size * AsConfig::Global_Scale;

   SelectObject(hdc, Pen_White);

   Arc(hdc, x + 3, y + 3, (x - 3 + circle_size), (y - 3 + circle_size), (x + 6), (y + 3) , (x + 3), (y + 6 + 3));

}

void AsPlatform::Clear_BG(HDC hdc)
{
   //заливаем фоном предыдущее положение шарика
   SelectObject(hdc, Pen_Black);
   SelectObject(hdc, Brush_Black);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

}

void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   //Отрисовка Платформы  в нормально мсостоянии

   int x = X_Pos * AsConfig::Global_Scale;
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int circle_size = Circle_Size * AsConfig::Global_Scale;
   int platform_width = Inner_Width * AsConfig::Global_Scale;
   //Рисуем боковые шарики

   //заливаем фоном предыдущее положение шарика
   Clear_BG(hdc);

   SelectObject(hdc, Pen_Pink);
   SelectObject(hdc, Brush_Pink);

   Ellipse(hdc, x, y, x + circle_size, y + circle_size);
   Ellipse(hdc, x + platform_width, y, x + platform_width + circle_size, y + circle_size);

   Draw_Circle_Highlight(hdc, paint_area, x, y);

   //Рисуем центральную частьплатформы
   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   RoundRect(hdc, x + 4 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale, x + (4 - 1 + Inner_Width) * AsConfig::Global_Scale,
      y + (1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

}

void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
   int i, j;
   int area_width, area_height;
   int x, y, y_offset;
   int moved_collums_count = 0;
   int max_platform_y;

   COLORREF pixel;
   COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

   area_width = Width * AsConfig::Global_Scale;
   area_height = Height * AsConfig::Global_Scale + 1;
   max_platform_y = AsConfig::Max_Y_Pos * AsConfig::Global_Scale + area_height;


   for (i = 0; i < area_width; i++)
   {
      if (Meltdown_Platform_Y_Pos[i] > max_platform_y)
         continue; 

      ++moved_collums_count;

      x = Platform_Rect.left + i;
      y_offset = AsConfig::Rand(AsConfig::Meltdown_Speed) + 1;

      for (j = 0; j < area_height; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - j;

         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }

      for (j = 0; j < y_offset; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - area_height +1 + j;

         SetPixel(hdc, x, y, bg_pixel);
      }
      Meltdown_Platform_Y_Pos[i] += y_offset;
   }  

   if (moved_collums_count == 0)
      Platform_State = EPS_Missing;
   
}

void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{
   XFORM xform, old_xform;
   int x = X_Pos * AsConfig::Global_Scale;
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = Circle_Size * AsConfig::Global_Scale;
   double alpha;

   Clear_BG(hdc);

   SelectObject(hdc, Pen_Pink);
   SelectObject(hdc, Brush_Pink);

   Ellipse(hdc, x, y, x + roller_size, y + roller_size);

   SetGraphicsMode(hdc, GM_ADVANCED);

   alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step;

   xform.eM11 = (float)cos(alpha); 
   xform.eM12 = (float)sin(alpha); 
   xform.eM21 = (float)-sin(alpha); 
   xform.eM22 = (float)cos(alpha); 
   xform.eDx = (float)(x + roller_size / 2); 
   xform.eDy = (float)(y + roller_size / 2); 
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2, AsConfig::Global_Scale / 2, roller_size / 2);

   SetWorldTransform(hdc, &old_xform);

   Draw_Circle_Highlight(hdc, paint_area, x, y);

   ++Rolling_Step;

   if(Rolling_Step >= Max_Rolling_Step)
   {
      Rolling_Step = 0;
   }

   X_Pos -= Rolling_Platform_Speed;

   if (X_Pos <= Roll_In_Platform_End_X_Pos)
   {
      X_Pos += Rolling_Platform_Speed;
      Platform_State = EPS_Expand_Roll_In;
      Inner_Width = 1;
   }
}

void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area)
{
   //Рисуем расширяющуюся платформу
   Draw_Normal_State(hdc, paint_area);

   --X_Pos;
   Inner_Width += 2;

   if (Inner_Width >= Normal_Platform_Inner_Width)
   {
      Inner_Width = Normal_Platform_Inner_Width;
      Platform_State = EPS_Ready;
      Redraw();                        
   }
}

void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   switch (Platform_State)
   {
   case EPS_Missing:
      break;
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;
   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;
   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;
   }

}
