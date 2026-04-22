#pragma once

#include<raylib.h>
#include"../../Config.h"
#include"../../UI/Text_Field.h"
#include"../../UI/Button.h"

static void Draw_Windows_Tool_Options(Font font, float font_size) {
  Vector2 Pos = {0, CONF::MMB_height + CONF::Tool_Bar_height};

  Text_Field Window_Base_Height_Text_Field((char*)"Base Height", {20, Pos.y}, font, font_size, CONF::window_base_height_field_text, CONF::window_base_height_field_active, CONF::Inputed_Window_Base_Height);
  Text_Field Window_Height_Text_Field((char*)"Window Height", {Window_Base_Height_Text_Field.End_Y_Pos() + 20, Pos.y}, font, font_size, CONF::window_height_field_text, CONF::window_height_field_active, CONF::Inputed_Window_Height);
  Text_Field Window_Width_Text_Field((char*)"Width", {Window_Height_Text_Field.End_Y_Pos() + 20, Pos.y}, font, font_size, CONF::window_width_field_text, CONF::window_width_field_active, CONF::Inputed_Window_Width);
  Tool_Bar_Options_Switch Window_Snapping_Switch((char*)"Snap to Grid", font, font_size, {Window_Width_Text_Field.End_Y_Pos() + 20, Pos.y}, CONF::Window_Snapping);

  Window_Base_Height_Text_Field.Draw();
  Window_Height_Text_Field.Draw();
  Window_Width_Text_Field.Draw();
  Window_Snapping_Switch.Draw();
}
