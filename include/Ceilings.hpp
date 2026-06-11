#pragma once

#include<raylib.h>
#include<vector>
#include"./Utilities.hpp"
#include"./Config.hpp"

struct Ceiling {

Vector2 P1, P2, P3, P4;
    float Height;

    Vector3 P13, P23, P33, P43;

    std::vector<Triangle> triangles;

    Ceiling(Vector2 P1, Vector2 P2, Vector2 P3, Vector2 P4, float Height) 
        : P1(P1), P2(P2), P3(P3), P4(P4), Height(Height) 
    {
        float spacing = CONF::Grid_Spacing;
 
        // 1. Translate 2D workspace coordinates to 3D spaces safely
        P13 = { P1.x / spacing, -P1.y / spacing, Height };
        P23 = { P2.x / spacing, -P2.y / spacing, Height };
        P33 = { P3.x / spacing, -P3.y / spacing, Height };
        P43 = { P4.x / spacing, -P4.y / spacing, Height };
 
        // 2. Clear out any legacy configurations
        triangles.clear();
 
        // 3. Generate Triangles using shared CCW anchor loops
        Triangle T1 = { P33, P23, P13 }; // Triangle One (Left half)
        Triangle T2 = { P23, P33, P43 }; // Triangle Two (Right half) - FIXED WINDING
 
        triangles.push_back(T1);
        triangles.push_back(T2);
    }
};
