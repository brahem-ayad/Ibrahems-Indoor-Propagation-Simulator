#pragma once

#include "raylib.h"
#include "raymath.h"

class Wall{
  public:
    Vector2 Start;
    Vector2 End;
    Vector2 Normal;

    Wall(Vector2 Start, Vector2 End){
      this->Start = Start;
      this->End = End;
      this->Normal = Vector2Normalize({Start.y - End.y, End.x - Start.x});
    }

    void Draw() {
      DrawLineV(Start, End, WHITE);
    }
};
