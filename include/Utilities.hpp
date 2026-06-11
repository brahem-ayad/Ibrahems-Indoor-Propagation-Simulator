#pragma once

#include<raylib.h>
#include<raymath.h>
#include<vector>
#include"./Generate_Wall_Collision_Quads.hpp"

Vector2 Get_Snapped_World_Mouse_Position();

void Draw_Rectangle_Line_Thickness(Vector2 Line_Starting_Pos, Vector2 Line_End_Pos, float Rectangle_Width, Color color);

void Draw_Oriented_Rectangle_on_Line(Vector2 LS, Vector2 LE, Vector2 P, float RW, float RT, bool &Is_Placable);

class RectanglePoints {
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;
};

RectanglePoints GetRectanglePoints(Vector2 P1, Vector2 P2);

class Triangle {
  public:
    Vector3 P1;
    Vector3 P2;
    Vector3 P3;
};

void DrawRectanglePoints(Vector2 P1, Vector2 P2, Color line_color, Color rectangle_color, float rectangle_opacity, bool Draw_End_Points);

Vector2 Get_Closest_Point_on_Segment(Vector2 p, Vector2 a, Vector2 b);

void Draw_Rectangle_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width, Color color);

RectanglePoints Get_Rectangle_Points_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width);

Vector3 Get_Closest_Point_to_Reference(std::vector<Vector3> points, Vector3 reference);

RayCollision Get_Ray_Collision_Quad(Ray R, WallQuad3D Q);
