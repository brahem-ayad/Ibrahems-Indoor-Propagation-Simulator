#pragma once

#include<raylib.h>
#include"./Config.hpp"

struct WINDOW {
  Vector2 Position;
  float Width;
  float base_height;
  float window_height;

  int ID;

  Vector2 P1;
  Vector2 P2;
  Vector2 P3;
  Vector2 P4;

  WINDOW_MATERIAL Material;
};
