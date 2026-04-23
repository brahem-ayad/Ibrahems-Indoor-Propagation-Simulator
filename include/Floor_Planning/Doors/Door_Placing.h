#pragma once

#include "../../Config.h"
#include "../../Utilities.h"
#include <raylib.h>
#include <raymath.h>
#include"../Floor_Plan.h"
#include<vector>
#include"../../Utilities.h"

static void Draw_Door_Placing(Camera2D camera2, Font font_20) {

  Vector2 pos_w;
  if(CONF::Door_Snapping) pos_w = Get_Snapped_Mouse_Position(camera2);
  else pos_w = GetScreenToWorld2D(GetMousePosition(), camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, camera2);

  bool not_near_any_wall = true;
  bool cant_place_door_on_wall = true;
  bool door_on_window = false;
  bool door_on_door = false;

  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height}) == false){
  if(CheckCollisionPointCircle(GetMousePosition(), {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50}, 70) == false){

    Vector2 p_pos_w = pos_w;
    Vector2 p_pos_s = pos_s;

    // check if a door is near a wall
    for(int i = 0; i < FP::walls.size(); i++){

      // check if a door is not on top of a window (that's on the same wall)
      for(int j = 0; j < FP::walls[i].windows.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Inputed_Door_Width/2 * GRID::spacing), FP::walls[i].windows[j].Position, (FP::walls[i].windows[j].width/2 * GRID::spacing)) == true){
          door_on_window = true;
        }
      }

      // check if a door is not on top of another door (that's on the same wall)
      for(int j = 0; j < FP::walls[i].doors.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Inputed_Door_Width/2 * GRID::spacing), FP::walls[i].doors[j].Position, (FP::walls[i].doors[j].width/2 * GRID::spacing)) == true){
          door_on_door = true;
        }
      }

      if(door_on_window == false and door_on_door == false){
        if(CheckCollisionCircleLine(pos_w, (CONF::Inputed_Door_Width/2 * GRID::spacing)/5, FP::walls[i].Start, FP::walls[i].End)){
          not_near_any_wall = false;
          p_pos_w = Get_Closest_Point_on_Segment(pos_w, FP::walls[i].Start, FP::walls[i].End);
          p_pos_s = GetWorldToScreen2D(p_pos_w, camera2);
          DrawCircleV(p_pos_s, 5 * camera2.zoom, BLUE);
          float d1 = Vector2Distance(p_pos_w, FP::walls[i].Start) / GRID::spacing;
          float d2 = Vector2Distance(p_pos_w, FP::walls[i].End) / GRID::spacing;
          if(d1 > CONF::Inputed_Door_Width/2 and d2 > CONF::Inputed_Door_Width/2){
            DrawCircleV(p_pos_s, CONF::Inputed_Door_Width/2 * GRID::spacing * camera2.zoom, Fade(BLUE, 0.2));
            cant_place_door_on_wall = false;

            Vector2 dir = Vector2Normalize(Vector2Subtract(FP::walls[i].Start, FP::walls[i].End));

            if(CONF::Theme == Light_Theme) Draw_Rectangle_Around_Lines(p_pos_w, dir, CONF::Inputed_Door_Width, 5, BLUE, camera2);
            else Draw_Rectangle_Around_Lines(p_pos_w, dir, CONF::Inputed_Door_Width, 5, BLUE, camera2);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
              Door D;
              D.Position = p_pos_w;
              D.height = CONF::Inputed_Door_Height;
              D.width = CONF::Inputed_Door_Width;
              RectanglePoints RP = Get_Rectangle_Points_Around_Lines(p_pos_w, dir, CONF::Inputed_Door_Width, 5, camera2);
              D.P1 = RP.P1;
              D.P2 = RP.P2;
              D.P3 = RP.P3;
              D.P4 = RP.P4;
              D.ID = i;
              FP::doors.push_back(D);
              FP::walls[i].doors.push_back(D);
              CONF::doors_on_walls_ids.push_back(i);
            }
          }
        }
      }
    }

    if(not_near_any_wall){
      DrawCircleV(pos_s, 5 * camera2.zoom, RED);
    }
    if(cant_place_door_on_wall){
      DrawCircleV(p_pos_s, CONF::Inputed_Door_Width/2 * GRID::spacing * camera2.zoom, Fade(RED, 0.2));
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){}
  }}

  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::doors.size() > 0){
      FP::doors.pop_back();
      FP::walls[CONF::doors_on_walls_ids.back()].doors.pop_back();
      CONF::doors_on_walls_ids.pop_back();
  }

}
