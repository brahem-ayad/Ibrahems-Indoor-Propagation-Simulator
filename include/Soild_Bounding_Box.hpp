#pragma once

#include<raylib.h>

static void DrawSolidBoundingBox(BoundingBox B, Color color){
  Vector3 position = {B.min.x + (B.max.x - B.min.x)/2,
                      B.min.y + (B.max.y - B.min.y)/2,
                      B.min.z + (B.max.z - B.min.z)/2};

  Vector3 size = {B.max.x - B.min.x,
                  B.max.y - B.min.y,
                  B.max.z - B.min.z,};

  DrawCubeV(position, size, color);
}
