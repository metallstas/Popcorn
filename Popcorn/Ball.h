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
   static const double Radius;

   void Init();
   void Draw(HDC hdc, RECT &paint_area);
   void Move();
   EBall_State Get_Satet();
   void Set_State(EBall_State new_state, double x_pos);
   double Get_Direction();
   void Set_Direction(double new_direction);

   static void Add_Hit_Checker(AHit_Checker *hit_checker);

private:

   RECT Ball_Rect, Prev_Ball_Rect;
   HPEN Ball_Pen_White;
   HBRUSH Ball_Brush_White;

   static const double Start_Ball_Y_Pos;
   static int Hit_Checkers_Count;
   double Ball_Direction;

   static AHit_Checker *Hit_Checkers[3];

   double Center_X_Pos, Center_Y_Pos;
   double Ball_Speed;
   double Rest_Distance;

   void Redraw();

};
