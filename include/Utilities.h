#pragma once

#include<raylib.h>
#include <raymath.h>
#include"Grid.h"

static Vector2 Get_Snapped_Mouse_Position(Camera2D camera){
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  float spacing;
  if(camera.zoom < 0.8f) spacing = GRID::spacing;
  else if(camera.zoom < 1.5f) spacing = GRID::spacing/2;
  else spacing = GRID::spacing/10;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  return position;
}

static Rectangle Shift_Rec(Rectangle Rec, float x, float y){
  return {Rec.x + x, Rec.y + y, Rec.width, Rec.height};
}

static Color Change_Brightness(Color color, int amount){
  return {(unsigned char)(color.r + amount), (unsigned char)(color.g + amount), (unsigned char)(color.b + amount), color.a};
}

static void DrawRectanglePoints(Vector2 P1, Vector2 P2, Color line_color, Color rectangle_color, float rectangle_opacity, bool Draw_End_Points){

  if (P1.x == P2.x or P1.y == P2.y) { DrawCircleV(P1, 5, line_color); DrawLineV(P1, P2, line_color); return ; }

  Vector2 UL; // Upper Left
  Vector2 LR; // Lower Right
  Vector2 UR; // Upper Right
  Vector2 LL; // Lower Left

  if(P1.x < P2.x){
    if(P1.y < P2.y){
      UL = P1;
      LR = P2;
      UR = {P2.x, P1.y};
      LL = {P1.x, P2.y};
    }
    else if(P1.y > P2.y){
      UL = {P1.x, P2.y};
      LR = {P2.x, P1.y};
      UR = P2;
      LL = P1;
    }
  }
  else if(P1.x > P2.x){
    if(P1.y < P2.y){
      UL = {P2.x, P1.y};
      LR = {P1.x, P2.y};
      UR = P1;
      LL = P2;
    }
    else if(P1.y > P2.y){
      UL = P2;
      LR = P1;
      UR = {P1.x, P2.y};
      LL = {P2.x, P1.y};
    }
  }

  DrawRectangleV(UL, Vector2Subtract(LR, UL), Fade(rectangle_color, rectangle_opacity));

  if(Draw_End_Points == true){
    DrawCircleV(P1, 5, line_color);
    DrawCircleV(P2, 5, line_color);
  }

  DrawLineV(UL, UR, line_color);
  DrawLineV(UR, LR, line_color);
  DrawLineV(LR, LL, line_color);
  DrawLineV(LL, UL, line_color);

}

class RectanglePoints {
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;
};

static RectanglePoints GetRectanglePoints(Vector2 P1, Vector2 P2){

  Vector2 UL; // Upper Left
  Vector2 LR; // Lower Right
  Vector2 UR; // Upper Right
  Vector2 LL; // Lower Left

  if(P1.x < P2.x){
    if(P1.y < P2.y){
      UL = P1;
      LR = P2;
      UR = {P2.x, P1.y};
      LL = {P1.x, P2.y};
    }
    else if(P1.y > P2.y){
      UL = {P1.x, P2.y};
      LR = {P2.x, P1.y};
      UR = P2;
      LL = P1;
    }
  }
  else if(P1.x > P2.x){
    if(P1.y < P2.y){
      UL = {P2.x, P1.y};
      LR = {P1.x, P2.y};
      UR = P1;
      LL = P2;
    }
    else if(P1.y > P2.y){
      UL = P2;
      LR = P1;
      UR = {P1.x, P2.y};
      LL = {P2.x, P1.y};
    }
  }

  return {UL, UR, LL, LR};
}

static void DrawQuad(Vector3 UL, Vector3 UR, Vector3 LL, Vector3 LR, Color color){
  DrawTriangle3D(UL, UR, LL, color);
  DrawTriangle3D(LL, LR, UL, color);
}
