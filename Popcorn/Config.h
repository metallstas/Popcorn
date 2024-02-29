#pragma once

#include "Windows.h"
#define _USE_MATH_DEFINES
#include <math.h>

class AsConfig
{
public:
   AsConfig();

   static const int Ball_Size = 3;
   static const int Global_Scale = 3;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Platform_Y_Pos = 185;
   static const int Level_X_Offset = 8;
   static const int Max_Y_Pos = 199 - Ball_Size;
   static const int Cell_Width = 16;
   static const int Level_Width = 12;
   static const int Level_Height = 14;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   


   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);

};
