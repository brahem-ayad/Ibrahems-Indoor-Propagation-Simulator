#pragma once

#include<raylib.h>
#include <raymath.h>
#include"../Config.h"
#include"../Utilities.h"

class Drop_Down_Menu {
  public:
    Vector2 Size = {300, 40};
    Vector2 Position = {10, CONF::MMB_height + CONF::Tool_Bar_height/2 - Size.y/2};
    Rectangle Rect = {Position.x, Position.y, Size.x, Size.y};
    Color Background_Color = (CONF::Theme == Light_Theme) ? CONF::Ibo_Light_Gray : CONF::Ibo_Darkest_Gray;
    Color Items_Background_Color = (CONF::Theme == Light_Theme) ? CONF::Ibo_Lightly_Dark_Gray : CONF::Ibo_Darkly_Light_Gray;
    Color Text_Color = (CONF::Theme == Light_Theme) ? BLACK : WHITE;
    Vector2 Arrow_Pos;

    void Draw(Font font32){
      //DrawRectangleRounded(Rect, 0.5, 10, Background_Color);

      if(CONF::SM_Mode_DDM_Active){
        // Item 1 BG (ITU Model)
        DrawRectangleRec(Shift_Rec(Rect, 0, Size.y/2), Items_Background_Color);
        DrawRectangleRounded(Shift_Rec(Rect, 0, Size.y), 0.5, 10, Items_Background_Color);
        if(CONF::Simulation_Model == SM_ITU){
          DrawRectangleRounded(Scale_Rec_Centered(Shift_Rec(Rect, 0, Size.y), 15, 4), 0.5, 10, ORANGE);
        }

        int N_Items = 2;
        CONF::SM_Mode_DDM_Rect = {Rect.x, Rect.y, Rect.width, Rect.height * N_Items};
      }
      else {
        CONF::SM_Mode_DDM_Rect = Rect;
      }

      // the Items Text
      if(CONF::SM_Mode_DDM_Active){
        Vector2 Text_Size_1 = MeasureTextEx(font32, "ITU", 32, 0);
        Vector2 Text_Pos_1 = Vector2Subtract(Vector2Add({Position.x, Position.y + Size.y}, Vector2Scale(Size, 0.5)), Vector2Scale(Text_Size_1, 0.5));
        DrawTextEx(font32, "ITU", Text_Pos_1, 32, 0, Text_Color);
      }

      // Items Selection
      if(CONF::SM_Mode_DDM_Active){
        if(CheckCollisionPointRec(GetMousePosition(), Shift_Rec(Rect, 0, Size.y))){
          DrawRectangleRounded(Shift_Rec(Rect, 0, Size.y), 0.5, 10, Fade(WHITE, 0.2));
          if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(CONF::Simulation_Model == SM_ITU) CONF::Simulation_Model = SM_None;
            else CONF::Simulation_Model = SM_ITU;
          }
        }
      }

      // the main input button
      DrawRectangleRounded(Rect, 0.5, 10, Background_Color);
      Vector2 Text_Size_0 = MeasureTextEx(font32, "Simulation Model  -", 32, 0);
      Vector2 Text_Pos_0 = Vector2Subtract(Vector2Add(Position, Vector2Scale(Size, 0.5)), Vector2Scale(Text_Size_0, 0.5));
      Arrow_Pos = Vector2Add(Text_Pos_0, {Text_Size_0.x - 12, Text_Size_0.y/2 - 5});
      DrawTextEx(font32, "Simulation Model  -", Text_Pos_0, 32, 0, Text_Color);
      DrawRectangleV(Arrow_Pos, {20, 10}, Background_Color);
      if(CONF::SM_Mode_DDM_Active) DrawTriangle(Arrow_Pos, Vector2Add(Arrow_Pos, {20, 0}), Vector2Add(Arrow_Pos, {10, 10}), Text_Color);
      else DrawTriangle(Vector2Add(Arrow_Pos, {5, -5}), Vector2Add(Arrow_Pos, {15, 5}), Vector2Add(Arrow_Pos, {5, 15}), Text_Color);

      // Switching
      if(CheckCollisionPointRec(GetMousePosition(), Rect)){
        DrawRectangleRounded(Rect, 0.5, 10, Fade(WHITE, 0.2));
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          if(CONF::SM_Mode_DDM_Active) CONF::SM_Mode_DDM_Active = false;
          else CONF::SM_Mode_DDM_Active = true;
        }
      }



    }
};
