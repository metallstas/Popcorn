#pragma once

#include "Config.h"

enum EBall_State
{
   EBS_Lost,
   EBS_Normal,
   EBS_On_Platform
};

class ABall;

class AHit_Checker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;

};

class ABall
{
public:
   ABall();

   EBall_State Ball_State;
   double Ball_Direction;
   static const double Radius;

   void Init();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(AHit_Checker *level_hit_checker, AHit_Checker *border_hit_checker, AHit_Checker *platform_hit_checker);
   EBall_State Get_Satet();
   void Set_State(EBall_State new_state, double x_pos);

private:

   RECT Ball_Rect, Prev_Ball_Rect;
   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   static const double Start_Ball_Y_Pos;

   double Center_X_Pos, Center_Y_Pos;
   double Ball_Speed;
   double Rest_Distance;

   void Redraw();

};
