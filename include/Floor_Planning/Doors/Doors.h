#pragma once

#include<raylib.h>
#include"../../Config.h"

class Door {
  public:
    Vector2 Position; // center position
    float width;
    float height;
    int ID;

    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;

    void Draw() {
      Color color;
      if(CONF::Theme == Light_Theme) color = BLACK;
      else color = WHITE;
      DrawLineEx(P1, P2, 2, color);
      DrawLineEx(P2, P3, 2, color);
      DrawLineEx(P3, P4, 2, color);
      DrawLineEx(P4, P1, 2, color);
    }
};
