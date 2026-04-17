#pragma once

#include<raylib.h>
#include<vector>
#include"Walls/Walls.h"
#include"Floors/Floors.h"
#include"Ceilings/Ceiling.h"

namespace FP {
  static Vector2 starting_pos;
  static bool is_starting_pos_available = false;

  static Vector2 camera_2d_target;

  static std::vector<Wall> walls;
  static std::vector<Transparent_Wall> transparent_walls;

  static std::vector<Floor> floors;
  static std::vector<Ceiling> ceilings;
};
