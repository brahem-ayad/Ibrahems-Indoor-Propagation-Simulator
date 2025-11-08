#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>
#include <iostream>
#include <math.h>

class BRay {
  private:
    float distance;
    Vector2 start_position;
    std::deque<Vector2> lines;
    
    float Initial_Power;

  public:
    Color color = {255, 0, 0, 255};

    Vector2 position;
    Vector2 direction;
    float speed;

    float frequency;
    float wavelength;

    float Power;

    BRay(Vector2 position, Vector2 direction, float speed, float frequency ,float Power){
      this->position = position;
      this->direction = Vector2Normalize(direction);
      this->speed = speed;
      this->start_position = position;
      lines.push_back(position);

      this->distance = 0;

      this->wavelength = 0.125;

      this->Power = Power;
      this->Initial_Power = Power;
    }

    float t = 0;

    void draw() {
      //for(int i = 0; i < lines.size() - 1; i++){ // Previous Lines
      //  DrawLineEx(lines[i], lines[i+1], 1, color);
      //}
      //DrawLineEx(start_position, position, 1, color); // Current Line Path

      DrawCircle(position.x, position.y, 1, color);
    }

    void update(Vector2 Wall_Start, Vector2 Wall_End, Vector2 Wall_Normal) {
      if(CheckCollisionCircleLine(position, 3, Wall_Start, Wall_End)){
        direction = Vector2Reflect(direction, Wall_Normal);
        start_position = position;

        Power *= 0.8;
        Initial_Power = Power;
        lines.push_back(position);
        }
      else {
        distance = Vector2Distance(start_position, position);
        Power = Initial_Power * pow( wavelength/(4 * M_PI * distance/1000) ,2);
      }

      position += direction * GetFrameTime() * speed;

      color.r = (int)fmin(fmax((10*log10(Power/0.001) + 90)/110 * 255.0, 0.0), 255.0);


    }

};
