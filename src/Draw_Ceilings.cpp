#include"../include/Draw_Ceilings.hpp"

#include<raylib.h>
#include<rlgl.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Utilities.hpp"
#include"../include/Ceilings.hpp"
#include"../include/Config.hpp"


void Draw_Ceilings_2D() {
  BeginShaderMode(CONF::Ceiling_Shader_2D);
  for(int i = 0; i < FP::ceilings.size(); i++){
    DrawRectanglePoints(FP::ceilings[i].P1, FP::ceilings[i].P4, RED, {255, 255, 255, 255}, 0.1, false);
  }
  EndShaderMode();
}

void Draw_Ceiling_3D(Ceiling C) {
  for(int i = 0; i < C.triangles.size(); i++){
    rlDisableBackfaceCulling();
    DrawTriangle3D(C.triangles[i].P1, C.triangles[i].P2, C.triangles[i].P3, {230, 230, 230, 255});
  }
}

void Draw_Ceilings_3D() {
  for(int i = 0; i < FP::ceilings.size(); i++){
    Draw_Ceiling_3D(FP::ceilings[i]);
  }
}
