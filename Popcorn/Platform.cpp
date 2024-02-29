#include "Platform.h"

AsPlatform::AsPlatform()
   :X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2), Width(28), Inner_Width(21), Pen_Pink(0), Pen_Blue(0), Brush_Pink(0),
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


void AsPlatform::Redraw(HWND hwnd)
{//Отрисовка экрана игры

   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = (Platform_Rect.left + Width) * AsConfig::Global_Scale;
   Platform_Rect.bottom = (Platform_Rect.top + Height) * AsConfig::Global_Scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);

}


//------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;
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

