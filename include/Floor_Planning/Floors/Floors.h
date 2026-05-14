#pragma once

#include"raylib.h"
#include"../../Grid.h"
#include"../../Utilities.h"

class Floor {
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;

    Vector3 P13;
    Vector3 P23;
    Vector3 P33;
    Vector3 P43;

    std::vector<Triangle> triangles;

    Floor(Vector2 P1, Vector2 P2, Vector2 P3, Vector2 P4) : P1(P1), P2(P2), P3(P3), P4(P4) {
      P13 = { P1.x / GRID::spacing, -P1.y / GRID::spacing, 0 };
      P23 = { P2.x / GRID::spacing, -P2.y / GRID::spacing, 0 };
      P33 = { P3.x / GRID::spacing, -P3.y / GRID::spacing, 0 };
      P43 = { P4.x / GRID::spacing, -P4.y / GRID::spacing, 0 };

      Triangle T1 = {P13, P23, P33};
      Triangle T2 = {P23, P33, P43};
      triangles.push_back(T1);
      triangles.push_back(T2);
    }

    void Draw_3D() {
      for(int i = 0; i < triangles.size(); i++){
        DrawTriangle3D(triangles[i].P1, triangles[i].P2, triangles[i].P3, LIGHTGRAY);
      }
      //DrawLine3D(P13, P23, BLACK);
      //DrawLine3D(P23, P43, BLACK);
      //DrawLine3D(P43, P33, BLACK);
      //DrawLine3D(P33, P13, BLACK);
    }
};
