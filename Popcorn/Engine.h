#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

enum EBrick_Type
{
   EBT_None,
   EBT_Pink,
   EBT_Blue
};

const int Timer_ID = WM_USER + 1;

class AsEngine;
class ABall
{
public:
   ABall();

   static const int Ball_Size = 3;

   double Ball_Direction;

   void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
   void Move(AsEngine *engine);

private:
   RECT Ball_Rect, Prev_Ball_Rect;

   double Ball_Speed;

   int Ball_X_Pos;
   int Ball_Y_Pos;

};





class AsEngine
{
public:
   AsEngine();

   HWND Hwnd;

   HPEN Pen_Pink, Pen_Blue, Pen_White, Pen_Black, Letter_Pen;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   static const int Global_Scale = 3;
   static const int Platform_Y_Pos = 185;
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Level_Width = 12;
   static const int Level_Height = 14;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size;

   int Platform_X_Pos;
   int Platform_Width;

   void Init_Engine(HWND hWnd); 
   void Check_Level_Brick_Hit(int &next_y_pos);
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

private:

   ABall Ball;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
   static const int Circle_Size = 7;
   static const int Platform_Height = 7;

   int Inner_Width;
   int Platform_X_Step;
   
   RECT Platform_Rect, Prev_Platform_Rect;
   RECT Level_Rect;

   void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
   void Redraw_Platform();
   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
   void Draw_Level(HDC hdc);
   void Draw_Platform(HDC hdc, int x, int y);
   void Draw_Border(HDC hdc, int x, int y, bool top_border);
   void Draw_Bounds(HDC hdc, RECT paint_area);

};