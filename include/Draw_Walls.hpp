#pragma once

#include<raylib.h>

struct Wall3DInterval {
    float start_t;
    float end_t;
    float bottom_z;
    float top_z;
};

void Draw_Wall_Segment_Combined(Vector2 start_pos, Vector2 end_pos, float width, Color color);

void Draw_Walls_2D();

void Draw_Walls_3D();
