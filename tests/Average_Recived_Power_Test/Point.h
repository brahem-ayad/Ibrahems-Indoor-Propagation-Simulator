#pragma once

#include "raylib.h"

class Point{
  private:
    float speed = 5;

  public:
    Vector2 Position;
    float Radius;

    Point(Vector2 Position, float Radius){
      this->Position = Position;
      this->Radius = Radius;
    }

    void Draw() {
      DrawCircleV(Position, Radius, RED);
    }

    void Update() {
      if(IsKeyDown(KEY_RIGHT)){
        Position.x += speed * GetFrameTime() * 60;
      }
      if(IsKeyDown(KEY_LEFT)){
        Position.x -= speed * GetFrameTime() * 60;
      }
      if(IsKeyDown(KEY_UP)){
        Position.y -= speed * GetFrameTime() * 60;
      }
      if(IsKeyDown(KEY_DOWN)){
        Position.y += speed * GetFrameTime() * 60;
      }
    }
};
