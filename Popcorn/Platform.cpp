#include "Platform.h"

AsPlatform::AsPlatform()
   :X_Pos(AsConfig::Border_X_Offset), Meltdown_Y_Pos(0), X_Step(AsConfig::Global_Scale * 2), Platform_State(EPS_Normal), Width(28), Inner_Width(21), Pen_Pink(0), Pen_Blue(0), Brush_Pink(0),
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

void AsPlatform::Act(HWND hwnd)
{
   if(Platform_State != EPS_Meltdown)
   {
      Platform_State = EPS_Meltdown;
      Meltdown_Y_Pos = Platform_Rect.bottom;
   }

   if(Platform_State == EPS_Meltdown)
   {
      Redraw(hwnd);
   }
}

void AsPlatform::Redraw(HWND hwnd)
{//Отрисовка экрана игры

   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

   if(Platform_State == EPS_Meltdown)
   {
      Platform_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
   }

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);

}


//------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   //Отрисовка Платформы  в нормально мсостоянии

   RECT intersection_rect;

   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;
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

void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
   int i, j;
   int area_width, area_height;
   int x, y, y_offset;
   COLORREF pixel;
   COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);

   area_width = Width * AsConfig::Global_Scale;
   area_height = Height * AsConfig::Global_Scale + 1;

   for (i = 0; i < area_width; i++)
   {
      x = Platform_Rect.left + i;
      y_offset = AsConfig::Rand(AsConfig::Meltdown_Speed);

      for (j = 0; j < area_height; j++)
      {
         y = Meltdown_Y_Pos - j;

         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }

      for (j = 0; j < y_offset; j++)
      {
         y = Meltdown_Y_Pos - area_height +1 + j;

         SetPixel(hdc, x, y, bg_pixel);
      }
   }  
   ++Meltdown_Y_Pos;
}

void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{

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
   }

}
