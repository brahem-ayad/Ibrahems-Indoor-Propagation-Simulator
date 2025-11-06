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

    Rectangle wall_rect(){
      if(end.x >= start.x and end.y >= start.y){
        return { start.x - thickness/2, start.y - thickness/2, end.x - start.x + thickness/2, end.y - start.y + thickness/2 };
      }
      else{
        return { end.x - thickness/2, end.y - thickness/2, start.x - end.x + thickness/2, start.y - end.y + thickness/2 };
      }
    }

    void draw() {
      //DrawLineEx(start, end, 5, WHITE);
      DrawRectangleLinesEx(wall_rect(), 1, WHITE);
    }
};
