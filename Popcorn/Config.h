﻿#pragma once

#include "Windows.h"
#define _USE_MATH_DEFINES
#include <math.h>

class AColor
{
public:
   unsigned char R,G,B;

   AColor(unsigned char r, unsigned char g, unsigned char b);

};

class AsConfig
{
public:
   AsConfig();

   static HWND Hwnd;

   static const AColor BG_Color, Pink_Brick, Blue_Brick, Ball_Color;
   static HPEN BG_Pen;
   static HBRUSH BG_Brush;

   static const int FPS = 20;
   static const int Ball_Size = 3;
   static const int Global_Scale = 3;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Platform_Y_Pos = 185;
   static const int Level_X_Offset = 8;
   static const int Max_Y_Pos = 199;
   static const int Cell_Width = 16;
   static const int Level_Width = 12;
   static const int Level_Height = 14;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
   static const int Level_Y_Offset = 6;
   static const int Cell_Height = 8;
   static const int Meltdown_Speed = 4;
   static int Current_Timer_Tick;

   static void Setup_Colors();
   static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &brush);
   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
   static int Rand(int range);

};
