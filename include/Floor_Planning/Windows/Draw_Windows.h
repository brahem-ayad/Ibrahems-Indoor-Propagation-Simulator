#pragma once

#include<raylib.h>
#include"../Floor_Plan.h"
#include"Windows.h"

static void Draw_Windows_2D(){
  for(int i = 0; i < FP::windows.size(); i++){
    FP::windows[i].Draw();
  }
}
