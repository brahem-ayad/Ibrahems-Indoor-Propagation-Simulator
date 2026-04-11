#pragma once

#include<raylib.h>
#include"Grid.h"

static Vector2 Get_Snapped_Mouse_Position(Camera2D camera){
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  float spacing = GRID::spacing;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  return position;
}

static Rectangle Shift_Rec(Rectangle Rec, float x, float y){
  return {Rec.x + x, Rec.y + y, Rec.width, Rec.height};
}
