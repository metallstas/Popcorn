#include "Engine.h"

AsEngine::AsEngine()
   :Game_State(EGS_Play_Level)
{
   
}


void AsEngine::Init_Engine(HWND hWnd)
{//Настройка игры при старте
   AsConfig::Hwnd = hWnd;

   AActive_Brick::Setup_Colors();
   int ball_x_pos = Platform.X_Pos + Platform.Width / 2;

   Level.Init();
   Platform.Init();
   Border.Init();
   Ball.Init();

   Ball.Set_State(EBS_Normal, ball_x_pos);

   Platform.Set_State(EPS_Normal); 
   Platform.Redraw();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//Отрисовка экрана игры
   
   Level.Draw(hdc, paint_area);
   
   
   //for (int i = 0; i < 16; i++)
   //{
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area);

   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);


}


int AsEngine::On_Key_Down(EKey_Type key_type)
{

   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;
      if (Platform.X_Pos <= AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;
      Platform.Redraw();
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;
      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;
      Platform.Redraw();
      break;

   case EKT_Space:
      if (Platform.Get_State() == EPS_Ready)
      {
         Platform.Set_State(EPS_Normal);
         Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
      }
      break;
   default:
      break;
   }

   return 0;
}


int AsEngine::On_Timer()
{
   ++AsConfig::Current_Timer_Tick;

   switch (Game_State)
   {
   case EGS_Play_Level:
      Ball.Move(&Level, &Border, &Platform);
      if (Ball.Get_Satet() == EBS_Lost)
      {
         Game_State = EGS_Lost_Ball;
         Platform.Set_State(EPS_Meltdown);
      }
      break;

   case EGS_Lost_Ball:
      if (Platform.Get_State() == EPS_Missing)
      {
         Game_State = EGS_Restart_Level;
         Platform.Set_State(EPS_Roll_In);
      }
      break;

   case EGS_Restart_Level:
      if (Platform.Get_State() == EPS_Ready)
      {
         Game_State = EGS_Play_Level;
         Ball.Set_State(EBS_On_Platform, Platform.X_Pos + Platform.Width / 2);

         break;
      }
   default:
      break;
   }

   Platform.Act();


   //Level.Active_Brick.Act();


   return 0;

}
