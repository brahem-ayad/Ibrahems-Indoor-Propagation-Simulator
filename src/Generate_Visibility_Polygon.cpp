#include"../include/Generate_Visibility_Polygon.hpp"

#include<raylib.h>
#include<cmath>
#include"../include/Floor_Plan.hpp"

void Generate_Visibility_Polygon(){

  FP::Visibility_Polygons.resize(FP::Transmitters.size());

  for(int i = 0; i < FP::Transmitters.size(); i++){
    FP::Visibility_Polygons[i].clear();
    FP::Visibility_Polygons[i].reserve(360);
    for(float j = 0; j < 360; j++){
      Ray R;
      R.position = FP::Transmitters[i].Position;
      R.direction = {std::cos(float(j/360*2*PI)), std::sin(float(j/360*2*PI)), 0.0f};

      bool does_hit = false;
      Vector3 hit_pos = {-9999.0f, -9999.0f, -9999.0f};
      for(int k = 0; k < FP::Wall_Quads_3D.size(); k++){
        RayCollision col = GetRayCollisionQuad(R, FP::Wall_Quads_3D[k].v1, FP::Wall_Quads_3D[k].v2, FP::Wall_Quads_3D[k].v3, FP::Wall_Quads_3D[k].v4);
        if(col.hit){
          does_hit = true;
          if(hit_pos.x == -9999.0f) hit_pos = col.point;
          else{
            float d1 = Vector3Distance(FP::Transmitters[i].Position, hit_pos);
            float d2 = Vector3Distance(FP::Transmitters[i].Position, col.point);
            if(d2 < d1){
              hit_pos = col.point;
            }
          }
        }
      }

      Vector3 pos;
      if(does_hit){
        pos = hit_pos;
      }
      else{
        pos = Vector3Add(R.position, Vector3Scale(R.direction, 180.0f));
      }
      FP::Visibility_Polygons[i].push_back(pos);
    }
  }
}
