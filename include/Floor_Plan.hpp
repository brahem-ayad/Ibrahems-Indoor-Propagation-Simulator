#pragma once

#include<vector>
#include"./Walls.hpp"
#include"./Doors.hpp"
#include"./Windows.hpp"
#include"./Floors.hpp"
#include"./Ceilings.hpp"
#include"./Transmitters.hpp"

namespace FP {
  inline std::vector<WALL> Walls_Vec;
  inline std::vector<DOOR> Doors;
  inline std::vector<WINDOW> Windows;
  inline std::vector<Floor> floors;
  inline std::vector<Ceiling> ceilings;

  inline std::vector<int> doors_on_walls_ids;
  inline std::vector<int> windows_on_walls_ids;

  inline std::vector<TRANSMITTER> Transmitters;
  inline std::vector<std::vector<Vector3>> Visibility_Polygons;

  inline std::vector<WallQuad3D> Wall_Quads_3D;
};
