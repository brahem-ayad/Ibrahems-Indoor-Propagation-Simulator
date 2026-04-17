#pragma once

#include"../Floor_Plan.h"
#include"../../Utilities.h"
#include <raylib.h>

static void Draw_Ceilings_2D() {
  for(int i = 0; i < FP::ceilings.size(); i++){
    DrawRectanglePoints(FP::ceilings[i].P1, FP::ceilings[i].P4, BLUE, SKYBLUE, 0.3, false);
  }
}

static void Draw_Ceilings_3D() {
  for(int i = 0; i < FP::ceilings.size(); i++){
    FP::ceilings[i].Draw_3D();
  }
}
