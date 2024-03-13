﻿#include "Engine.h"

AsEngine::AsEngine()
   :Hwnd(0)
{
   
}


void AsEngine::Init_Engine(HWND hWnd)
{//Настройка игры при старте

   Hwnd = hWnd;
      
   AActive_Brick::Setup_Colors();
   Level.Init();
   Ball.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw(hWnd);

   SetTimer(Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//Отрисовка экрана игры
   
   Level.Draw(hdc, paint_area);
   
   Platform.Draw(hdc, paint_area);
   
   //for (int i = 0; i < 16; i++)
   //{
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area);

   Border.Draw(hdc, paint_area);

}


int AsEngine::On_Key_Down(EKey_Type key_type)
{

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;
      if (Platform.X_Pos <= AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;
      Platform.Redraw(Hwnd);
      break;
   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;
      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;
      Platform.Redraw(Hwnd);
      break;
   case EKT_Space:
      break;
   default:
      break;
   }

   return 0;
}


int AsEngine::On_Timer()
{
   ++AsConfig::Current_Timer_Tick;

   Ball.Move(Hwnd, Platform.X_Pos, Platform.Width, &Level);

   Level.Active_Brick.Act(Hwnd);

   Platform.Act(Hwnd);

   return 0;

}
