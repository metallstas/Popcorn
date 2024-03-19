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


   void Init();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(int platform_x_pos, int platform_width, ALevel *level);
   EBall_State Get_Satet();
   void Set_State(EBall_State new_state, int x_pos);

private:

   RECT Ball_Rect, Prev_Ball_Rect;
   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   static const double Start_Ball_Y_Pos;

   double Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed;
   double Ball_Direction;

   void Redraw();

};
