#pragma once

#include "raylib.h"
#include "raymath.h"

class Wall {
  public:
    Vector2 Start;
    Vector2 End;
    Vector2 Normal;
    Vector2 LineVector;

    Wall(Vector2 Start, Vector2 End){
      this->Start = Start;
      this->End = End;
      this->Normal= Vector2Normalize({Start.y - End.y, End.x - Start.x});
      this->LineVector = {End.x - Start.x, End.y - Start.y};
    }

    void draw() {
      DrawLineV(Start, End, WHITE);
      DrawCircleV(Start, 2, WHITE);
      DrawCircleV(End, 2, WHITE);
    }
};
