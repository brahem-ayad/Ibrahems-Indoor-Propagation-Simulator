#pragma once

#include<raylib.h>
#include"../../Config.h"
#include"../../UI/Text_Field.h"

static void Draw_Walls_Tool_Options(Font font, float font_size) {
  Vector2 Pos = {0, CONF::MMB_height + CONF::Tool_Bar_height};

  Text_Field Wall_Height_Text_Field((char*)"Height", {20, Pos.y}, font, font_size, CONF::wall_height_field_text, CONF::wall_height_field_active, CONF::Inputed_Wall_Height);

  Wall_Height_Text_Field.Draw();

}
