#pragma once

#include <raylib.h>
#include"../Config.h"

static bool Font_Loaded = false;

static void Draw_Main_Menu_Bar(Font font) {
  Color Background_Color;
  Color Text_Color;
  if(CONF::Theme == Light_Theme){
    Background_Color = {220, 220, 220, 255};
    Text_Color = BLACK;
  }
  else {
    Background_Color = {22, 22, 22, 255};
    Text_Color = WHITE;
  }

  // Main Background
  DrawRectangle(0, 0, GetScreenWidth(), CONF::MMB_height, Background_Color);
  DrawRectangle(0, CONF::MMB_height, GetScreenWidth(), 1, Fade(Background_Color, 0.2));

  // Item Background
  float padding = 10;
  if(CheckCollisionPointRec(GetMousePosition(), {2.5f + MeasureTextEx(font, "File", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "File", 20, 0).x + padding*2, 20})){
    DrawRectangleRec({2.5f + MeasureTextEx(font, "File", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "File", 20, 0).x + padding*2, 20}, Fade(CONF::Ibo_Dark_Gray, 0.2));
  }
  if(CheckCollisionPointRec(GetMousePosition(), {50 + 2.5f + MeasureTextEx(font, "Edit", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "Edit", 20, 0).x + padding*2, 20})){
    DrawRectangleRec({50 + 2.5f + MeasureTextEx(font, "Edit", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "Edit", 20, 0).x + padding*2, 20}, Fade(CONF::Ibo_Dark_Gray, 0.2));
  }
  if(CheckCollisionPointRec(GetMousePosition(), {100 + 2.5f + MeasureTextEx(font, "View", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "View", 20, 0).x + padding*2, 20})){
    DrawRectangleRec({100 + 2.5f + MeasureTextEx(font, "View", 20, 0).x/2 - padding, 2.5, MeasureTextEx(font, "View", 20, 0).x + padding*2, 20}, Fade(CONF::Ibo_Dark_Gray, 0.2));
  }

  DrawTextEx(font, "File", {2.5f + MeasureTextEx(font, "File", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
  DrawTextEx(font, "Edit", {50 + 2.5f + MeasureTextEx(font, "Edit", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
  DrawTextEx(font, "View", {100 + 2.5f + MeasureTextEx(font, "View", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
}
