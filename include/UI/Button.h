#pragma once

#include<raylib.h>

#include"../Config.h"
#include"Main_Menu_Bar.h"
#include"../Floor_Planning/Floor_Plan.h"

class Tool_Bar_Button {
  private:
    char* text;
    Vector2 Position;
    Rectangle Rect;
    Color Rect_Color;
    Font font;
    float font_size;
    Color text_color;
    float offset;
    Selected_Tool tool_name;

  public:
    Tool_Bar_Button(char* text, Font font, float font_size, float offset, Selected_Tool tool_name) : text(text), font(font), font_size(font_size), offset(offset), tool_name(tool_name) {
      float margin = 10;
      Rect = {margin + offset, CONF::MMB_height + margin, 100, CONF::Tool_Bar_height - margin*2};
      if(CONF::Theme == Light_Theme) {
        Rect_Color = CONF::Ibo_Light_Gray;
        text_color = BLACK;
        if(CONF::tool_state == tool_name) Rect_Color = SKYBLUE;
      }
      else {
        Rect_Color = CONF::Ibo_Darkest_Gray;
        text_color = WHITE;
        if(CONF::tool_state == tool_name) { Rect_Color = SKYBLUE; text_color = BLACK; }
      }
    }

    void Draw(){
      Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
      float padding = 10;
      Rect.width = text_size.x + padding*2;
      Vector2 text_pos = {Rect.x + Rect.width/2 - text_size.x/2, Rect.y + Rect.height/2 - text_size.y/2};
      DrawRectangleRounded(Rect, 0.5, 10, Rect_Color);
      DrawTextEx(font, text, text_pos, font_size, 0, text_color);

      if(CheckCollisionPointRec(GetMousePosition(), Rect)){
        DrawRectangleRounded(Rect, 0.5, 10, Fade(WHITE, 0.2));
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          if(CONF::tool_state != tool_name) {
            CONF::tool_state = tool_name;
            FP::is_starting_pos_available = false;
            CONF::Floor_Start_Available = false;
            CONF::Ceiling_Start_Available = false;
          }
          else {
            CONF::tool_state = None;
            FP::is_starting_pos_available = false;
            CONF::Floor_Start_Available = false;
            CONF::Ceiling_Start_Available = false;
          }
        }
      }
    }

    float End(){ // returns the position of the right most point on the rectangle
      Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
      float padding = 10;
      Rect.width = text_size.x + padding*2;
      Vector2 text_pos = {Rect.x + Rect.width/2 - text_size.x/2, Rect.y + Rect.height/2 - text_size.y/2};
      return Rect.x + Rect.width;
    }
};

class Tool_Bar_Options_Switch {
  private:
    char* text;
    Vector2 Position;
    Rectangle Rect;
    Color Rect_Color;
    Font font;
    float font_size;
    Color text_color;
    float offset;
    bool &switchable_value;

  public:
    Tool_Bar_Options_Switch(char* text, Font font, float font_size, Vector2 Position, bool &switchable_value) : text(text), font(font),
    font_size(font_size), Position(Position), switchable_value(switchable_value) {
      Rect = {Position.x, 5 + Position.y, 100, CONF::Tool_Options_Bar_height - 10};
      if(CONF::Theme == Light_Theme) {
        Rect_Color = CONF::Ibo_Light_Gray;
        text_color = BLACK;
        if(switchable_value) Rect_Color = SKYBLUE;
      }
      else {
        Rect_Color = CONF::Ibo_Darkest_Gray;
        text_color = WHITE;
        if(switchable_value) { Rect_Color = SKYBLUE; text_color = BLACK; }
      }
    }

    void Draw(){
      Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
      float padding = 10;
      Rect.width = text_size.x + padding*2;
      Vector2 text_pos = {Rect.x + Rect.width/2 - text_size.x/2, Rect.y + Rect.height/2 - text_size.y/2};
      DrawRectangleRounded(Rect, 0.5, 10, Rect_Color);
      DrawTextEx(font, text, text_pos, font_size, 0, text_color);

      if(CheckCollisionPointRec(GetMousePosition(), Rect)){
        DrawRectangleRounded(Rect, 0.5, 10, Fade(WHITE, 0.2));
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          if(!switchable_value) switchable_value = true;
          else switchable_value = false;
        }
      }
    }

    float End(){ // returns the position of the right most point on the rectangle
      Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
      float padding = 10;
      Rect.width = text_size.x + padding*2;
      Vector2 text_pos = {Rect.x + Rect.width/2 - text_size.x/2, Rect.y + Rect.height/2 - text_size.y/2};
      return Rect.x + Rect.width;
    }
};

static void Draw_Walls_Line_Mode_Button(Vector2 Pos) {
  Rectangle Rect = {Pos.x + 20, 8 + Pos.y, 34, 34};

  Color Rect_Color;
  Color Icon_Color;
  if(CONF::Wall_Drawing_Shape == LINE) { Rect_Color = SKYBLUE; Icon_Color = CONF::Ibo_Darkest_Gray; }
  else if(CONF::Theme == Light_Theme) { Rect_Color = CONF::Ibo_Light_Gray; Icon_Color = CONF::Ibo_Darkest_Gray; }
  else { Rect_Color = CONF::Ibo_Darkest_Gray; Icon_Color = WHITE; }

  DrawRectangleRounded(Rect, 0.5, 15, Rect_Color);
  float padding = 10;
  Vector2 l_start = {Rect.x + padding, Rect.y + Rect.height - padding};
  Vector2 l_end = {Rect.x + Rect.width - padding, Rect.y + padding};
  DrawLineEx(l_start, l_end, 5, Icon_Color);
  DrawCircleV(l_start, 2.5, Icon_Color);
  DrawCircleV(l_end, 2.5, Icon_Color);

  if(CheckCollisionPointRec(GetMousePosition(), Rect)){
    DrawRectangleRounded(Rect, 0.5, 15, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      CONF::Wall_Drawing_Shape = LINE;
      FP::is_starting_pos_available = false;
    }
  }
}

static void Draw_Walls_Rect_Mode_Button(Vector2 Pos) {
  Rectangle Rect = {Pos.x + 20 + 34 + 20, 8 + Pos.y, 34, 34};

  Color Rect_Color;
  Color Icon_Color;
  if(CONF::Wall_Drawing_Shape == RECT) { Rect_Color = SKYBLUE; Icon_Color = CONF::Ibo_Darkest_Gray; }
  else if(CONF::Theme == Light_Theme) { Rect_Color = CONF::Ibo_Light_Gray; Icon_Color = CONF::Ibo_Darkest_Gray; }
  else { Rect_Color = CONF::Ibo_Darkest_Gray; Icon_Color = WHITE; }

  DrawRectangleRounded(Rect, 0.5, 15, Rect_Color);
  float padding = 5;
  DrawRectangleLinesEx({Rect.x + padding, Rect.y + padding, Rect.width - padding*2, Rect.height - padding*2}, 5, Icon_Color);

  if(CheckCollisionPointRec(GetMousePosition(), Rect)){
    DrawRectangleRounded(Rect, 0.5, 15, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      CONF::Wall_Drawing_Shape = RECT;
      FP::is_starting_pos_available = false;}

  }
}
