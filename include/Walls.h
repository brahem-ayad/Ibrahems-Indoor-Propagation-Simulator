#pragma once

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<algorithm>

#include"Config.h"
#include"Floor_Plan.h"

struct Transparent_Wall {
    int index;
    float distance;
};

class Wall_2D{
  private:
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Vector3 V4;

  public:
    Vector2 Start;
    Vector2 End;
    float Height = 3;

  void Draw_2D() {
    DrawLineEx(Start, End, 3, BLACK);
  }

  void Draw_3D(Camera3D camera3, std::vector<Transparent_Wall> &transparent_walls, int id) {
    float spacing = GRID::spacing;

    V1 = {Start.x/spacing, -Start.y/spacing, 0};
    V2 = {End.x/spacing, -End.y/spacing, 0};
    V3 = {End.x/spacing, -End.y/spacing, Height};
    V4 = {Start.x/spacing, -Start.y/spacing, Height};

    //To make the walls that are between the camera and the target slightly transparent
    Ray ray;
    ray.position = camera3.position;
    ray.direction = Vector3Normalize(Vector3Subtract(camera3.target, camera3.position));
    RayCollision col = GetRayCollisionQuad(ray, V1, V2, V3, V4);
    if(col.hit){
      transparent_walls.push_back({id, Vector3Distance(col.point, camera3.position)});
    }
    else{
      DrawTriangle3D(V1, V2, V3, WHITE);
      DrawTriangle3D(V3, V4, V1, WHITE);

      DrawLine3D(V1, V2, BLACK);
      DrawLine3D(V2, V3, BLACK);
      DrawLine3D(V3, V4, BLACK);
      DrawLine3D(V4, V1, BLACK);
    }
  }

};

namespace FP {
  static Vector2 starting_pos;
  static bool is_starting_pos_available = false;

  static Vector2 camera_2d_target;

  static std::vector<Wall_2D> walls_2d;
  static std::vector<Transparent_Wall> transparent_walls;
};

static void Draw_Walls_2D(){
  for(int i = 0; i < FP::walls_2d.size(); i++){
    FP::walls_2d[i].Draw_2D();
  }
}

static void Draw_Walls_3D(Camera3D camera3){
  rlDisableBackfaceCulling();

  FP::transparent_walls.clear();
  for(int i = 0; i < FP::walls_2d.size(); i++){
    FP::walls_2d[i].Draw_3D(camera3, FP::transparent_walls, i);
  }

  std::sort(FP::transparent_walls.begin(), FP::transparent_walls.end(), [](const auto& a, const auto& b) {
      return a.distance > b.distance;
  });

  for(int i = 0; i < FP::transparent_walls.size(); i++){
    float spacing = GRID::spacing;

    Wall_2D wall = FP::walls_2d[FP::transparent_walls[i].index];

    Vector3 V1 = {wall.Start.x/spacing, -wall.Start.y/spacing, 0};
    Vector3 V2 = {wall.End.x/spacing,   -wall.End.y/spacing, 0};
    Vector3 V3 = {wall.End.x/spacing,   -wall.End.y/spacing,   wall.Height};
    Vector3 V4 = {wall.Start.x/spacing, -wall.Start.y/spacing, wall.Height};

    Color color = Fade(WHITE, 0.5);

    DrawTriangle3D(V1, V2, V3, color);
    DrawTriangle3D(V3, V4, V1, color);
  }

}
