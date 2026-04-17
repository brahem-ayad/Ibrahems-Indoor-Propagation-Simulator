#pragma once

#include<raylib.h>
#include"../Floor_Plan.h"
#include"Walls.h"

static void Draw_Walls_2D(Camera2D camera2){
  for(int i = 0; i < FP::walls.size(); i++){
    FP::walls[i].Draw_2D(camera2);
  }
}

static void Draw_Walls_3D(Camera3D camera3, Shader shader){
  rlDisableBackfaceCulling();

  if(!CONF::Show_All_Walls) FP::transparent_walls.clear();
  for(int i = 0; i < FP::walls.size(); i++){
    FP::walls[i].Draw_3D(camera3, FP::transparent_walls, i, shader);
  }

  if(!CONF::Show_All_Walls){
    std::sort(FP::transparent_walls.begin(), FP::transparent_walls.end(), [](const auto& a, const auto& b) {
        return a.distance > b.distance;
    });

    for(int i = 0; i < FP::transparent_walls.size(); i++){
      float spacing = GRID::spacing;

      Wall wall = FP::walls[FP::transparent_walls[i].index];

      Vector3 V1 = {wall.Start.x/spacing, -wall.Start.y/spacing, 0};
      Vector3 V2 = {wall.End.x/spacing,   -wall.End.y/spacing, 0};
      Vector3 V3 = {wall.End.x/spacing,   -wall.End.y/spacing,   wall.Height};
      Vector3 V4 = {wall.Start.x/spacing, -wall.Start.y/spacing, wall.Height};

      Color color = Fade(WHITE, 0.2);

      DrawTriangle3D(V1, V2, V3, color);
      DrawTriangle3D(V3, V4, V1, color);
    }
  }
}
