#pragma once

#include<raylib.h>
#include"../../Config.h"
#include"../../Utilities.h"
#include"Walls.h"
#include"../Floor_Plan.h"
#include"Wall_Tool_Tip.h"

// the function that draws the walls and the related UI as they are being placed
static void Draw_Wall_Placing(Camera2D camera2, Font font_20){

  // check that the mouse position is not at the top bar.
  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height}) == false){
  Vector2 Position;
  if(CONF::tool_state == None) Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
  else Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};
  if(CheckCollisionPointCircle(GetMousePosition(), Position, 70) == false){ // and not at the gimbal

    // Getting the snapped mouse position and Drawing a Blue circle at its place.
    Vector2 pos = Get_Snapped_Mouse_Position(camera2);
    DrawCircleV(GetWorldToScreen2D(pos, camera2), 5, BLUE);

    // If the start of the line has been specified. Draw a blue line between the start and the current mouse position.
    if(FP::is_starting_pos_available == true){

      DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera2), GetWorldToScreen2D(pos, camera2), 3, BLUE);
      Draw_Wall_Length_Tooltip(pos, camera2, font_20);
 
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and !CONF::Moving_2d){
        Wall Wall;
        Wall.Start = FP::starting_pos;
        Wall.End = pos;
        Wall.Height = CONF::Inputed_Wall_Height;
        FP::walls.push_back(Wall);

        FP::starting_pos = pos;
      }

    }
    else {
      if(IsKeyPressed(KEY_ESCAPE)){
        CONF::tool_state = None;
      }
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and !CONF::Moving_2d){
        FP::starting_pos = pos;
        FP::is_starting_pos_available = true;
      }
    }

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) or IsKeyPressed(KEY_ESCAPE)){
      FP::is_starting_pos_available = false;
    }
  }}

  // Note
  // I feel like this would cause problems later when more tools are used, I want to also check which one was last used and undo based on that
  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::walls.size() > 0){
    FP::walls.pop_back();
  }
}
