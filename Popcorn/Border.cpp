#include "Border.h"

AsBorder::AsBorder()
   :Border_Pen_Blue(0), Border_Pen_White(0), Border_Brush_Blue (0), Border_Brush_White(0)
{

}

void AsBorder::Init()
{
   AsConfig::Create_Pen_Brush(85, 255, 255, Border_Pen_Blue, Border_Brush_Blue);
   AsConfig::Create_Pen_Brush(255, 255, 255, Border_Pen_White, Border_Brush_White);

}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN pen_black, HBRUSH brush_black)
{//������ ������� �����

   int global_scale = AsConfig::Global_Scale;
   //�������� ����� �����
   SelectObject(hdc, Border_Pen_Blue);
   SelectObject(hdc, Border_Brush_Blue);

   if(top_border)
      Rectangle(hdc, x * global_scale, (y + 1) * global_scale, (4 + x) * global_scale, (4 + y) * global_scale);
   else
      Rectangle(hdc, (1 + x) * global_scale, y * global_scale, (4 + x) * global_scale, (4 + y) * global_scale);

   //����� �����
   SelectObject(hdc, Border_Pen_White);
   SelectObject(hdc, Border_Brush_White);

   if(top_border)
      Rectangle(hdc, x * global_scale, y * global_scale, (4 + x) * global_scale, (1 + y) * global_scale);
   else
      Rectangle(hdc, (0 + x) * global_scale, (0 + y) * global_scale, (1 + x) * global_scale, (4 + y) * global_scale);

   //������ �����

   SelectObject(hdc, pen_black);
   SelectObject(hdc, brush_black);

   if (top_border)
      Rectangle(hdc, (2 + x) * global_scale, (2 + y) * global_scale, (3 + x) * global_scale, (3 + y) * global_scale);
   else
      Rectangle(hdc, (2 + x) * global_scale, (1 + y) * global_scale, (3 + x) * global_scale, (2 + y) * global_scale);

}


void AsBorder::Draw(HDC hdc, RECT &paint_area, HPEN &pen_black, HBRUSH &brush_black)
{//������ ��� �������� �����

 //����� �����
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 2, 1 + i * 4, false, pen_black, brush_black);
   }
   //����� ������
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 201, 1 + i * 4, false, pen_black, brush_black);
   }

   //����� �x�����
   for (int i = 0; i < 50; i++)
   {
      Draw_Element(hdc, 3 + i * 4, 0, true, pen_black, brush_black);
   }
}