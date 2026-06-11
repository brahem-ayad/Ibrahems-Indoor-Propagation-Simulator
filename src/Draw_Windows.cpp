#include"../include/Draw_Windows.hpp"

#include<raylib.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Windows.hpp"
#include"../include/Config.hpp"

void Draw_Window_2D(WINDOW W) {
  float line_thickness;
  line_thickness = 1.0f;

  Color line_color;
  Color bg_color;

  line_color = BLACK;
  bg_color = Fade(LIGHTGRAY, 0.5);

  if(W.Material == Glass_Window) bg_color = Fade(SKYBLUE, 0.5);

  // the mid points for the line in the middle
  Vector2 P14 = Vector2Add(W.P1, Vector2Scale(Vector2Subtract(W.P2, W.P1), 0.5));
  Vector2 P23 = Vector2Add(W.P4, Vector2Scale(Vector2Subtract(W.P3, W.P4), 0.5));

  DrawTriangle(W.P3, W.P2, W.P1, bg_color);
  DrawTriangle(W.P1, W.P4, W.P3, bg_color);

  DrawLineEx(W.P1, W.P2, line_thickness, line_color);
  DrawLineEx(W.P2, W.P3, line_thickness, line_color);
  DrawLineEx(W.P3, W.P4, line_thickness, line_color);
  DrawLineEx(W.P4, W.P1, line_thickness, line_color);

  DrawLineEx(P14, P23, line_thickness, line_color);

  DrawCircleV(W.P1, line_thickness/2, line_color);
  DrawCircleV(W.P2, line_thickness/2, line_color);
  DrawCircleV(W.P3, line_thickness/2, line_color);
  DrawCircleV(W.P4, line_thickness/2, line_color);
}

void Draw_Windows_2D(){
  for(int i = 0; i < FP::Windows.size(); i++){
    Draw_Window_2D(FP::Windows[i]);
  }
}
