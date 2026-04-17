#pragma once

#include<iostream>

#include<raylib.h>
#include<string>
#include"../Config.h"

class Text_Field {
  private:
    Vector2 Position;
    float Width = 80;
    Font font;
    float font_size;
    std::string &field_text;
    bool &Active;
    Rectangle Field_Rect;
    char* text;
    float &value_to_be_edited;

  public:

    Text_Field(char* text, Vector2 Position, Font font, float font_size, std::string &field_text, bool &active, float &value_to_be_edited) :
      text(text), Position(Position), font(font), font_size(font_size), field_text(field_text), Active(active),
      value_to_be_edited(value_to_be_edited) {}

    void Draw() {

      Vector2 text_size = MeasureTextEx(font, text, font_size, 0);
      Vector2 text_pos = {Position.x, Position.y + CONF::Tool_Options_Bar_height/2 - text_size.y/2};

      Color text_color;
      Color Background_Color;
      Color Border_Color;
      if(CONF::Theme == Light_Theme) {
        text_color = BLACK;
        Background_Color = RAYWHITE;
        Border_Color = CONF::Ibo_Darkest_Gray;
      }
      else {
        text_color = WHITE;
        Background_Color = CONF::Ibo_Darkest_Gray;
        Border_Color = CONF::Ibo_Dark_Gray;

      }

      DrawTextEx(font, text, text_pos, font_size, 0, text_color);

      float distance_between_text_and_field = 12;

      float padding = 2;
      Field_Rect = {text_pos.x + text_size.x + distance_between_text_and_field, text_pos.y - padding, Width, text_size.y + padding*2};

      DrawRectangleRounded(Field_Rect, 0.5, 10, Background_Color);
      DrawRectangleRoundedLinesEx(Field_Rect, 0.5, 10, 2, Border_Color);

      Vector2 field_text_size = MeasureTextEx(font, field_text.c_str(), font_size, 0);
      Vector2 field_text_pos = {Field_Rect.x + Field_Rect.width/2 -  field_text_size.x/2, Field_Rect.y + padding};

      DrawTextEx(font, field_text.c_str(), field_text_pos, font_size, 0, text_color);

      if(CheckCollisionPointRec(GetMousePosition(), Field_Rect)){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          Active = true;
        }
      }
      else{
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          Active = false;
          value_to_be_edited = std::stof(field_text);
        }
      }

      if(IsKeyPressed(KEY_ENTER)){
        Active = false;
        value_to_be_edited = std::stof(field_text);
      }

      if(Active){

        DrawRectangleRounded(Field_Rect, 0.5, 10, Fade(WHITE, 0.2));

        if(int(GetTime()) % 2 == 0){
          DrawRectangle(field_text_pos.x + field_text_size.x + 5, field_text_pos.y, 5, field_text_size.y, text_color);
        }

        if(IsKeyPressed(KEY_BACKSPACE) and !field_text.empty()){
          field_text.pop_back();
        }
 
        int key = GetCharPressed();

        while (key > 0) {
          if ( key >= 48 and key <= 57  or key == 46) {
            field_text.push_back((char)key);
          }
          key = GetCharPressed();
        }
      }
    }
};
