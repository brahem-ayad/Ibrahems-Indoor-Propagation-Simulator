#pragma once

#include<raylib.h>
#include <raymath.h>
#include<vector>

#include"../Floor_Planning/Floor_Plan.h"

static void Generate_Polygon_Visibility_Points(std::vector<Ray> rays) {
  for(int i = 0; i < rays.size(); i++){

    std::vector<Vector3> points;

    for(int j = 0; j < FP::walls.size(); j++){
      for(int k = 0; k < FP::walls[j].triangles.size(); k++){
        RayCollision col = GetRayCollisionTriangle(rays[i], FP::walls[j].triangles[k].P1, FP::walls[j].triangles[k].P2, FP::walls[j].triangles[k].P3);

        if(col.hit){
          points.push_back(col.point);
        }
      }
    }

    if(points.size() > 0){
      Vector3 col_point = Get_Closest_Point_to_Reference(points, rays[i].position);

      CONF::visibility_polygon.push_back(col_point);
    }
    else{
      Vector3 space_point = Vector3Add(CONF::BS_POS, Vector3Scale(rays[i].direction, 180));
      CONF::visibility_polygon.push_back(space_point);
    }

  }

}

static void Generate_Polygon_Invisibility_Points(std::vector<Ray> rays) {
  for(int i = 0; i < rays.size(); i++){

      Vector3 space_point = Vector3Add({CONF::BS_POS.x, CONF::BS_POS.y, CONF::BS_POS.z - 0.1f}, Vector3Scale(rays[i].direction, 180));
      CONF::invisibility_polygon.push_back(space_point);
  }

}

