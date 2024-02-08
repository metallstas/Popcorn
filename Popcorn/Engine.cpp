#include "Engine.h"

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

enum EBrick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

HPEN Pen_Pink, Pen_Blue, Pen_White, Letter_Pen;
HBRUSH Brush_Pink, Brush_Blue, Brush_White;

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Size = 7;

int Inner_Width = 21;

char Level_01[14][12] =
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


void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}

//------------------------------------------------------------------------------------------------------------

void Init()
{//��������� ���� ��� ������

   Create_Pen_Brush(255, 85, 255, Pen_Pink, Brush_Pink);
   Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);
   Create_Pen_Brush(255, 255, 255, Pen_White, Brush_White);

   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
}
//------------------------------------------------------------------------------------------------------------


void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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

void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
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

void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{//����� �������� �����
   bool swith_color;
   double offset;
   double rotation_angle; //�������������� ���� � ���� ��������
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
      //���
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

      //�������� ����
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

      //���
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      offset_back_part = (int)round(offset);

      Rectangle(hdc, 0, 0 - brick_half_height - offset_back_part, Brick_Width * Global_Scale, brick_half_height - (int)offset);
      //�������� ����
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

void Draw_Level(HDC hdc)
{//����� ������
   int i, j;

   for (i = 0; i < 14; i++)
   {
      for (j = 0; j < 12; j++)
      {
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
      }
   }
}

//------------------------------------------------------------------------------------------------------------

void Draw_Platform(HDC hdc, int x, int y)
{
   //��������� ���������
   //������ ������� ������

   SelectObject(hdc, Pen_Pink);
   SelectObject(hdc, Brush_Pink);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   
   //������ ���� �� ������
   SelectObject(hdc, Pen_White);

   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x - 1 + Circle_Size) * Global_Scale, (y - 1 + Circle_Size) * Global_Scale,
      (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 2 + 1) * Global_Scale);

   //������ ����������� ��������������
   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);

}

//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{//��������� ������ ����

   //Draw_Level(hdc);

   //Draw_Platform(hdc, 100, 150);

   for (int i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
   }

   
}
