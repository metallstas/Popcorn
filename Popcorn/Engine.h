﻿#pragma once

#include "Ball.h"
#include "Border.h"
#include "Level.h"
#include "Platform.h"

enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int Timer_ID = WM_USER + 1;

class AsEngine
{
public:
   AsEngine();

   void Init_Engine(HWND hWnd); 
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

private:
   HWND Hwnd;

   HPEN BG_Pen;
   HBRUSH BG_Brush;

   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
  
};
