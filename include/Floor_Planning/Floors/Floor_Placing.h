#pragma once

#include "../../Config.h"
#include "../../Utilities.h"
#include <raylib.h>
#include <raymath.h>
#include"../Floor_Plan.h"
#include<vector>
#include"../../Utilities.h"
#include"../Walls/Wall_Tool_Tip.h"

static void Draw_Floor_Placing(Camera2D camera2, Font font_20) {

  Vector2 pos_w = Get_Snapped_Mouse_Position(camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, camera2);

  if(CheckCollisionPointRec(GetWorldToScreen2D(Get_Snapped_Mouse_Position(camera2), camera2), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height}) == false){
  if(CheckCollisionPointCircle(GetWorldToScreen2D(Get_Snapped_Mouse_Position(camera2), camera2), {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50}, 70) == false){

    DrawCircleV(pos_s, 5, ORANGE);
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and !CONF::Moving_2d){
      if(CONF::Floor_Start_Available == false){
        CONF::Floor_Start_Available = true;
        CONF::Floor_Start_2D = pos_w;
      }
      else if(CONF::Floor_Start_Available == true){

        RectanglePoints points = GetRectanglePoints(GetWorldToScreen2D(CONF::Floor_Start_2D, camera2), pos_s);


        Vector2 P1 = GetScreenToWorld2D(points.P1, camera2);
        Vector2 P2 = GetScreenToWorld2D(points.P2, camera2);
        Vector2 P3 = GetScreenToWorld2D(points.P3, camera2);
        Vector2 P4 = GetScreenToWorld2D(points.P4, camera2);

        Floor floor(P1, P2, P3, P4);

        FP::floors.push_back(floor);

        CONF::Floor_Start_2D = pos_s;
        CONF::Floor_Start_Available = false;
      }
    }

    if(CONF::Floor_Start_Available) {
    DrawRectanglePoints(GetWorldToScreen2D(CONF::Floor_Start_2D, camera2), pos_s, ORANGE, YELLOW, 0.3, true);
    Draw_Area_Tooltip(pos_w, CONF::Floor_Start_2D, camera2, font_20);
    }

  }}

  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::floors.size() > 0){
      FP::floors.pop_back();
  }

  if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
    CONF::Floor_Start_Available = false;
  }

}
