#pragma once

#include<iostream>

#include<raylib.h>
#include<string>

class Text_Field {
  private:
    Vector2 Position;
    float Width;

    Color Background_Color;
    Color Border_Color;

    float Roundness;
    float Border_Thickness;

    Rectangle Field_Rect;

    std::string Text = "3";
    Font The_Font;
    float Font_Size;

    float Padding;

    Vector2 Text_Pos;

    bool Active = false;

  public:

    Text_Field(Vector2 pos, float width, Color background_color, Color border_color, float roundness, float border_thickness, Font font, float font_size, float padding) :
      Position(pos), Width(width), Background_Color(background_color), Border_Color(border_color), Roundness(roundness), Border_Thickness(border_thickness), The_Font(font), Font_Size(font_size), Padding(padding) {
        Field_Rect = {Position.x, Position.y, Width, Font_Size + Padding*2};
        Text_Pos = {Field_Rect.x + Padding, Field_Rect.y + Padding};
      }

    void Draw() {
      DrawRectangleRounded(Field_Rect, Roundness, 10, Background_Color);
      DrawRectangleRoundedLinesEx(Field_Rect, Roundness, 10, Border_Thickness, Border_Color);

      DrawTextEx(The_Font, Text.c_str(), Text_Pos, Font_Size, 0, Border_Color);

      if(CheckCollisionPointRec(GetMousePosition(), Field_Rect)){
        DrawRectangleRounded(Field_Rect, Roundness, 10, Fade(Border_Color, 0.1));
        DrawRectangleRoundedLinesEx(Field_Rect, Roundness, 10, Border_Thickness, Fade(Background_Color, 0.1));
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          Active = true;
        }
      }
      else{
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          Active = false;
        }
      }

      if(IsKeyPressed(KEY_ENTER)){
        Active = false;
      }

      if(Active){
        DrawRectangleRoundedLinesEx(Field_Rect, Roundness, 10, Border_Thickness, Fade(WHITE, 0.1));

        Vector2 Text_Size = MeasureTextEx(The_Font, Text.c_str(), Font_Size, 0);
        if(int(GetTime()) % 2 == 0){
          DrawRectangle(Text_Pos.x + Text_Size.x + 5, Text_Pos.y, 10, Text_Size.y, Fade(Border_Color, 0.8));
        }

        if(IsKeyPressed(KEY_BACKSPACE) and !Text.empty()){
          Text.pop_back();
        }
 
        int key = GetCharPressed();

        while (key > 0) {
          if ((key >= 48) && (key <= 57) && Text.size() < 6) {
            Text.push_back((char)key);
          }
          key = GetCharPressed();
        }
      }
    }
};
