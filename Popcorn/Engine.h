#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Border.h"
#include "Level.h"
#include "Config.h"

enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};



const int Timer_ID = WM_USER + 1;

class AsEngine;
class ALevel;
class AsPlatform;

class ABall
{
public:
   ABall();

   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   static const int Ball_Size = 3;

   double Ball_Direction;

   void Init();
   void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
   void Move(ALevel *level, AsPlatform *platform, AsEngine *engine);

private:
   RECT Ball_Rect, Prev_Ball_Rect;

   double Ball_Speed;

   int Ball_X_Pos;
   int Ball_Y_Pos;

};


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

   static const int Y_Pos = 185;

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

   HPEN BG_Pen_Black;
   HBRUSH BG_Brush_Black;

   static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size;

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