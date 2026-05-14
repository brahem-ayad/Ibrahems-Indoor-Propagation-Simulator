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
#include "Draw_Ray_Launching.h"

static void Draw_Simulation_State(Camera2D camera2, Camera3D &camera3, Camera3D &camerafps, Font font_32, Shader shader, Shader grid_shader, Shader ITU_Shader, int ITU_Shader_Uniform_LoS_ID, Shader Points_Shader){

  //DrawSphere(camera3.target, 0.1, RED);

  if(IsKeyPressed(KEY_L)){
    if(CONF::FPS_View) {
      CONF::FPS_View = false;
      EnableCursor();
      CONF::FPS_View_Started = false;
    }
    else {
      CONF::FPS_View = true;
      camerafps.position = camera3.position;
      player.position = camera3.position;
      player.isGrounded = false;
      player.dir = Vector3Normalize(Vector3Subtract(camera3.target, camera3.position));
      camerafps.target = camera3.target;
    }
  }

  if(CONF::FPS_View){
    Update_FPS_Camera(camerafps);
    BeginMode3D(camerafps);
  }
  else{
    Update_3D_Camera(camera3);
    BeginMode3D(camera3);
  }

  //BeginShaderMode(grid_shader);
  //  Draw_3D_Grid();
  //EndShaderMode();

  Draw_Floors_3D();

  Draw_Walls_3D(camera3, shader);

  Draw_Ceilings_3D();

  if(IsKeyPressed(KEY_R)){
    CONF::Simulation_Model = SM_RAY_LAUNCHING;
  }

  if(CONF::Simulation_Model == SM_ITU) Draw_ITU_Rays(ITU_Shader, camera3, ITU_Shader_Uniform_LoS_ID);
  else if(CONF::Simulation_Model == SM_RAY_LAUNCHING) Draw_RAY_LAUNCHING(Points_Shader);

  EndMode3D();

  if(CONF::FPS_View){
    if(!CONF::FPS_View_Started) DrawText("Click anywhere to enter FPS mode", 20, GetScreenHeight() - 30, 20, GRAY);
    else DrawText("Press Escape to exit FPS mode", 20, GetScreenHeight() - 30, 20, GRAY);
  }

  Draw_Simulation_Tool_Bar(font_32);

  Draw_View_Gimbal(font_32, 32, camera2, camera3);
}
