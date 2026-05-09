#pragma once

#include<raylib.h>
#include"../../Config.h"
#include"../../UI/Text_Field.h"
#include"../../UI/Button.h"

static void Draw_Ceilings_Tool_Options(Font font, float font_size) {
  Vector2 Pos = {0, CONF::MMB_height + CONF::Tool_Bar_height};

  Text_Field Ceiling_Height_Text_Field((char*)"Height", {20, Pos.y}, font, font_size, CONF::ceiling_height_field_text, CONF::ceiling_height_field_active, CONF::Inputed_Ceiling_Height);

  Ceiling_Height_Text_Field.Draw();

  Draw_Ceilingss_Rect_Mode_Button({Ceiling_Height_Text_Field.End_Y_Pos(), Pos.y});
}
