#pragma once

#include<raylib.h>
#include<vector>

#include"Config.h"
#include"Cameras.h"
#include"Grid.h"
#include"Walls.h"
#include"Main_Menu_Bar.h"
#include"Utilities.h"
#include"View_Gimbal.h"

static void Draw_Floor_Planning_Status_Bar(Font font, Camera2D camera);
static void Draw_Floor_Planning_Tool_Bar(Font font_32, Font font_20, Selected_Tool &selected_tool, Camera2D camera);

static void Draw_Floor_Planning_State(Camera2D &camera2, Camera3D &camera3,Font font_32, Font font_20, State &state, Texture2D texture, bool imageLoaded, Selected_Tool &selected_tool, Shader shader, Shader grid_shader){

  if(CONF::View == View_2D){
    Update_2D_Camera(camera2, state);

    BeginMode2D(camera2);

    Draw_2D_Grid(camera2);

    if(imageLoaded){
      DrawTexture(texture, 0, 0, Fade(WHITE, 0.5));
    }

    Draw_Walls_2D();

    EndMode2D();
  }
  else if(CONF::View == View_3D){

    Update_3D_Camera(camera3);

    BeginMode3D(camera3);

    DrawSphere(camera3.target, 0.1, RED);

    BeginShaderMode(grid_shader);
      Draw_3D_Grid();
    EndShaderMode();

    BeginShaderMode(shader);

    Draw_Walls_3D(camera3);

    EndShaderMode();

    EndMode3D();
  }

  Draw_View_Gimbal(font_32, 32, camera2, camera3);

  Draw_Floor_Planning_Tool_Bar(font_32, font_20, selected_tool, camera2);
  Draw_Floor_Planning_Status_Bar(font_32, camera2);
}

static void Draw_Floor_Planning_Status_Bar(Font font, Camera2D camera) {
  float height = 20;
  if(CONF::Theme == Light_Theme) DrawRectangle(0, GetScreenHeight()-height, GetScreenWidth(), GetScreenHeight(), Fade(LIGHTGRAY, 0.8));
  else DrawRectangle(0, GetScreenHeight()-height, GetScreenWidth(), GetScreenHeight(), Fade({20, 20, 20, 255}, 0.8));
  Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera);
  Color Text_Color;
  if(CONF::Theme == Light_Theme) Text_Color = DARKGRAY; else Text_Color = LIGHTGRAY;
  DrawTextEx(font, TextFormat("Mouse Position : {%.2f, %.2f}", pos.x/GRID::spacing, pos.y/GRID::spacing), {10, GetScreenHeight()-height+2}, 16, 1, Text_Color);
  DrawTextEx(font, TextFormat("Zoom : {%.1f}", camera.zoom), {(float)GetScreenWidth() - 96, GetScreenHeight()-height+2}, 16, 1, Text_Color);
}

namespace Tool_Bar {
  static float height = 60;
};

void Draw_Floor_Planning_Tool_Bar(Font font_32, Font font_20, Selected_Tool &selected_tool, Camera2D camera) {
  // Tool Bar Main Background
  Color Background_Color;
  if(CONF::Theme == Light_Theme) Background_Color = {240, 240, 240, 200}; else Background_Color = {40, 40, 40, 200};

  DrawRectangle(0, MMB::height, GetScreenWidth(), Tool_Bar::height, Background_Color);
  DrawRectangle(0, MMB::height + Tool_Bar::height, GetScreenWidth(), 2, Fade(GRAY, 0.5));

  if(CONF::View == View_2D){

  }

}
