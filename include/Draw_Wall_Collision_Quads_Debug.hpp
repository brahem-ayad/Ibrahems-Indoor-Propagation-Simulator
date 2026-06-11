#pragma once

#include<raylib.h>
#include"../include/Generate_Wall_Collision_Quads.hpp"

void Draw_Wall_Collision_Quads_Debug(const std::vector<WallQuad3D>& quads, Color lineColor);
