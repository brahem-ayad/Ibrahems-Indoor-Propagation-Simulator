#include"../include/Draw_Wall_Collision_Quads_Debug.hpp"

#include<raylib.h>
#include"../include/Generate_Wall_Collision_Quads.hpp"

void Draw_Wall_Collision_Quads_Debug(const std::vector<WallQuad3D>& quads, Color lineColor) {
  for (const auto& quad : quads) {
    DrawLine3D(quad.v1, quad.v2, lineColor);
    DrawLine3D(quad.v2, quad.v3, lineColor);
    DrawLine3D(quad.v3, quad.v4, lineColor);
    DrawLine3D(quad.v4, quad.v1, lineColor);

    DrawLine3D(quad.v1, quad.v3, ColorAlpha(lineColor, 0.3f));
  }
}
