#pragma once

#include<raylib.h>
#include"../Config.h"
#include"../UI/Button.h"

static void Draw_Floor_Planning_Tool_Bar(Font font_32, Font font_26, Font font_20, Camera2D camera2) {
  // Tool Bar Main Background
  Color Background_Color;
  if(CONF::Theme == Light_Theme) Background_Color = {240, 240, 240, 200}; else Background_Color = {40, 40, 40, 200};

  DrawRectangle(0, CONF::MMB_height, GetScreenWidth(), CONF::Tool_Bar_height, Background_Color);
  DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height, GetScreenWidth(), 2, Fade(GRAY, 0.5));

  if(CONF::tool_state != None and CONF::tool_state != Floor_Tool){
    DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height + 2, GetScreenWidth(), CONF::Tool_Options_Bar_height, Background_Color);
    DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height + 2 + CONF::Tool_Options_Bar_height, GetScreenWidth(), 2, Fade(GRAY, 0.5));
  }

  if(CONF::View == View_2D){
    float margin = 10;
    float offset = 120;

    Tool_Bar_Button Walls_Button((char*)"Walls", font_32, 32, 0, Walls_Tool);
    Tool_Bar_Button Doors_Button((char*)"Doors", font_32, 32, Walls_Button.End(), Doors_Tool);
    Tool_Bar_Button Windows_Button((char*)"Windows", font_32, 32, Doors_Button.End(), Windows_Tool);
    Tool_Bar_Button Floor_Button((char*)"Floor", font_32, 32, Windows_Button.End(), Floor_Tool);
    Tool_Bar_Button Ceiling_Button((char*)"Ceiling", font_32, 32, Floor_Button.End(), Ceiling_Tool);

    Walls_Button.Draw();
    Doors_Button.Draw();
    Windows_Button.Draw();
    Floor_Button.Draw();
    Ceiling_Button.Draw();

  }

}
