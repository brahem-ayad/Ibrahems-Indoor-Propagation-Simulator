#pragma once

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include"../../Config.h"

class Window {
  public:
    Vector2 Position; // center position
    float width;
    float base_height;
    float window_height;
    int ID;

    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;

    void Draw(Camera2D camera2) {
      float line_thickness;
      if(camera2.zoom < 1.0) line_thickness = Remap(Clamp(camera2.zoom, 0.1, 0.4), 0.1, 0.4, 10, 1.5);
      else line_thickness = Remap(camera2.zoom, 1.0, 7.0, 1.5, 0.2);

      Color line_color;
      Color bg_color;

      if(CONF::Theme == Light_Theme) {
        line_color = BLACK;
        bg_color = RAYWHITE;
      }
      else {
        line_color = WHITE;
        bg_color = BLACK;
      }

      // the mid points for the line in the middle
      Vector2 P14 = Vector2Add(P1, Vector2Scale(Vector2Subtract(P4, P1), 0.5));
      Vector2 P23 = Vector2Add(P2, Vector2Scale(Vector2Subtract(P3, P2), 0.5));

      // the extra points ofr the side black tirangles
      float w = 0.1 * GRID::spacing;
      Vector2 P1_2_w = Vector2Add(P1, Vector2Scale(Vector2Normalize(Vector2Subtract(P2, P1)), w));
      Vector2 P4_3_w = Vector2Add(P4, Vector2Scale(Vector2Normalize(Vector2Subtract(P3, P4)), w));
      Vector2 P2_1_w = Vector2Subtract(P2, Vector2Scale(Vector2Normalize(Vector2Subtract(P2, P1)), w));
      Vector2 P3_4_w = Vector2Subtract(P3, Vector2Scale(Vector2Normalize(Vector2Subtract(P3, P4)), w));

      // the white background
      DrawTriangle(P3, P2, P1, bg_color);
      DrawTriangle(P1, P4, P3, bg_color);

      // the blue background overlay
      DrawTriangle(P3, P2, P1, Fade(SKYBLUE, 0.5));
      DrawTriangle(P1, P4, P3, Fade(SKYBLUE, 0.5));

      // the black sides
      DrawTriangle(P4_3_w, P1_2_w, P1, line_color);
      DrawTriangle(P1, P4, P4_3_w, line_color);
      DrawTriangle(P3, P2, P2_1_w, line_color);
      DrawTriangle(P2_1_w, P3_4_w, P3, line_color);

      DrawLineEx(P1, P2, line_thickness, line_color);
      DrawLineEx(P2, P3, line_thickness, line_color);
      DrawLineEx(P3, P4, line_thickness, line_color);
      DrawLineEx(P4, P1, line_thickness, line_color);

      DrawLineEx(P14, P23, line_thickness, line_color);

      DrawCircleV(P1, line_thickness/2, line_color);
      DrawCircleV(P2, line_thickness/2, line_color);
      DrawCircleV(P3, line_thickness/2, line_color);
      DrawCircleV(P4, line_thickness/2, line_color);
    }
};
