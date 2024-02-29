#pragma once

#include "Windows.h"
#include "Config.h"

enum EBrick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

class ALevel
{
public:
   ALevel();

   static const int Level_Y_Offset = 6;
   static const int Cell_Height = 8;

   void Init();
   void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
   void Draw(HDC hdc, RECT &paint_area);


private:
   HPEN Letter_Pen, Level_Pen_Pink, Level_Pen_Blue;
   HBRUSH Level_Brush_Pink, Level_Brush_Blue;
   RECT Level_Rect;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;

   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};
