#pragma once

#include<raylib.h>

#include"../Config.h"
#include"Main_Menu_Bar.h"

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
          if(CONF::tool_state != tool_name) CONF::tool_state = tool_name;
          else CONF::tool_state = None;
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
