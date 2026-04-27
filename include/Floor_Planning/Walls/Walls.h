#pragma once

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<vector>
#include"../../Config.h"
#include"../../Utilities.h"
#include"../Doors/Doors.h"
#include"../Windows/Windows.h"
#include<algorithm>

class Object {
  public:
    Vector2 pos;
    float width;
};

static void SortObjectsOnLine(std::vector<Object>& objects, Vector2 start) {
    std::sort(objects.begin(), objects.end(), [&](const Object& a, const Object& b) {
        // Helper lambda to get squared distance from start
        auto getDistSq = [&](Vector2 pos) {
            float dx = pos.x - start.x;
            float dy = pos.y - start.y;
            return (dx * dx) + (dy * dy);
        };

        return getDistSq(a.pos) < getDistSq(b.pos);
    });
}

class Line {
  public:
    Vector3 start;
    Vector3 end;
};

class Wall{
  private:
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Vector3 V4;

  public:
    Vector2 Start;
    Vector2 End;
    float Height;
    std::vector<Door> doors;
    std::vector<Window> windows;
    std::vector<Vector3> points;
    std::vector<Triangle> triangles;
    std::vector<Object> objects;
    std::vector<Line> lines;

    Wall(Vector2 Start, Vector2 End, float Height) : Start(Start), End(End), Height(Height) {}

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

    void Update(){
      float spacing = GRID::spacing;

      if(points.size() > 0) points.clear();
      if(triangles.size() > 0) triangles.clear();
      if(objects.size() > 0) objects.clear();
      if(lines.size() > 0) lines.clear();

      V1 = {Start.x/spacing, -Start.y/spacing, 0};
      V2 = {Start.x/spacing, -Start.y/spacing, Height};
      points.push_back(V1);
      points.push_back(V2);

      for(int i = 0; i < doors.size(); i++){

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

        Vector3 P1 = {V_s.x / spacing, -V_s.y / spacing, doors[i].height};
        Vector3 P2 = {V_s.x / spacing, -V_s.y / spacing, Height};
        Vector3 P3 = {V_e.x / spacing, -V_e.y / spacing, doors[i].height};
        Vector3 P4 = {V_e.x / spacing, -V_e.y / spacing, Height};
        triangles.push_back({P1, P2, P3});
        triangles.push_back({P4, P2, P3});

        Object O;
        O.pos = doors[i].Position;
        O.width = doors[i].width;
        objects.push_back(O);

        Line L;
        L.start = V_1;
        L.end = V_3;
        lines.push_back(L);
        L.start = V_3;
        L.end = V_4;
        lines.push_back(L);
        L.start = V_4;
        L.end = V_6;
        lines.push_back(L);
      }

      for(int i = 0; i < windows.size(); i++){

        Vector2 dir = Vector2Normalize(Vector2Subtract(End, Start));
        Vector2 V_s = Vector2Add(windows[i].Position, Vector2Scale(dir, -windows[i].width/2 * spacing));
        Vector2 V_e = Vector2Add(windows[i].Position, Vector2Scale(dir, windows[i].width/2 * spacing));

        Vector3 V_1 = {V_s.x / spacing, -V_s.y / spacing, 0}; // left side bottom
        Vector3 V_2 = {V_s.x / spacing, -V_s.y / spacing, Height}; // left side top
        Vector3 V_3 = {V_s.x / spacing, -V_s.y / spacing, windows[i].base_height}; // left side window base height
        Vector3 V_4 = {V_s.x / spacing, -V_s.y / spacing, windows[i].base_height + windows[i].window_height}; // left side window height

        Vector3 V_5 = {V_e.x / spacing, -V_e.y / spacing, windows[i].base_height + windows[i].window_height}; // right side window height
        Vector3 V_6 = {V_e.x / spacing, -V_e.y / spacing, windows[i].base_height}; // right side window base height
        Vector3 V_7 = {V_e.x / spacing, -V_e.y / spacing, Height}; // right side top
        Vector3 V_8 = {V_e.x / spacing, -V_e.y / spacing, 0}; // right side bottom

        points.push_back(V_1);
        points.push_back(V_2);
        points.push_back(V_3);
        points.push_back(V_4);
        points.push_back(V_5);
        points.push_back(V_6);
        points.push_back(V_7);
        points.push_back(V_8);

        Vector3 P1 = {V_s.x / spacing, -V_s.y / spacing, 0};
        Vector3 P2 = {V_s.x / spacing, -V_s.y / spacing, windows[i].base_height};
        Vector3 P3 = {V_e.x / spacing, -V_e.y / spacing, 0};
        Vector3 P4 = {V_e.x / spacing, -V_e.y / spacing, windows[i].base_height};
        triangles.push_back({P1, P2, P3});
        triangles.push_back({P4, P2, P3});
        Vector3 P5 = {V_s.x / spacing, -V_s.y / spacing, windows[i].base_height + windows[i].window_height};
        Vector3 P6 = {V_s.x / spacing, -V_s.y / spacing, Height};
        Vector3 P7 = {V_e.x / spacing, -V_e.y / spacing, windows[i].base_height + windows[i].window_height};
        Vector3 P8 = {V_e.x / spacing, -V_e.y / spacing, Height};
        triangles.push_back({P5, P6, P7});
        triangles.push_back({P8, P6, P7});

        Object O;
        O.pos = windows[i].Position;
        O.width = windows[i].width;
        objects.push_back(O);

        Line L;
        L.start = V_3;
        L.end = V_4;
        lines.push_back(L);
        L.start = V_4;
        L.end = V_5;
        lines.push_back(L);
        L.start = V_5;
        L.end = V_6;
        lines.push_back(L);
        L.start = V_6;
        L.end = V_3;
        lines.push_back(L);
      }

      V3 = {End.x/spacing, -End.y/spacing, 0};
      V4 = {End.x/spacing, -End.y/spacing, Height};
      points.push_back(V3);
      points.push_back(V4);

      if(objects.size() > 0){
        SortObjectsOnLine(objects, Start);

        Vector2 dir = Vector2Normalize(Vector2Subtract(End, Start));
        Vector2 V_s = Start;
        Vector2 V_e = Vector2Add(objects[0].pos, Vector2Scale(dir, -objects[0].width/2 * spacing));
        Vector3 P1 = {V_s.x / spacing, -V_s.y / spacing, 0};
        Vector3 P2 = {V_s.x / spacing, -V_s.y / spacing, Height};
        Vector3 P3 = {V_e.x / spacing, -V_e.y / spacing, 0};
        Vector3 P4 = {V_e.x / spacing, -V_e.y / spacing, Height};
        triangles.push_back({P1, P2, P3});
        triangles.push_back({P4, P2, P3});

        dir = Vector2Normalize(Vector2Subtract(End, Start));
        V_s = Vector2Add(objects[objects.size()-1].pos, Vector2Scale(dir, objects[objects.size()-1].width/2 * spacing));
        V_e = End;
        P1 = {V_s.x / spacing, -V_s.y / spacing, 0};
        P2 = {V_s.x / spacing, -V_s.y / spacing, Height};
        P3 = {V_e.x / spacing, -V_e.y / spacing, 0};
        P4 = {V_e.x / spacing, -V_e.y / spacing, Height};
        triangles.push_back({P1, P2, P3});
        triangles.push_back({P4, P2, P3});

        for(int i = 0; i < objects.size()-1; i++){
          Vector2 dir = Vector2Normalize(Vector2Subtract(End, Start));
          Vector2 V_s = Vector2Add(objects[i].pos, Vector2Scale(dir, objects[i].width/2 * spacing));
          Vector2 V_e = Vector2Add(objects[i+1].pos, Vector2Scale(dir, -objects[i+1].width/2 * spacing));
          Vector3 P1 = {V_s.x / spacing, -V_s.y / spacing, 0};
          Vector3 P2 = {V_s.x / spacing, -V_s.y / spacing, Height};
          Vector3 P3 = {V_e.x / spacing, -V_e.y / spacing, 0};
          Vector3 P4 = {V_e.x / spacing, -V_e.y / spacing, Height};
          triangles.push_back({P1, P2, P3});
          triangles.push_back({P4, P2, P3});
        }
      }
      else{
        triangles.push_back({V1, V2, V3});
        triangles.push_back({V4, V2, V3});
      }
    }

    void Draw_3D(Camera3D camera3, Shader shader) {

      //for(int i = 0; i < points.size(); i++){
      //  DrawSphere(points[i], 0.1, RED);
      //}

      BeginShaderMode(shader);

      for(int i = 0; i < triangles.size(); i++){
        DrawTriangle3D(triangles[i].P1, triangles[i].P2, triangles[i].P3, WHITE);
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

      for(int i = 0; i < lines.size(); i++){
        DrawLine3D(lines[i].start, lines[i].end, BLACK);
      }

    }

};

