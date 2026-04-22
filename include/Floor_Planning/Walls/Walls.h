#pragma once

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<vector>
#include"../../Config.h"
#include"../../Utilities.h"
#include"../Doors/Doors.h"
#include"../Windows/Windows.h"

class Wall{
  public:
    Vector2 Start;
    Vector2 End;
    float Height;
    std::vector<Door> doors;
    std::vector<Window> windows;
    std::vector<Vector3> points;

  void Draw_2D(Camera2D camera2) {

    Color Line_Color;
    if(CONF::Theme == Light_Theme) Line_Color = BLACK;
    else Line_Color = WHITE;

    if(CheckCollisionCircleLine(GetScreenToWorld2D(GetMousePosition(), camera2), 5, Start, End)){
      DrawLineEx(Start, End, 1.5f, BLUE);
      DrawCircleV(Start, 2, Line_Color);
    }
    else {
      DrawLineEx(Start, End, 1.5f, Line_Color);
      DrawCircleV(Start, 2, Line_Color);
    }

  }

  void Draw_3D(Camera3D camera3, Shader shader) {
    float spacing = GRID::spacing;

    // This shouldn't be run everyframe. but only when the 3D view button is pressed
    // the points are cleared and new ones are generated
    // then the points are used for drawing the walls
    // and also for simulation
    points.clear();

    Vector3 V1 = {Start.x/spacing, -Start.y/spacing, 0};
    Vector3 V2 = {Start.x/spacing, -Start.y/spacing, Height};
    points.push_back(V1);
    points.push_back(V2);

    // Use a sorting algorithm to sort the doors

    // Note : these points should not be added base on the order at which doors are in the array
    // but in terms of their positions
    // or they should be reordered later. Or I think it will be easier to reorder the doors based on their positions and the
    // end points of a wall. same for windows
    for(int i = 0; i < doors.size(); i++){ // number of base points should be equal to 1 + number of doors * 4

      Vector2 dir = Vector2Normalize(Vector2Subtract(End, Start));
      Vector2 V_s = Vector2Add(doors[i].Position, Vector2Scale(dir, -doors[i].width/2 * spacing));
      Vector2 V_e = Vector2Add(doors[i].Position, Vector2Scale(dir, doors[i].width/2 * spacing));

      Vector3 V_1 = {V_s.x / spacing, -V_s.y / spacing, 0}; // left side bottom
      Vector3 V_2 = {V_s.x / spacing, -V_s.y / spacing, Height}; // left side top
      Vector3 V_3 = {V_s.x / spacing, -V_s.y / spacing, doors[i].height}; // left side door height

      Vector3 V_4 = {V_e.x / spacing, -V_e.y / spacing, doors[i].height}; // right side door height
      Vector3 V_5 = {V_e.x / spacing, -V_e.y / spacing, Height}; // right side top
      Vector3 V_6 = {V_e.x / spacing, -V_e.y / spacing, 0}; // right side bottom

      points.push_back(V_1);
      points.push_back(V_2);
      points.push_back(V_3);
      points.push_back(V_4);
      points.push_back(V_5);
      points.push_back(V_6);
    }


    Vector3 V3 = {End.x/spacing, -End.y/spacing, 0};
    Vector3 V4 = {End.x/spacing, -End.y/spacing, Height};
    points.push_back(V3);
    points.push_back(V4);

    for(int i = 0; i < points.size(); i++){
      DrawSphere(points[i], 0.1, RED);
    }

    BeginShaderMode(shader);

    if(CONF::Theme == Light_Theme){
      for(int i = 0; i < points.size()-1; i += 3){
        DrawQuad(points[i], points[i+2], points[i+3], points[i+1], WHITE);
      }

      //DrawQuad(V1, V3, V4, V2, WHITE);
    }
    else{
      for(int i = 0; i < points.size()-1; i += 3){
        DrawQuad(points[i], points[i+2], points[i+3], points[i+1], RAYWHITE);
      }

      //DrawQuad(V1, V3, V4, V2, RAYWHITE);
    }

    EndShaderMode();

    if(CONF::Theme == Light_Theme){
      //DrawLine3D(V1, V3, GRAY);
      DrawLine3D(V3, V4, GRAY);
      DrawLine3D(V2, V1, GRAY);
      DrawLine3D(V4, V2, BLACK);
    }
    else {
      //DrawLine3D(V1, V3, LIGHTGRAY);
      DrawLine3D(V3, V4, LIGHTGRAY);
      DrawLine3D(V2, V1, LIGHTGRAY);
      DrawLine3D(V4, V2, WHITE);
    }

    // Real Smart guy wrote that by the way :)
    for(int i = 2; i < points.size()-4;){
      DrawLine3D(points[i], points[i+2], BLACK);
      i += 2;
      DrawLine3D(points[i], points[i+1], BLACK);
      i += 1;
      DrawLine3D(points[i], points[i+2], BLACK);
      i += 3;
    }

  }

};

