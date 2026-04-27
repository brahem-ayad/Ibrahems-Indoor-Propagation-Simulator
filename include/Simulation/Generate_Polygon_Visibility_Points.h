#pragma once

#include<raylib.h>
#include<vector>

#include"../Floor_Planning/Floor_Plan.h"

static void Generate_Polygon_Visibility_Points(std::vector<Ray> rays) {
  for(int i = 0; i < rays.size(); i++){
    bool does_hit_wall = true;

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

  }

}
