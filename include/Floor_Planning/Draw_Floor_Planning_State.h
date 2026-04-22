#pragma once

#include<raylib.h>
#include <raymath.h>
#include<vector>

#include"../UI/Button.h"
#include"../Config.h"
#include"../Cameras.h"
#include"../Grid.h"
#include"../UI/Text_Field.h"
#include "Ceilings/Ceilings_Tool_Options.h"
#include "Ceilings/Draw_Ceilings.h"
#include "Floors/Draw_Floors.h"
#include"Walls/Walls.h"
#include"../UI/Main_Menu_Bar.h"
#include"../Utilities.h"
#include"../UI/View_Gimbal.h"
#include"Floors/Floor_Placing.h"
#include"Walls/Draw_Walls.h"
#include"Walls/Wall_Placing.h"
#include"../UI/Status_Bar.h"
#include"Floor_Planning_ToolBar.h"
#include"Walls/Walls_Tool_Options.h"
#include"Ceilings/Ceiling_Placing.h"
#include"Doors/Doors_Tool_Options.h"
#include"Doors/Door_Placing.h"
#include"Doors/Draw_Doors.h"
#include "Windows/Draw_Windows.h"
#include"Windows/Windows_Tool_Options.h"
#include"Windows/Window_Placing.h"

static void Draw_Floor_Planning_State(Camera2D &camera2, Camera3D &camera3, Camera3D &camerafps, Font font_32, Font font_26, Font font_20, Texture2D texture, bool imageLoaded, Shader shader, Shader grid_shader){

  if(CONF::View == View_2D){
    Update_2D_Camera(camera2);

    BeginMode2D(camera2);

    Draw_2D_Grid(camera2);

    if(imageLoaded){
      DrawTexture(texture, 0, 0, Fade(WHITE, 0.5));
    }

    Draw_Floors_2D();
    Draw_Walls_2D(camera2);
    Draw_Doors_2D();
    Draw_Ceilings_2D();
    Draw_Windows_2D();

    EndMode2D();
  }
  else if(CONF::View == View_3D){

    if(IsKeyPressed(KEY_P)){
      if(CONF::FPS_View == true) CONF::FPS_View = false;
      else CONF::FPS_View = true;
    }

    if(CONF::FPS_View) {
      Update_FPS_Camera(camerafps);
      BeginMode3D(camerafps);
    }
    else {
      Update_3D_Camera(camera3);
      BeginMode3D(camera3);
    }

    //DrawSphere(camera3.target, 0.1, RED);

    BeginShaderMode(grid_shader);
      Draw_3D_Grid();
    EndShaderMode();

    Draw_Floors_3D();

    Draw_Walls_3D(camera3, shader);

    Draw_Ceilings_3D();

    EndMode3D();
  }

  Draw_View_Gimbal(font_32, 32, camera2, camera3);

  Draw_Floor_Planning_Tool_Bar(font_32, font_26, font_20, camera2);
  Draw_Floor_Planning_Status_Bar(font_32, camera2);

  if(CONF::tool_state == Walls_Tool) Draw_Walls_Tool_Options(font_26, 26);
  if(CONF::tool_state == Ceiling_Tool) Draw_Ceilings_Tool_Options(font_26, 26);
  if(CONF::tool_state == Doors_Tool) Draw_Doors_Tool_Options(font_26, 26);
  if(CONF::tool_state == Windows_Tool) Draw_Windows_Tool_Options(font_26, 26);

  if(CONF::tool_state == Walls_Tool) Draw_Wall_Placing(camera2, font_20);
  if(CONF::tool_state == Floor_Tool) Draw_Floor_Placing(camera2, font_20);
  if(CONF::tool_state == Ceiling_Tool) Draw_Ceiling_Placing(camera2, font_20);
  if(CONF::tool_state == Doors_Tool) Draw_Door_Placing(camera2, font_20);
  if(CONF::tool_state == Windows_Tool) Draw_Window_Placing(camera2, font_20);
}
