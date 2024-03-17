#pragma once

#include "Config.h"
#include "Level.h"

enum EBall_State
{
   EBS_Lost,
   EBS_Normal,
   EBS_On_Platform
};

class ABall
{
public:
   ABall();

   EBall_State Ball_State;

   double Ball_Direction;

   void Init();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(int platform_x_pos, int platform_width, ALevel *level);
   void Reset(int x_pos);

private:

   RECT Ball_Rect, Prev_Ball_Rect;
   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   static const double Start_Ball_Y_Pos;

   double Ball_Speed, Ball_X_Pos, Ball_Y_Pos;

};
