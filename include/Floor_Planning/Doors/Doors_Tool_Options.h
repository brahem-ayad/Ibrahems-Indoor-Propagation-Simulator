#pragma once

#include<raylib.h>
#include"../../Config.h"
#include"../../UI/Text_Field.h"
#include"../../UI/Button.h"

static void Draw_Doors_Tool_Options(Font font, float font_size) {
  Vector2 Pos = {0, CONF::MMB_height + CONF::Tool_Bar_height};

  Text_Field Door_Height_Text_Field((char*)"Height", {20, Pos.y}, font, font_size, CONF::door_height_field_text, CONF::door_height_field_active, CONF::Inputed_Door_Height);
  Text_Field Door_Width_Text_Field((char*)"Width", {Door_Height_Text_Field.End_Y_Pos() + 20, Pos.y}, font, font_size, CONF::door_width_field_text, CONF::door_width_field_active, CONF::Inputed_Door_Width);
  Tool_Bar_Options_Switch Door_Snapping_Switch((char*)"Snap to Grid", font, font_size, {Door_Width_Text_Field.End_Y_Pos() + 20, Pos.y}, CONF::Door_Snapping);

  Door_Height_Text_Field.Draw();
  Door_Width_Text_Field.Draw();
  Door_Snapping_Switch.Draw();
}
