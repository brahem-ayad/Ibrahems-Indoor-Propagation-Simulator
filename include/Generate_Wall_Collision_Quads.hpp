#pragma once

#include<raylib.h>
#include<vector>
#include"./Config.hpp"

struct WallQuad3D {
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    Vector3 v4;
    WALL_MATERIAL Material;
    float Wall_Width;
};

std::vector<WallQuad3D> Generate_Wall_Collision_Quads();
