﻿#include "Border.h"

AsBorder::AsBorder()
   :Border_Pen_Blue(0), Border_Pen_White(0), Border_Brush_Blue (0), Border_Brush_White(0)
{

}

void AsBorder::Init()
{
   AsConfig::Create_Pen_Brush(85, 255, 255, Border_Pen_Blue, Border_Brush_Blue);
   AsConfig::Create_Pen_Brush(255, 255, 255, Border_Pen_White, Border_Brush_White);

}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{//Рисуем элемент рамки

   int global_scale = AsConfig::Global_Scale;
   //Основная линия рамки
   SelectObject(hdc, Border_Pen_Blue);
   SelectObject(hdc, Border_Brush_Blue);

   if(top_border)
      Rectangle(hdc, x * global_scale, (y + 1) * global_scale, (4 + x) * global_scale, (4 + y) * global_scale);
   else
      Rectangle(hdc, (1 + x) * global_scale, y * global_scale, (4 + x) * global_scale, (4 + y) * global_scale);

   //Белая койма
   SelectObject(hdc, Border_Pen_White);
   SelectObject(hdc, Border_Brush_White);

   if(top_border)
      Rectangle(hdc, x * global_scale, y * global_scale, (4 + x) * global_scale, (1 + y) * global_scale);
   else
      Rectangle(hdc, (0 + x) * global_scale, (0 + y) * global_scale, (1 + x) * global_scale, (4 + y) * global_scale);

   //Черная точка

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   if (top_border)
      Rectangle(hdc, (2 + x) * global_scale, (2 + y) * global_scale, (3 + x) * global_scale, (3 + y) * global_scale);
   else
      Rectangle(hdc, (2 + x) * global_scale, (1 + y) * global_scale, (3 + x) * global_scale, (2 + y) * global_scale);

}


void AsBorder::Draw(HDC hdc, RECT &paint_area)
{//Рисуем все элементы рамки

 //Рамка слева
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 2, 1 + i * 4, false);
   }
   //Рамка справа
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 201, 1 + i * 4, false);
   }

   //Рамка сxверху
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 3 + i * 4, 0, true);
   }
}
