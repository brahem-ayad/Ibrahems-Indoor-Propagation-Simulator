#pragma once

#include <cmath>
#include<raylib.h>
#include<vector>
#include<algorithm>

static void Draw_RAY_LAUNCHING(Shader Points_Shader) {
  float Delta_Theta = PI/12;
  float Delta_Phi = Delta_Theta;
  float radius = 1;
  std::vector<Vector3> points;
  points.clear();
  for(float theta = 0; theta <= PI; theta += Delta_Theta){
    float N = std::ceil(2 * PI * std::sin(std::max(std::sin(theta), 0.0001f)) / Delta_Theta);
    Delta_Phi = 2*PI/N;
  for(float phi = 0; phi < 2*PI - Delta_Phi/2; phi += Delta_Phi){
    Vector3 P;
    P.x = radius * std::sin(theta) * std::cos(phi);
    P.y = radius * std::sin(theta) * std::sin(phi);
    P.z = radius * std::cos(theta);
    points.push_back(P);
    if(theta == 0){
      break;
    }
  }
  }

  BeginShaderMode(Points_Shader);
  for(int i = 0; i < points.size() - 1; i++){
    DrawLine3D(points[i], points[i+1], BLUE);
  }

  for(int i = 0; i < points.size(); i++){
    DrawSphereEx(points[i], 0.01, 5, 6, BLACK);
  }
  EndShaderMode();
}
