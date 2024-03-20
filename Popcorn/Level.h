#pragma once

#include "Config.h"
#include "Active_Brick.h"


enum ELetter_Type
{
   ELT_None,
   ELT_O
};

class ALevel
{
public:
   ALevel();

   AActive_Brick Active_Brick;

   void Init();
   void Check_Level_Brick_Hit(double &next_y_pos, double &ball_direction);
   void Draw(HDC hdc, RECT &paint_area);

private:
   HPEN Letter_Pen, Level_Pen_Pink, Level_Pen_Blue;
   HBRUSH Level_Brush_Pink, Level_Brush_Blue;
   RECT Level_Rect;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];

   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};
