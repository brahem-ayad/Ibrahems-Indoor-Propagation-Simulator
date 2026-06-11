#include"../include/Draw_Floors.hpp"

#include<raylib.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Utilities.hpp"

void Draw_Floors_2D() {
  for(int i = 0; i < FP::floors.size(); i++){
    DrawRectanglePoints(FP::floors[i].P1, FP::floors[i].P4, DARKGRAY, GRAY, 0.3, false);
  }
}

void Draw_Floors_3D() {
  for(int i = 0; i < FP::floors.size(); i++){
    FP::floors[i].Draw_3D();
  }
}
