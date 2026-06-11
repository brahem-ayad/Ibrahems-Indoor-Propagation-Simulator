#pragma once

#include<raylib.h>
#include<vector>
#include"./Config.hpp"
#include"./Doors.hpp"
#include"./Windows.hpp"

struct WALL {
  Vector2 Wall_Start_2D;
  Vector2 Wall_End_2D;
  Vector2 Wall_Start_3D;
  Vector2 Wall_End_3D;

  float Wall_Height;
  float Wall_Width;
  WALL_MATERIAL Wall_Material;

  float Start_Left_Miter = 1.0f;
  float Start_Right_Miter = 1.0f;
  float End_Left_Miter = 1.0f;
  float End_Right_Miter = 1.0f;

  std::vector<DOOR> Doors_Vec;
  std::vector<WINDOW> Windows_Vec;
};
