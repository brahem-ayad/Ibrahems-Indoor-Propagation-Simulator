#pragma once

#include <raylib.h>
#include"Config.h"

static bool Font_Loaded = false;

namespace MMB {
  static float height = 25.0f;
};

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
  DrawRectangle(0, 0, GetScreenWidth(), MMB::height, Background_Color);
  DrawRectangle(0, MMB::height, GetScreenWidth(), 1, Fade(Background_Color, 0.2));

  // Item Background
  DrawRectangle(0, 0, 100, MMB::height, Background_Color);
 
  DrawTextEx(font, "File", {2.5f + MeasureTextEx(font, "File", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
  DrawTextEx(font, "Edit", {50 + 2.5f + MeasureTextEx(font, "Edit", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
  DrawTextEx(font, "View", {100 + 2.5f + MeasureTextEx(font, "View", 20, 0).x/2, 2.5}, 20, 0, Text_Color);
}
