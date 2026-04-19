#pragma once

#include<raylib.h>
#include"../Floor_Plan.h"
#include"Walls.h"

static void Draw_Walls_2D(Camera2D camera2){
  for(int i = 0; i < FP::walls.size(); i++){
    FP::walls[i].Draw_2D(camera2);
  }
}

static void Draw_Walls_3D(Camera3D camera3, Shader shader){
  rlDisableBackfaceCulling();

  for(int i = 0; i < FP::walls.size(); i++){
    FP::walls[i].Draw_3D(camera3, shader);
  }

}
