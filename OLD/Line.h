#pragma once

#include "raylib.h"
#include "raymath.h"

class Line{
  public:
    Vector2 Start;
    Vector2 End;
    Vector2 Normal;
    Vector2 MidPoint;
    Vector2 LineVector;

    Line(Vector2 Start, Vector2 End){
      this->Start = Start;
      this->End = End;
      this->MidPoint = {Start.x + (End.x - Start.x)/2, Start.y + (End.y - Start.y)/2};
      this->LineVector = Vector2Subtract(End, Start);
      this->Normal = Vector2Normalize({-LineVector.y, LineVector.x});
    }

    void draw(){
      DrawLineV(Start, End, WHITE);
      DrawCircleV(MidPoint, 2, YELLOW);
      DrawLineV(MidPoint, Vector2Add(MidPoint, Vector2Scale(Normal, 50)), YELLOW);
    }
};
