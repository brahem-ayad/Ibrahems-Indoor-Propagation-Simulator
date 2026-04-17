#pragma once

#include<raylib.h>
#include"../Config.h"

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
