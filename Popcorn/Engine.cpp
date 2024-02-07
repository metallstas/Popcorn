#include "Engine.h"

enum EBrick_Type
{
   EBT_None,
   EBT_Purple,
   EBT_Blue
};

HPEN Pen_Purple, Pen_Blue, Pen_White;
HBRUSH Brush_Purple, Brush_Blue, Brush_White;

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
{//Настройка игры при старте

   Create_Pen_Brush(255, 85, 255, Pen_Purple, Brush_Purple);
   Create_Pen_Brush(85, 255, 255, Pen_Blue, Brush_Blue);
   Create_Pen_Brush(255, 255, 255, Pen_White, Brush_White);
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
   case EBT_Purple:
      pen = Pen_Purple;
      brush = Brush_Purple;
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

void Draw_Level(HDC hdc)
{//Вывод уровня
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
   //Отрисовка Платформы
   //Рисуем боковые шарики

   SelectObject(hdc, Pen_Purple);
   SelectObject(hdc, Brush_Purple);

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
void Draw_Frame(HDC hdc)
{//Отрисовка экрана игры

   //Draw_Level(hdc);

   //Draw_Platform(hdc, 100, 150);

   

   XFORM xform, old_xform;
   SetGraphicsMode(hdc, GM_ADVANCED);
   // Rotate
    
   xform.eM11 = (FLOAT)0.8660;
   xform.eM12 = (FLOAT)0.5000;
   xform.eM21 = (FLOAT)-0.5000;
   xform.eM22 = (FLOAT)0.8660;
   xform.eDx = (FLOAT)100.0;
   xform.eDy = (FLOAT)100.0;
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   SelectObject(hdc, Pen_Blue);
   SelectObject(hdc, Brush_Blue);

   Rectangle(hdc, 20 * Global_Scale, 100 * Global_Scale, (20 + Brick_Width) * Global_Scale, (100 + Brick_Height) * Global_Scale);

   SetWorldTransform(hdc, &old_xform);
}
