#pragma once

#include <algorithm>
#include<raylib.h>
#include"../../Config.h"
#include"../../Utilities.h"
#include"Walls.h"
#include"../Floor_Plan.h"
#include"Wall_Tool_Tip.h"

// the function that draws the walls and the related UI as they are being placed
static void Draw_Wall_Placing(Camera2D camera2, Font font_20){

  // check that the mouse position is not at the top bar.
  if(CheckCollisionPointRec(GetWorldToScreen2D(Get_Snapped_Mouse_Position(camera2), camera2), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height}) == false){
  Vector2 Position;
  if(CONF::tool_state == None) Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
  else Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};
  if(CheckCollisionPointCircle(GetMousePosition(), Position, 70) == false){ // and not at the gimbal

    // Getting the snapped mouse position and Drawing a Blue circle at its place.
    Vector2 pos = Get_Snapped_Mouse_Position(camera2);
    if(!FP::is_starting_pos_available) DrawCircleV(GetWorldToScreen2D(pos, camera2), 5, BLUE);

    // If the start of the line has been specified. Draw a blue line between the start and the current mouse position.
    if(FP::is_starting_pos_available == true){

      Vector2 pos_2;
      if(IsKeyDown(KEY_LEFT_SHIFT)) pos_2 = Get_Snapped_Mouse_Position_Axial(camera2, FP::starting_pos);
      else pos_2 = pos;

      DrawCircleV(GetWorldToScreen2D(pos_2, camera2), 5, BLUE);

      if(CONF::Wall_Drawing_Shape == LINE){
        DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera2), GetWorldToScreen2D(pos_2, camera2), 3, BLUE);
        Draw_Wall_Length_Tooltip(pos_2, camera2, font_20);
      }
      else{
        DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera2), GetWorldToScreen2D({pos.x, FP::starting_pos.y}, camera2), 3, BLUE);
        DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera2), GetWorldToScreen2D({FP::starting_pos.x, pos.y}, camera2), 3, BLUE);
        DrawLineEx(GetWorldToScreen2D({pos.x, FP::starting_pos.y}, camera2), GetWorldToScreen2D(pos, camera2), 3, BLUE);
        DrawLineEx(GetWorldToScreen2D({FP::starting_pos.x, pos.y}, camera2), GetWorldToScreen2D(pos, camera2), 3, BLUE);

        DrawCircleV(GetWorldToScreen2D(FP::starting_pos, camera2), 5, BLUE);
        Draw_Area_Tooltip(pos, FP::starting_pos, camera2, font_20);
      }
 
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and !CONF::Moving_2d){
        if(CONF::Wall_Drawing_Shape == LINE or FP::starting_pos.x == pos.x or FP::starting_pos.y == pos.y){
          Wall Wall { FP::starting_pos, pos_2, CONF::Inputed_Wall_Height };
          FP::walls.push_back(Wall);
        }
        else{
          Wall W1 = {FP::starting_pos, {pos.x, FP::starting_pos.y}, CONF::Inputed_Wall_Height};
          Wall W2 = {FP::starting_pos, {FP::starting_pos.x, pos.y}, CONF::Inputed_Wall_Height};
          Wall W3 = {{pos.x, FP::starting_pos.y}, pos, CONF::Inputed_Wall_Height};
          Wall W4 = {{FP::starting_pos.x, pos.y}, pos, CONF::Inputed_Wall_Height};
          FP::walls.push_back(W1);
          FP::walls.push_back(W2);
          FP::walls.push_back(W3);
          FP::walls.push_back(W4);
          FP::is_starting_pos_available = false;
        }

        FP::starting_pos = pos_2;
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

  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::walls.size() > 0){
    // remove any door on the wall
    for(int i = 0; i < FP::walls[FP::walls.size() - 1].doors.size(); i++){
      int id = FP::walls[FP::walls.size() - 1].doors[i].ID;

      // remove the doors from the floor plan doors vector
      FP::doors.erase(
          std::remove_if(FP::doors.begin(), FP::doors.end(), [id](const Door& door) {
              return door.ID == id;
          }),
          FP::doors.end()
      );
      // remove them from the CONF doors on walls ids vector
      CONF::doors_on_walls_ids.erase(
          std::remove_if(CONF::doors_on_walls_ids.begin(), CONF::doors_on_walls_ids.end(), [id](const int& ID) {
          return ID == id;
          }),
          CONF::doors_on_walls_ids.end()
      );
    }
    // remove any window on the wall
    for(int i = 0; i < FP::walls[FP::walls.size() - 1].windows.size(); i++){
      int id = FP::walls[FP::walls.size() - 1].windows[i].ID;

      // remove the doors from the floor plan doors vector
      FP::windows.erase(
          std::remove_if(FP::windows.begin(), FP::windows.end(), [id](const Window& window) {
              return window.ID == id;
          }),
          FP::windows.end()
      );
      // remove them from the CONF doors on walls ids vector
      CONF::windows_on_walls_ids.erase(
          std::remove_if(CONF::windows_on_walls_ids.begin(), CONF::windows_on_walls_ids.end(), [id](const int& ID) {
          return ID == id;
          }),
          CONF::windows_on_walls_ids.end()
      );
    }

    FP::walls.pop_back();

    FP::is_starting_pos_available = false;
  }
}
