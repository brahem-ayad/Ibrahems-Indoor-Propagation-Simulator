#pragma once

#include<raylib.h>
#include"../Floor_Plan.h"
#include"Doors.h"

static void Draw_Doors_2D(){
  for(int i = 0; i < FP::doors.size(); i++){
    FP::doors[i].Draw();
  }
}
