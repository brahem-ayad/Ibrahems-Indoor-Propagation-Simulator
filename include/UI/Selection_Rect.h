#pragma once

#include<raylib.h>
#include <raymath.h>
#include"../Config.h"
#include"../Utilities.h"
#include"../Floor_Planning/Floor_Plan.h"

static void Draw_Selection_Rect(Camera2D camera2) {
  if(CONF::tool_state == None){
  // check that the mouse position is not at the top bar.
  if(CheckCollisionPointRec(GetWorldToScreen2D(Get_Snapped_Mouse_Position(camera2), camera2), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height}) == false){
  Vector2 Position;
  if(CONF::tool_state == None) Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
  else Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};
  if(CheckCollisionPointCircle(GetMousePosition(), Position, 70) == false){ // and not at the gimbal

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      CONF::Selection_Rect_Start = GetScreenToWorld2D(GetMousePosition(), camera2);
    }

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      RectanglePoints S_Rect = GetRectanglePoints(CONF::Selection_Rect_Start, GetScreenToWorld2D(GetMousePosition(), camera2));

      if( S_Rect.P1.x != S_Rect.P4.x and S_Rect.P1.y != S_Rect.P4.y ){
        DrawRectanglePoints(S_Rect.P1, S_Rect.P4, BLUE, BLUE, 0.2, false);
      }

    }

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
      RectanglePoints S_Rect = GetRectanglePoints(CONF::Selection_Rect_Start, GetScreenToWorld2D(GetMousePosition(), camera2));
      for(int i = 0; i < FP::walls.size(); i++){

        // if shift is held down, add to the selection
        if(IsKeyDown(KEY_LEFT_SHIFT)){
          if(CheckCollisionLineRec(FP::walls[i].Start, FP::walls[i].End, S_Rect)){
            FP::walls[i].Selected = true;
          }
        }
        // else, make only the ones under the rect selected and everything else not.
        else{
          if(CheckCollisionLineRec(FP::walls[i].Start, FP::walls[i].End, S_Rect)){
            FP::walls[i].Selected = true;
          }
          else{
            FP::walls[i].Selected = false;
          }
        }
      }
    }

    // to select a single wall
    for(int i = 0; i < FP::walls.size(); i++){
      if(CheckCollisionCircleLine(GetScreenToWorld2D(GetMousePosition(), camera2), 5, FP::walls[i].Start, FP::walls[i].End)){
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
          FP::walls[i].Selected = true;
        }
      }
    }

    if(IsKeyPressed(KEY_ESCAPE)){
      for(int i = 0; i < FP::walls.size(); i++){
        FP::walls[i].Selected = false;
      }
    }

  }}}
}
