#include"../include/Draw_Doors.hpp"

#include<raylib.h>
#include<raymath.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Doors.hpp"
#include "Config.hpp"

void Draw_Door_2D(DOOR Door) {
  Color color;
  Color bg_color;

  float line_thickness = 1.0f;

  color = BLACK;

  if      (Door.Material == Empty_Door)  bg_color = {235, 235, 235, 255};
  else if (Door.Material == Wooden_Door) bg_color = {217, 186, 140, 255};
  else if (Door.Material == Metal_Door)  bg_color = {102, 128, 153, 255};


  DrawTriangle(Door.P3, Door.P2, Door.P1, bg_color);
  DrawTriangle(Door.P1, Door.P4, Door.P3, bg_color);

  DrawLineEx(Door.P1, Door.P2, line_thickness, color);
  DrawLineEx(Door.P2, Door.P3, line_thickness, color);
  DrawLineEx(Door.P3, Door.P4, line_thickness, color);
  DrawLineEx(Door.P4, Door.P1, line_thickness, color);

  DrawCircleV(Door.P1, line_thickness/2, color);
  DrawCircleV(Door.P2, line_thickness/2, color);
  DrawCircleV(Door.P3, line_thickness/2, color);
  DrawCircleV(Door.P4, line_thickness/2, color);
}

void Draw_Doors_2D(){
  for(int i = 0; i < FP::Doors.size(); i++){
    Draw_Door_2D(FP::Doors[i]);
  }
}
