#pragma once

#include<raylib.h>
#include"./Config.hpp"

struct DOOR {
  Vector2 Position;
  float Width;
  float Height;
  DOOR_MATERIAL Material;

  Vector2 P1;
  Vector2 P2;
  Vector2 P3;
  Vector2 P4;

  int ID;
};
