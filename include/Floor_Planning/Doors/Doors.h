#pragma once

#include<raylib.h>
#include<raymath.h>
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

    void Draw(Camera2D camera2) {
      Color color;
      Color bg_color;

      float line_thickness;
      if(camera2.zoom < 1.0) line_thickness = Remap(Clamp(camera2.zoom, 0.1, 0.4), 0.1, 0.4, 10, 1.5);
      else line_thickness = Remap(camera2.zoom, 1.0, 7.0, 1.5, 0.2);

      if(CONF::Theme == Light_Theme) {
        color = BLACK;
        bg_color = RAYWHITE;
      }
      else {
        color = WHITE;
        bg_color = BLACK;
      }

      DrawTriangle(P3, P2, P1, bg_color);
      DrawTriangle(P1, P4, P3, bg_color);

      DrawLineEx(P1, P2, line_thickness, color);
      DrawLineEx(P2, P3, line_thickness, color);
      DrawLineEx(P3, P4, line_thickness, color);
      DrawLineEx(P4, P1, line_thickness, color);

      DrawCircleV(P1, line_thickness/2, color);
      DrawCircleV(P2, line_thickness/2, color);
      DrawCircleV(P3, line_thickness/2, color);
      DrawCircleV(P4, line_thickness/2, color);
    }
};
