#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>

class BRay {
  private:
    float distance;
    Vector2 start_position;
    std::deque<Vector2> lines;

  public:
    Vector2 position;
    Vector2 direction;
    float speed;

    float Power;

    BRay(Vector2 position, Vector2 direction, float speed){
      this->position = position;
      this->direction = Vector2Normalize(direction);
      this->speed = speed;
      this->start_position = position;
      lines.push_back(position);
    }

    float t = 0;

    void draw() {
      for(int i = 0; i < lines.size() - 1; i++){ // Previous Lines
        DrawLineEx(lines[i], lines[i+1], 1, {255,0,0, 10});
      }
      DrawLineEx(start_position, position, 1, {255,0,0, 10}); // Current Line Path

      DrawCircle(position.x, position.y, 1, {255,0,0, 255});
    }

    void update(Rectangle Wall_Rec, Vector2 Wall_Normal) {
      if(CheckCollisionCircleRec(position, 1, Wall_Rec)){
        direction = Vector2Reflect(direction, Wall_Normal);
        start_position = position;

        lines.push_back(position);
        }
      position += direction * GetFrameTime() * speed;

    }

};
