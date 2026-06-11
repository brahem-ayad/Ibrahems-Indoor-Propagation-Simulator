#include"../include/Draw_Window_Placing.hpp"

#include<raylib.h>
#include<raymath.h>
#include<vector>
#include"../include/Config.hpp"
#include"../include/Utilities.hpp"
#include"../include/Floor_Plan.hpp"
#include"../include/Utilities.hpp"
#include"../include/Windows.hpp"

void Draw_Window_Placing() {

  Vector2 pos_w;
  if(CONF::Enable_Snapping) pos_w = Get_Snapped_World_Mouse_Position();
  else pos_w = GetScreenToWorld2D(GetMousePosition(), CONF::camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, CONF::camera2);

  bool not_near_any_wall = true;
  bool cant_place_window_on_wall = true;
  bool window_on_door = false;
  bool window_on_window = false;

  if(true){
  if(true){

    Vector2 p_pos_w = pos_w;
    Vector2 p_pos_s = pos_s;

    // check if a window is near a wall
    for(int i = 0; i < FP::Walls_Vec.size(); i++){

      // check that the window is not on top of a door
      for(int j = 0; j < FP::Doors.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Window_Width/2 * CONF::Grid_Spacing), FP::Doors[j].Position, (FP::Doors[j].Width/2 * CONF::Grid_Spacing)) == true){
          window_on_door = true;
        }
      }

      // check that the window is not on top of any other window
      for(int j = 0; j < FP::Windows.size(); j++){
        if(CheckCollisionCircles(pos_w, (CONF::Window_Width/2 * CONF::Grid_Spacing), FP::Windows[j].Position, (FP::Windows[j].Width/2 * CONF::Grid_Spacing)) == true){
          window_on_window = true;
        }
      }

      if(window_on_door == false and window_on_window == false){
        if(CheckCollisionCircleLine(pos_w, (CONF::Window_Width/2 * CONF::Grid_Spacing)/5, FP::Walls_Vec[i].Wall_Start_2D, FP::Walls_Vec[i].Wall_End_2D)){
          not_near_any_wall = false;
          p_pos_w = Get_Closest_Point_on_Segment(pos_w, FP::Walls_Vec[i].Wall_Start_2D, FP::Walls_Vec[i].Wall_End_2D);
          p_pos_s = GetWorldToScreen2D(p_pos_w, CONF::camera2);
          DrawCircleV(p_pos_s, 5 * CONF::camera2.zoom, BLUE);
          float d1 = Vector2Distance(p_pos_w, FP::Walls_Vec[i].Wall_Start_2D) / CONF::Grid_Spacing;
          float d2 = Vector2Distance(p_pos_w, FP::Walls_Vec[i].Wall_End_2D) / CONF::Grid_Spacing;
          if(d1 > CONF::Window_Width/2 and d2 > CONF::Window_Width/2){
            DrawCircleV(p_pos_s, CONF::Window_Width/2 * CONF::Grid_Spacing * CONF::camera2.zoom, Fade(BLUE, 0.2));
            cant_place_window_on_wall = false;

            Vector2 dir = Vector2Normalize(Vector2Subtract(FP::Walls_Vec[i].Wall_Start_2D, FP::Walls_Vec[i].Wall_End_2D));

            Draw_Rectangle_Around_Lines(p_pos_w, dir, CONF::Window_Width, 5, BLUE);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
              WINDOW W;
              W.Position = p_pos_w;
              W.base_height = CONF::Window_Base_Height;
              W.window_height = CONF::Window_Frame_Height;
              W.Width = CONF::Window_Width;
              W.Material = CONF::Window_Material;
              RectanglePoints RP = Get_Rectangle_Points_Around_Lines(p_pos_w, dir, CONF::Window_Width, 2);
              W.P1 = RP.P1;
              W.P2 = RP.P2;
              W.P3 = RP.P3;
              W.P4 = RP.P4;
              W.ID = i;
              FP::Windows.push_back(W);
              FP::Walls_Vec[i].Windows_Vec.push_back(W);
              FP::windows_on_walls_ids.push_back(i);
            }
          }
        }
      }
    }

    if(not_near_any_wall){
      DrawCircleV(pos_s, 5 * CONF::camera2.zoom, RED);
    }
    if(cant_place_window_on_wall){
      DrawCircleV(p_pos_s, CONF::Window_Width/2 * CONF::Grid_Spacing * CONF::camera2.zoom, Fade(RED, 0.2));
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){}
  }}

  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z) and FP::Windows.size() > 0){
      FP::Windows.pop_back();
      FP::Walls_Vec[FP::windows_on_walls_ids.back()].Windows_Vec.pop_back();
      FP::windows_on_walls_ids.pop_back();
  }

  if(IsKeyPressed(KEY_ESCAPE)){
    CONF::Selected_Tool = None_Tool;
  }

}
