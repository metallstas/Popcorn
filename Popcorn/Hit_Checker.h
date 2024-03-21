#pragma once

#include "Ball.h"


class AHit_Checker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;

};