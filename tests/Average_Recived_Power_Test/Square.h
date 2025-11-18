#pragma once

#include "raylib.h"

class Square{
  private:
    float Width;
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;
    Rectangle Rect;

    Square(Vector2 Top_Left_Position, float Width){
      this->P1 = Top_Left_Position;
      this->P2 = {Top_Left_Position.x + Width, Top_Left_Position.y};
      this->P3 = {Top_Left_Position.x, Top_Left_Position.y + Width};
      this->P4 = {Top_Left_Position.x + Width, Top_Left_Position.y + Width};
      this->Width = Width;
      this->Rect = {P1.x, P1.y, Width, Width};
    }

    void Draw(){
      DrawRectangleLines(P1.x, P1.y, Width, Width, WHITE);
    }

    void Update(Vector2 center, float radius, Rectangle rectangle){
      if(CheckCollisionCircleRec(center, radius, rectangle)){
        DrawRectangle(P1.x, P1.y, Width, Width, RED);
      }
    }
};
