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
class ALevel;
class AsPlatform;

class ABall
{
public:
   ABall();

   HPEN Pen_White;
   HBRUSH Brush_White;

   static const int Ball_Size = 3;

   double Ball_Direction;

   void Init();
   void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
   void Move(ALevel *level, AsPlatform *platform, AsEngine *engine);

private:
   RECT Ball_Rect, Prev_Ball_Rect;

   double Ball_Speed;

   int Ball_X_Pos;
   int Ball_Y_Pos;

};

class ALevel
{
public:
   ALevel();


   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Level_Width = 12;
   static const int Level_Height = 14;

   void Init();
   void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
   void Draw_Level(HDC hdc, RECT &paint_area, AsEngine *engine);


private:
   HPEN Letter_Pen, Pen_Pink, Pen_Blue;
   HBRUSH Brush_Pink, Brush_Blue;
   RECT Level_Rect;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;

   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type, AsEngine *engine);
   void Set_Brick_Letter_Color(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};


class AsPlatform
{
public:

   AsPlatform();
   void Init();
   void Redraw_Platform(AsEngine *engine);
   void Draw_Platform(HDC hdc, RECT &paint_area);


   int X_Step;
   int Width;
   int X_Pos;

   static const int Y_Pos = 185;

private:
   HPEN Pen_Pink, Pen_Blue, Pen_Black, Pen_White;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Size = 7;

   int Inner_Width;
   static const int Height = 7;  



};


class AsBorder
{
public:
   void Init();

   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;

   void Draw_Bounds(HDC hdc, RECT paint_area, AsEngine *engine);

private:
   void Draw_Border(HDC hdc, int x, int y, bool top_border, AsEngine *engine);
};


class AsEngine
{
public:
   AsEngine();

   HWND Hwnd;

   HPEN Pen_Pink, Pen_Blue, Pen_White, Pen_Black;
   HBRUSH Brush_Pink, Brush_Blue, Brush_Black, Brush_White;

   static const int Global_Scale = 3;


   static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size;

   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);


   void Init_Engine(HWND hWnd); 
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

private:

   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
   
};