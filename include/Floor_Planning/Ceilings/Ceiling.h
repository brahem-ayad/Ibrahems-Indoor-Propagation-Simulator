#pragma once

#include"raylib.h"
#include"../../Grid.h"
#include"../../Utilities.h"
#include <vector>

class Ceiling {
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;
    float Height;

    Vector3 P13;
    Vector3 P23;
    Vector3 P33;
    Vector3 P43;

    std::vector<Triangle> triangles;

    Ceiling(Vector2 P1, Vector2 P2, Vector2 P3, Vector2 P4, float Height) : P1(P1), P2(P2), P3(P3), P4(P4), Height(Height) {
      P13 = { P1.x / GRID::spacing, -P1.y / GRID::spacing, Height };
      P23 = { P2.x / GRID::spacing, -P2.y / GRID::spacing, Height };
      P33 = { P3.x / GRID::spacing, -P3.y / GRID::spacing, Height };
      P43 = { P4.x / GRID::spacing, -P4.y / GRID::spacing, Height };

      Triangle T1 = {P13, P23, P33};
      Triangle T2 = {P23, P33, P43};
      triangles.push_back(T1);
      triangles.push_back(T2);
    }

    void Draw_3D() {
      for(int i = 0; i < triangles.size(); i++){
        Color c = CONF::Theme == Light_Theme ? (Color){230, 230, 230, 255} : RAYWHITE;
        DrawTriangle3D(triangles[i].P1, triangles[i].P2, triangles[i].P3, c);
      }
      Color c = CONF::Theme == Light_Theme ? BLACK : WHITE;
      DrawLine3D(P13, P23, c);
      DrawLine3D(P23, P43, c);
      DrawLine3D(P43, P33, c);
      DrawLine3D(P33, P13, c);
    }
};
