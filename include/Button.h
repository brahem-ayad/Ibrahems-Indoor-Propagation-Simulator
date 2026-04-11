#pragma once

#include<raylib.h>

#include"Config.h"

class Button {
  private:
    Vector2 Position;
    float Width;
    float Padding;
    Rectangle Button_Rect;
    Color Background_Color;
    Color Border_Color;
    Texture2D icon_texture;
    char *text;
    Vector2 text_size;
    float font_size;
    Vector2 Text_Pos;
    Vector2 Icon_Pos;
    Font font;
    float Icon_Size;
    float Roundness;
    float Border_Thickness;
    float icon_ratio = 5.f/6.f;

    Selected_Tool state;

  public:
    Button(Vector2 pos, float padding, Color background_color, Color border_color, float border_thickness, float roundness, char* text, float font_size, Texture2D icon_texture, Font font, Selected_Tool state)
      : Position(pos), Padding(padding), Background_Color(background_color), Border_Color(border_color), Border_Thickness(border_thickness), Roundness(roundness), text(text), font_size(font_size), icon_texture(icon_texture), font(font), state(state)
    {
      text_size = MeasureTextEx(font, text, font_size, 0);
      Width = text_size.x + icon_texture.width*text_size.y/icon_texture.height*icon_ratio + Padding*3;
      Button_Rect = {pos.x, pos.y, Width, text_size.y+Padding*2};
      Text_Pos = {Button_Rect.x + Padding, Button_Rect.y + Padding};
      Icon_Size = text_size.y/icon_texture.height * icon_ratio;
      Icon_Pos = {Text_Pos.x + text_size.x + Padding, Text_Pos.y + text_size.y/12 };
    }

    void Draw() {
      DrawRectangleRounded(Button_Rect, Roundness, 10, Background_Color);
      DrawRectangleRoundedLinesEx(Button_Rect, Roundness, 10, Border_Thickness, Border_Color);
      DrawTextEx(font, text, Text_Pos, font_size, 0, Border_Color);
      DrawTextureEx(icon_texture, Icon_Pos, 0, Icon_Size, Border_Color);

      if(CheckCollisionPointRec(GetMousePosition(), Button_Rect)){
        DrawRectangleRounded(Button_Rect, Roundness, 10, Fade(Border_Color, 0.1));
        DrawRectangleRoundedLinesEx(Button_Rect, Roundness, 10, Border_Thickness, Fade(Background_Color, 0.1));
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          CONF::tool_state = state;
        }
      }

      if(CONF::tool_state == state){
        Color Active_Color = SKYBLUE;
        DrawRectangleRounded(Button_Rect, Roundness, 10, Active_Color);
        DrawRectangleRoundedLinesEx(Button_Rect, Roundness, 10, Border_Thickness, DARKBLUE);
        DrawTextEx(font, text, Text_Pos, font_size, 0, DARKBLUE);
        DrawTextureEx(icon_texture, Icon_Pos, 0, Icon_Size, DARKBLUE);
      }
    }

    float width() {
      return Width;
    }

};


