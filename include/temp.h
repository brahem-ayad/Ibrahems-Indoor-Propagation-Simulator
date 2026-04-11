#pragma once

#include<raylib.h>
#include"Main_Menu_Bar.h"
#include"Floor_Plan.h"
#include"Config.h"

// Walls
Rectangle Walls_Tool_Rec = {10, MMB::height + Tool_Bar::height/2 - 20, 100, 40};

if(selected_tool != Walls_Tool and CheckCollisionPointRec(GetMousePosition(), Walls_Tool_Rec)){
  DrawRectangleRounded(Shift_Rec(Walls_Tool_Rec, 0.f, 3.f), 0.5, 10, CONF::Crimson);
  DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, LIGHTGRAY);
  Vector2 text_size = MeasureTextEx(font_32, "Walls", 32, 0);
  Vector2 text_pos = {Walls_Tool_Rec.x + Walls_Tool_Rec.width/2 - text_size.x/2, Walls_Tool_Rec.y + Walls_Tool_Rec.height/2 - text_size.y/2};
  DrawTextEx(font_32, "Walls", text_pos, 32, 0, CONF::Crimson);
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, Fade(WHITE, 0.2));
    selected_tool = Walls_Tool;
  }
}
else if(selected_tool == Walls_Tool){
  // the button
  DrawRectangleRounded(Shift_Rec(Walls_Tool_Rec, 0.f, 3.f), 0.5, 10, CONF::Crimson);
  DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, LIGHTGRAY);
  Vector2 text_size = MeasureTextEx(font_32, "Walls", 32, 0);
  Vector2 text_pos = {Walls_Tool_Rec.x + Walls_Tool_Rec.width/2 - text_size.x/2, Walls_Tool_Rec.y + Walls_Tool_Rec.height/2 - text_size.y/2};
  DrawTextEx(font_32, "Walls", text_pos, 32, 0, DARKGRAY);
  if(CheckCollisionPointRec(GetMousePosition(), Walls_Tool_Rec)){
    DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, CONF::Spicy_Pastel_Pink);
    Vector2 text_size = MeasureTextEx(font_32, "Walls", 32, 0);
    Vector2 text_pos = {Walls_Tool_Rec.x + Walls_Tool_Rec.width/2 - text_size.x/2, Walls_Tool_Rec.y + Walls_Tool_Rec.height/2 - text_size.y/2};
    DrawTextEx(font_32, "Walls", text_pos, 32, 0, CONF::Crimson);
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, Fade(WHITE, 0.2));
        selected_tool = None;
    }
  }

  // the mouse pointer
  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), MMB::height + height}) == false){
    Vector2 pos = Get_Snapped_Mouse_Position(camera);

    DrawCircleV(GetWorldToScreen2D(pos, camera), 5, BLUE);

    if(FP::is_starting_pos_available == true){
      DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera), GetWorldToScreen2D(pos, camera), 3, BLUE);

      // Wall Length Tool Tip
      Vector2 pos_screen = GetWorldToScreen2D(pos, camera);
      Vector2 wtt = {pos_screen.x+20, pos_screen.y-30}; // wtt = wall tool tip
      float length = Vector2Distance(FP::starting_pos, pos)/GRID::spacing;
      const char *text = TextFormat("Length = %.2f", length);
      Vector2 text_size = MeasureTextEx(font_20, text, 20, 1);
      float margin = 5;
      DrawRectangleRounded({wtt.x - margin, wtt.y - margin, text_size.x + margin*2, text_size.y + margin*2}, 0.5, 10, Fade(LIGHTGRAY, 0.5));

      DrawTextEx(font_20, text, wtt, 20, 1, BLACK);
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      if(FP::is_starting_pos_available == true) {
        Wall_2D Wall;
        Wall.Start = FP::starting_pos;
        Wall.End = pos;
        //Wall.Height = Something;
        FP::walls_2d.push_back(Wall);

        FP::starting_pos = pos;
      }
      else {
        FP::starting_pos = pos;
        FP::is_starting_pos_available = true;
      }
    }

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      FP::is_starting_pos_available = false;
    }

  }
}
else {
  DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, LIGHTGRAY);
  Vector2 text_size = MeasureTextEx(font_32, "Walls", 32, 0);
  Vector2 text_pos = {Walls_Tool_Rec.x + Walls_Tool_Rec.width/2 - text_size.x/2, Walls_Tool_Rec.y + Walls_Tool_Rec.height/2 - text_size.y/2};
  DrawTextEx(font_32, "Walls", text_pos, 32, 0, DARKGRAY);
}


// -------------------------

  // 3D View Button
  Rectangle View_3D_Option = {(float)GetScreenWidth() - 150 - 10, MMB::height + Tool_Bar::height/2 - 20, 150, 40};

  DrawRectangleRounded(View_3D_Option, 0.5, 10, LIGHTGRAY);
  if(CheckCollisionPointRec(GetMousePosition(), View_3D_Option)){
    DrawRectangleRounded(View_3D_Option, 0.5, 10, SKYBLUE);
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      DrawRectangleRounded(View_3D_Option, 0.5, 10, Fade(WHITE, 0.2));
      selected_tool = None;
      if(view == View_2D){
        view = View_3D;
      }
      else if(view == View_3D){
        view = View_2D;
      }
    }
  }
  if(view == View_2D){
    Vector2 text_size = MeasureTextEx(font_32, "3D View", 32, 0);
    Vector2 text_pos = {View_3D_Option.x + View_3D_Option.width/2 - text_size.x/2, View_3D_Option.y + View_3D_Option.height/2 - text_size.y/2};
    DrawTextEx(font_32, "3D View", text_pos, 32, 1, DARKGRAY);
  }
  else if(view == View_3D){
    Vector2 text_size = MeasureTextEx(font_32, "2D View", 32, 0);
    Vector2 text_pos = {View_3D_Option.x + View_3D_Option.width/2 - text_size.x/2, View_3D_Option.y + View_3D_Option.height/2 - text_size.y/2};
    DrawTextEx(font_32, "2D View", text_pos, 32, 1, DARKGRAY);
  }
