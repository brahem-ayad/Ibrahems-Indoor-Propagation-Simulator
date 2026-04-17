#pragma once

#include "../Cameras.h"
#include "../Config.h"
#include<raylib.h>

static void Draw_View_Gimbal(Font font, float font_size, Camera2D &camera2, Camera3D &camera3) {
  Vector2 Position;
  if(CONF::tool_state == None) Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
  else Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};

  Color Inner_Circle_Color;
  Color Outer_Circle_and_Triangles_Color;
  Color Text_Color;
  if(CONF::Theme == Light_Theme){
    Inner_Circle_Color = CONF::Ibo_Light_Gray;
    Outer_Circle_and_Triangles_Color = CONF::Ibo_Dark_Gray;
    Text_Color = BLACK;
  }
  else{
    Inner_Circle_Color = CONF::Ibo_Darkest_Gray;
    Outer_Circle_and_Triangles_Color = CONF::Ibo_Darker_Gray;
    Text_Color = WHITE;
  }

  char* text;
  if(CONF::View == View_2D) text = (char*)"2D";
  else text = (char*)"3D";

  DrawCircleV(Position, 50, Fade(GRAY, 0.2));

  DrawCircleV(Position, 28, Outer_Circle_and_Triangles_Color);
  DrawCircleV(Position, 24, Inner_Circle_Color);
  Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
  DrawTextEx(font, text, {Position.x - text_size.x/2, Position.y - text_size.y/2}, font_size, 0, Text_Color);
  if(CheckCollisionPointCircle(GetMousePosition(), Position, 28)){
    DrawCircleV(Position, 28, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_2D) { CONF::View = View_3D; CONF::tool_state = None; }
      else CONF::View = View_2D;
    }
  }


  float triangle_size = 18;
  // UP Triangle
  Vector2 V1 = {Position.x - triangle_size*2/3, Position.y - 50 + triangle_size};
  Vector2 V2 = {Position.x + triangle_size*2/3, Position.y - 50 + triangle_size};
  Vector2 V3 = {Position.x, Position.y - 50};
  DrawTriangle(V1, V2, V3, Outer_Circle_and_Triangles_Color);
  if(CheckCollisionPointTriangle(GetMousePosition(), V1, V2, V3)){
    DrawTriangle(V1, V2, V3, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Rotation) Rotate_3D_Camera_Around_Target_UP(camera3);
      else if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Movement) { Move_3D_Camera_UP(camera3); CAMERA::is_moving = true; }
      else camera2.target.y -= CAMERA::speed * 5;
    }
  }
  // RIGHT Triangle
  Vector2 V4 = {Position.x + 50 - triangle_size, Position.y - triangle_size*2/3};
  Vector2 V5 = {Position.x + 50 - triangle_size, Position.y + triangle_size*2/3};
  Vector2 V6 = {Position.x + 50, Position.y};
  DrawTriangle(V4, V5, V6, Outer_Circle_and_Triangles_Color);
  if(CheckCollisionPointTriangle(GetMousePosition(), V4, V5, V6)){
    DrawTriangle(V4, V5, V6, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Rotation) Rotate_3D_Camera_Around_Target_RIGHT(camera3);
      else if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Movement) { Move_3D_Camera_RIGHT(camera3); CAMERA::is_moving = true; }
      else camera2.target.x += CAMERA::speed * 5;
    }
  }
  // LEFT Triangle
  Vector2 V7 = {Position.x - 50 + triangle_size, Position.y + triangle_size*2/3};
  Vector2 V8 = {Position.x - 50 + triangle_size, Position.y - triangle_size*2/3};
  Vector2 V9 = {Position.x - 50, Position.y};
  DrawTriangle(V7, V8, V9, Outer_Circle_and_Triangles_Color);
  if(CheckCollisionPointTriangle(GetMousePosition(), V7, V8, V9)){
    DrawTriangle(V7, V8, V9, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Rotation) Rotate_3D_Camera_Around_Target_LEFT(camera3);
      else if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Movement) { Move_3D_Camera_LEFT(camera3); CAMERA::is_moving = true; }
      else camera2.target.x -= CAMERA::speed * 5;
    }
  }
  // DOWN Triangle
  Vector2 V10 = {Position.x + triangle_size*2/3, Position.y + 50 - triangle_size};
  Vector2 V11 = {Position.x - triangle_size*2/3, Position.y + 50 - triangle_size};
  Vector2 V12 = {Position.x, Position.y + 50};
  DrawTriangle(V10, V11, V12, Outer_Circle_and_Triangles_Color);
  if(CheckCollisionPointTriangle(GetMousePosition(), V10, V11, V12)){
    DrawTriangle(V10, V11, V12, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Rotation) Rotate_3D_Camera_Around_Target_DOWN(camera3);
      else if(CONF::View == View_3D and CONF::Gimbal_Arrows_3D_Mode == Movement) { Move_3D_Camera_DOWN(camera3); CAMERA::is_moving = true; }
      else camera2.target.y += CAMERA::speed * 5;
    }
  }

  // Zoom Keys
  Vector2 Zoom_Out_Key_Pos = {Position.x - 35, Position.y - 35};
  DrawCircleV(Zoom_Out_Key_Pos, 15, Outer_Circle_and_Triangles_Color);
  DrawCircleV(Zoom_Out_Key_Pos, 12, Inner_Circle_Color);
  Vector2 Zoom_Minus_Sign_Size = {12, 4};
  DrawRectangle(Zoom_Out_Key_Pos.x - Zoom_Minus_Sign_Size.x/2, Zoom_Out_Key_Pos.y - Zoom_Minus_Sign_Size.y/2, Zoom_Minus_Sign_Size.x, Zoom_Minus_Sign_Size.y, Text_Color);
  if(CheckCollisionPointCircle(GetMousePosition(), Zoom_Out_Key_Pos, 15)){
    DrawCircleV(Zoom_Out_Key_Pos, 15, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D) {
        if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_Out(camera3);
        else CAMERA::Orthographic_fovy += CAMERA::zoom_speed * GetFrameTime();
      }
      else camera2.zoom -= CAMERA::zoom_speed/10 * GetFrameTime();
    }
  }

  Vector2 Zoom_In_Key_Pos = {Position.x + 35, Position.y - 35};
  DrawCircleV(Zoom_In_Key_Pos, 15, Outer_Circle_and_Triangles_Color);
  DrawCircleV(Zoom_In_Key_Pos, 12, Inner_Circle_Color);
  DrawRectangle(Zoom_In_Key_Pos.x - Zoom_Minus_Sign_Size.x/2, Zoom_In_Key_Pos.y - Zoom_Minus_Sign_Size.y/2, Zoom_Minus_Sign_Size.x, Zoom_Minus_Sign_Size.y, Text_Color);
  DrawRectangle(Zoom_In_Key_Pos.x - Zoom_Minus_Sign_Size.y/2, Zoom_In_Key_Pos.y - Zoom_Minus_Sign_Size.x/2, Zoom_Minus_Sign_Size.y, Zoom_Minus_Sign_Size.x, Text_Color);
  if(CheckCollisionPointCircle(GetMousePosition(), Zoom_In_Key_Pos, 15)){
    DrawCircleV(Zoom_In_Key_Pos, 15, Fade(WHITE, 0.2));
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      if(CONF::View == View_3D) {
        if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_In(camera3);
        else CAMERA::Orthographic_fovy -= CAMERA::zoom_speed * GetFrameTime();
      }
      else camera2.zoom += CAMERA::zoom_speed/10 * GetFrameTime();
    }
  }

  // The Movement/Rotation Switcher
  if(CONF::View == View_3D){
    Vector2 Rotate_or_Move_with_Arrows_Switch = {Position.x + 35, Position.y + 35};
    DrawCircleV(Rotate_or_Move_with_Arrows_Switch, 15, Outer_Circle_and_Triangles_Color);
    DrawCircleV(Rotate_or_Move_with_Arrows_Switch, 12, Inner_Circle_Color);
    char* text_R_or_M;
    if(CONF::Gimbal_Arrows_3D_Mode == Rotation) text_R_or_M = (char*)"R";
    else text_R_or_M = (char*)"M";
    Vector2 text_R_or_M_Size = MeasureTextEx(font, text_R_or_M, font_size, 0);
    Vector2 text_R_or_M_Pos = {Rotate_or_Move_with_Arrows_Switch.x - text_R_or_M_Size.x/2, Rotate_or_Move_with_Arrows_Switch.y - text_R_or_M_Size.y/2};
    DrawTextEx(font, text_R_or_M, text_R_or_M_Pos, font_size, 0, Text_Color);
    if(CheckCollisionPointCircle(GetMousePosition(), Rotate_or_Move_with_Arrows_Switch, 15)){
      DrawCircleV(Rotate_or_Move_with_Arrows_Switch, 15, Fade(WHITE, 0.2));
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CONF::Gimbal_Arrows_3D_Mode == Rotation) CONF::Gimbal_Arrows_3D_Mode = Movement;
        else CONF::Gimbal_Arrows_3D_Mode = Rotation;
      }
    }
  }

  // The Camera Projection Switcher
  if(CONF::View == View_3D){
    Vector2 Camera_Projection_Switch = {Position.x - 35, Position.y + 35};
    DrawCircleV(Camera_Projection_Switch, 15, Outer_Circle_and_Triangles_Color);
    DrawCircleV(Camera_Projection_Switch, 12, Inner_Circle_Color);
    char* Camera_Projection_Switch_Text;
    if(CONF::Camera_3D_Projection == Perspective) Camera_Projection_Switch_Text = (char*)"P";
    else Camera_Projection_Switch_Text = (char*)"O";
    Vector2 Camera_Projection_Switch_Text_Size = MeasureTextEx(font, Camera_Projection_Switch_Text, font_size, 0);
    Vector2 Camera_Projection_Switch_Text_Pos = {Camera_Projection_Switch.x - Camera_Projection_Switch_Text_Size.x/2, Camera_Projection_Switch.y - Camera_Projection_Switch_Text_Size.y/2};
    DrawTextEx(font, Camera_Projection_Switch_Text, Camera_Projection_Switch_Text_Pos, font_size, 0, Text_Color);
    if(CheckCollisionPointCircle(GetMousePosition(), Camera_Projection_Switch, 15)){
      DrawCircleV(Camera_Projection_Switch, 15, Fade(WHITE, 0.2));
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CONF::Camera_3D_Projection == Perspective) CONF::Camera_3D_Projection = Orthographic;
        else CONF::Camera_3D_Projection = Perspective;
      }
    }
  }

}



