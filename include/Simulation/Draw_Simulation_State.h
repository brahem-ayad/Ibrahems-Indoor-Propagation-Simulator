#pragma once

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include"../Grid.h"
#include"../Floor_Planning/Floors/Draw_Floors.h"
#include"../Floor_Planning/Walls/Draw_Walls.h"
#include"../Floor_Planning/Ceilings/Draw_Ceilings.h"
#include"../UI/View_Gimbal.h"
#include"./Simulation_Tool_Bar.h"
#include"./Draw_ITU_Model.h"

static void Draw_Simulation_State(Camera2D camera2, Camera3D &camera3, Font font_32, Shader shader, Shader grid_shader, Shader ITU_Shader){

  //DrawSphere(camera3.target, 0.1, RED);

  Update_3D_Camera(camera3);
  BeginMode3D(camera3);

  BeginShaderMode(grid_shader);
    Draw_3D_Grid();
  EndShaderMode();

  Draw_Floors_3D();

  Draw_Walls_3D(camera3, shader);

  Draw_Ceilings_3D();

  Draw_ITU_Rays(ITU_Shader, camera3);

  EndMode3D();

  Draw_Simulation_Tool_Bar();

  Draw_View_Gimbal(font_32, 32, camera2, camera3);
}
