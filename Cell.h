#pragma once

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <math.h>

class Cell{
  public:
    Vector2 P1;
    float Width;
    Rectangle Rect;
    float Total_Power;
    Color color;

    Cell(Vector2 P1, float Width){
      this->P1 = P1;
      this->Width = Width;
      this->Rect = {P1.x, P1.y, Width, Width};
      this->Total_Power = 0;
      this->color = {0, 0, 255, 255};
    }

    void draw(){
      DrawRectangle(P1.x, P1.y, Width, Width, color);
    }

    void update(Vector2 center, float radius, float Power){
      if(CheckCollisionCircleRec(center, radius, Rect)){
        this->Total_Power += Power;
        color.r = std::clamp( (log10(Total_Power)+8)*100 , 0.0f, 255.0f);
        color.b = 255 - color.r;
      }
    }
};
