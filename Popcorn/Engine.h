#pragma once

#include "Ball.h"
#include "Border.h"
#include "Level.h"

enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};



const int Timer_ID = WM_USER + 1;

class AsEngine; 

class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Redraw(AsEngine *engine);
   void Draw(HDC hdc, RECT &paint_area);


   int X_Step;
   int Width;
   int X_Pos;

private:
   HPEN Pen_Pink, Pen_Blue, Pen_Black, Pen_White;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Size = 7;

   int Inner_Width;
   static const int Height = 7;  



};


class AsEngine
{
public:
   AsEngine();

   HWND Hwnd;

   HPEN BG_Pen;
   HBRUSH BG_Brush;

   void Init_Engine(HWND hWnd); 
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

private:

   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
   
};