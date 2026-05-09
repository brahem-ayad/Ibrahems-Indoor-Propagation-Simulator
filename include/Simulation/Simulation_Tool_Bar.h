#pragma once

#include<raylib.h>
#include"../Config.h"
#include"../UI/Drop_Down_Menu.h"

static void Draw_Simulation_Tool_Bar(Font font32) {
  // Simulation Bar Main Background
  Color Background_Color;
  if(CONF::Theme == Light_Theme) Background_Color = {240, 240, 240, 200}; else Background_Color = {40, 40, 40, 200};

  DrawRectangle(0, CONF::MMB_height, GetScreenWidth(), CONF::Tool_Bar_height, Background_Color);
  DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height, GetScreenWidth(), 2, Fade(GRAY, 0.5));

  // Secondary Bar
  //if(CONF::tool_state != None and CONF::tool_state != Floor_Tool){
  //  DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height + 2, GetScreenWidth(), CONF::Tool_Options_Bar_height, Background_Color);
  //  DrawRectangle(0, CONF::MMB_height + CONF::Tool_Bar_height + 2 + CONF::Tool_Options_Bar_height, GetScreenWidth(), 2, Fade(GRAY, 0.5));
  //}

  Drop_Down_Menu Simulation_Mode_DDM;
  Simulation_Mode_DDM.Draw(font32);

}
