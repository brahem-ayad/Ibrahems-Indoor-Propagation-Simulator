#pragma once

#include "raylib.h"
#include "raymath.h"

class Wall {
  private:
    float thickness = 10;

  public:
    Vector2 start;
    Vector2 end;

    Wall(Vector2 start, Vector2 end){
      this->start = start;
      this->end = end;
    }

    Vector2 GetNormal() {
      return Vector2Normalize({start.y - end.y, end.x - start.x});
    }


    void draw() {
      DrawLineEx(start, end, thickness/2, WHITE);
    }
};
