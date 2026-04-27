#pragma once

#include<raylib.h>
#include<raymath.h>

#include"../Config.h"
#include"../Floor_Planning/Floor_Plan.h"
#include"../Utilities.h"

static void Generate_Corner_Ray(Vector3 point) {
  Ray R;

  R.position = CONF::BS_POS;
  R.direction = Vector3Normalize(Vector3Subtract(point, R.position));

  bool does_hit_wall = false;

  std::vector<Vector3> points;

  for(int j = 0; j < FP::walls.size(); j++){
    for(int k = 0; k < FP::walls[j].triangles.size(); k++){
      Triangle Boro = Get_Slightly_Bigger_Triangle(FP::walls[j].triangles[k].P1, FP::walls[j].triangles[k].P2, FP::walls[j].triangles[k].P3);

      RayCollision col = GetRayCollisionTriangle(R, Boro.P1, Boro.P2, Boro.P3);

      if(col.hit){
        points.push_back(col.point);
      }
    }
  }

  if(points.size() > 0){
    Vector3 col_point = Get_Closest_Point_to_Reference(points, R.position);
    //DrawSphere(col_point, 0.1, BLUE);

    if( Vector3Distance(col_point, {point.x, point.y, R.position.z}) > 0.1f ){
      does_hit_wall = true;
    }
  }

  if(does_hit_wall == false) {
    CONF::rays.push_back(R);
    CONF::visibility_polygon.push_back({point.x, point.y, R.position.z});

    //Create_Extra_Rays(R, 0.01);
  }
}
