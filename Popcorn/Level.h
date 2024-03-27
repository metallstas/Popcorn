#pragma once

#include "Ball.h"
#include "Active_Brick.h"


enum ELetter_Type
{
   ELT_None,
   ELT_O
};

class ALevel: public AHit_Checker
{
public:
   ALevel();

   AActive_Brick Active_Brick;

   void Init();
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);
   void Draw(HDC hdc, RECT &paint_area);

private:
   HPEN Letter_Pen, Level_Pen_Pink, Level_Pen_Blue;
   HBRUSH Level_Brush_Pink, Level_Brush_Blue;
   RECT Level_Rect;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];

   bool Hit_Circle_On_Line(double y, double next_x_pos, double radius, double left_x, double right_x);
   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};
