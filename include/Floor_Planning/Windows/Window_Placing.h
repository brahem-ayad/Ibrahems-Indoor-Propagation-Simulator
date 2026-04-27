#pragma once

#include "../../Config.h"
#include "../../Utilities.h"
#include <raylib.h>
#include <raymath.h>
#include"../Floor_Plan.h"
#include<vector>
#include"../../Utilities.h"
#include"Windows.h"

// I want to add the following
// If the windows height is more than the walls height don't add it
// or if its vertical positions is outside the wall or so on

// now if two windows are in the same 2D position you can't place them
// but I think there should be a check for height so that you can place two windows on a wall one on top of the other

static void Draw_Window_Placing(Camera2D camera2, Font font_20) {

  Vector2 pos_w;
  if(CONF::Window_Snapping) pos_w = Get_Snapped_Mouse_Position(camera2);
  else pos_w = GetScreenToWorld2D(GetMousePosition(), camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, camera2);

  bool not_near_any_wall = true;
  bool cant_place_window_on_wall = true;
  bool window_on_door = false;
  bool window_on_window = false;

  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height}) == false){
  if(CheckCollisionPointCircle(GetMousePosition(), {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50}, 70) == false){

    Vector2 p_pos_w = pos_w;
    Vector2 p_pos_s = pos_s;

    // check if a window is near a wall
    for(int i = 0; i < FP::walls.size(); i++){

      // check that the window is not on top of a door
      for(int j = 0; j < FP::doors.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Inputed_Window_Width/2 * GRID::spacing), FP::doors[j].Position, (FP::doors[j].width/2 * GRID::spacing)) == true){
          window_on_door = true;
        }
      }

      // check that the window is not on top of any other window
      for(int j = 0; j < FP::windows.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Inputed_Window_Width/2 * GRID::spacing), FP::windows[j].Position, (FP::windows[j].width/2 * GRID::spacing)) == true){
          window_on_window = true;
        }
      }

      if(window_on_door == false and window_on_window == false){
        if(CheckCollisionCircleLine(pos_w, (CONF::Inputed_Window_Width/2 * GRID::spacing)/5, FP::walls[i].Start, FP::walls[i].End)){
          not_near_any_wall = false;
          p_pos_w = Get_Closest_Point_on_Segment(pos_w, FP::walls[i].Start, FP::walls[i].End);
          p_pos_s = GetWorldToScreen2D(p_pos_w, camera2);
          DrawCircleV(p_pos_s, 5 * camera2.zoom, BLUE);
          float d1 = Vector2Distance(p_pos_w, FP::walls[i].Start) / GRID::spacing;
          float d2 = Vector2Distance(p_pos_w, FP::walls[i].End) / GRID::spacing;
          if(d1 > CONF::Inputed_Window_Width/2 and d2 > CONF::Inputed_Window_Width/2){
            DrawCircleV(p_pos_s, CONF::Inputed_Window_Width/2 * GRID::spacing * camera2.zoom, Fade(BLUE, 0.2));
            cant_place_window_on_wall = false;

            Vector2 dir = Vector2Normalize(Vector2Subtract(FP::walls[i].Start, FP::walls[i].End));

            Draw_Rectangle_Around_Lines(p_pos_w, dir, CONF::Inputed_Window_Width, 5, BLUE, camera2);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
              Window W;
              W.Position = p_pos_w;
              W.base_height = CONF::Inputed_Window_Base_Height;
              W.window_height = CONF::Inputed_Window_Height;
              W.width = CONF::Inputed_Window_Width;
              RectanglePoints RP = Get_Rectangle_Points_Around_Lines(p_pos_w, dir, CONF::Inputed_Window_Width, 5, camera2);
              W.P1 = RP.P1;
              W.P2 = RP.P2;
              W.P3 = RP.P3;
              W.P4 = RP.P4;
              W.ID = i;
              FP::windows.push_back(W);
              FP::walls[i].windows.push_back(W);
              CONF::windows_on_walls_ids.push_back(i);
            }
          }
        }
      }
    }

    if(not_near_any_wall){
      DrawCircleV(pos_s, 5 * camera2.zoom, RED);
    }
    if(cant_place_window_on_wall){
      DrawCircleV(p_pos_s, CONF::Inputed_Window_Width/2 * GRID::spacing * camera2.zoom, Fade(RED, 0.2));
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){}
  }}

  // Note : the code must be updated so that the walls and doors that are on that wall must be deleted as well
  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::windows.size() > 0){
      FP::windows.pop_back();
      FP::walls[CONF::windows_on_walls_ids.back()].windows.pop_back();
      CONF::windows_on_walls_ids.pop_back();
  }

}
