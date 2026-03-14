#pragma once

#include<raylib.h>

static void DrawGrid3D(int slices, float spacing, Color color){
  float size = slices*spacing/2;

  for(int i = -slices/2; i <= slices/2; i++){
    DrawLine3D({-size, i*spacing, 0}, {size, i*spacing, 0}, Fade(color, 0.5));
    DrawLine3D({i*spacing, -size, 0}, {i*spacing, size, 0}, Fade(color, 0.5));
  }
  DrawLine3D({-size, 0, 0}, {size, 0, 0}, color);
  DrawLine3D({0, -size, 0}, {0, size, 0}, color);
}
