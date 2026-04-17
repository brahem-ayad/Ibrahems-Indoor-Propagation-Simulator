#pragma once

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<algorithm>
#include<vector>
#include"../../Config.h"
#include"../../Utilities.h"

struct Transparent_Wall {
    int index;
    float distance;
};

// A wall should also have input to vectors of door and window objects.
// they have position and height and width
// and that way the wall can be split up at run time hopefully

class Wall{
  private:
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Vector3 V4;

  public:
    Vector2 Start;
    Vector2 End;
    float Height;

  void Draw_2D(Camera2D camera2) {

    Color Line_Color;
    if(CONF::Theme == Light_Theme) Line_Color = BLACK;
    else Line_Color = WHITE;

    if(CheckCollisionCircleLine(GetScreenToWorld2D(GetMousePosition(), camera2), 5, Start, End)){
      DrawLineEx(Start, End, 1, BLUE);
      DrawCircleV(Start, 2, Line_Color);
    }
    else {
      DrawLineEx(Start, End, 1, Line_Color);
      DrawCircleV(Start, 2, Line_Color);
    }
  }

  void Draw_3D(Camera3D camera3, std::vector<Transparent_Wall> &transparent_walls, int id, Shader shader) {
    float spacing = GRID::spacing;

    V1 = {Start.x/spacing, -Start.y/spacing, 0};
    V2 = {End.x/spacing, -End.y/spacing, 0};
    V3 = {End.x/spacing, -End.y/spacing, Height};
    V4 = {Start.x/spacing, -Start.y/spacing, Height};

    //To make the walls that are between the camera and the target slightly transparent
    if(!CONF::Show_All_Walls){
      Ray ray;
      ray.position = camera3.position;
      ray.direction = Vector3Normalize(Vector3Subtract(camera3.target, camera3.position));
      RayCollision col = GetRayCollisionQuad(ray, V1, V2, V3, V4);
      if(col.hit and camera3.position.z > 0){
        transparent_walls.push_back({id, Vector3Distance(col.point, camera3.position)});
      }
      else{
        DrawTriangle3D(V1, V2, V3, WHITE);
        DrawTriangle3D(V3, V4, V1, WHITE);

        Color Line_Color;
        if(CONF::Theme == Light_Theme) Line_Color = BLACK;
        else Line_Color = WHITE;
        DrawLine3D(V1, V2, Line_Color);
        DrawLine3D(V2, V3, Line_Color);
        DrawLine3D(V3, V4, Line_Color);
        DrawLine3D(V4, V1, Line_Color);
      }
    }
    else {
      BeginShaderMode(shader);
      if(CONF::Theme == Light_Theme){
        DrawQuad(V1, V2, V3, V4, WHITE);
      }
      else{
        DrawQuad(V1, V2, V3, V4, WHITE);
      }
      EndShaderMode();

      if(CONF::Theme == Light_Theme){
        DrawLine3D(V1, V2, GRAY);
        DrawLine3D(V2, V3, GRAY);
        DrawLine3D(V4, V1, GRAY);
        DrawLine3D(V3, V4, BLACK);
      }
      else {
        DrawLine3D(V1, V2, LIGHTGRAY);
        DrawLine3D(V2, V3, LIGHTGRAY);
        DrawLine3D(V4, V1, LIGHTGRAY);
        DrawLine3D(V3, V4, WHITE);
      }


    }
  }

};

