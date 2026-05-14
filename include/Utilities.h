#pragma once

#include <cstdlib>
#include<raylib.h>
#include <raymath.h>
#include"Grid.h"
#include<algorithm>

static Vector2 Get_Snapped_Mouse_Position(Camera2D camera){
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  float spacing;
  if(camera.zoom < 0.9f) spacing = GRID::spacing;
  else if(camera.zoom < 1.5f) spacing = GRID::spacing/2;
  else spacing = GRID::spacing/10;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  return position;
}

static Vector2 Get_Snapped_Mouse_Position_Axial(Camera2D camera, Vector2 s_pos){
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  float spacing;
  if(camera.zoom < 0.8f) spacing = GRID::spacing;
  else if(camera.zoom < 1.5f) spacing = GRID::spacing/2;
  else spacing = GRID::spacing/10;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  Vector2 v = Vector2Normalize(Vector2Subtract(position, s_pos));
  float dot_product_x = Vector2DotProduct(v, {1, 0});

  if(std::abs(dot_product_x) > 0.7071) position.y = s_pos.y;
  else position.x = s_pos.x;

  return position;
}

static Rectangle Shift_Rec(Rectangle Rec, float x, float y){
  return {Rec.x + x, Rec.y + y, Rec.width, Rec.height};
}

static Color Change_Brightness(Color color, int amount){
  return {(unsigned char)(color.r + amount), (unsigned char)(color.g + amount), (unsigned char)(color.b + amount), color.a};
}

static void DrawRectanglePoints(Vector2 P1, Vector2 P2, Color line_color, Color rectangle_color, float rectangle_opacity, bool Draw_End_Points){

  if (P1.x == P2.x or P1.y == P2.y) { DrawCircleV(P1, 5, line_color); DrawLineV(P1, P2, line_color); return ; }

  Vector2 UL; // Upper Left
  Vector2 LR; // Lower Right
  Vector2 UR; // Upper Right
  Vector2 LL; // Lower Left

  if(P1.x < P2.x){
    if(P1.y < P2.y){
      UL = P1;
      LR = P2;
      UR = {P2.x, P1.y};
      LL = {P1.x, P2.y};
    }
    else if(P1.y > P2.y){
      UL = {P1.x, P2.y};
      LR = {P2.x, P1.y};
      UR = P2;
      LL = P1;
    }
  }
  else if(P1.x > P2.x){
    if(P1.y < P2.y){
      UL = {P2.x, P1.y};
      LR = {P1.x, P2.y};
      UR = P1;
      LL = P2;
    }
    else if(P1.y > P2.y){
      UL = P2;
      LR = P1;
      UR = {P1.x, P2.y};
      LL = {P2.x, P1.y};
    }
  }

  DrawRectangleV(UL, Vector2Subtract(LR, UL), Fade(rectangle_color, rectangle_opacity));

  if(Draw_End_Points == true){
    DrawCircleV(P1, 5, line_color);
    DrawCircleV(P2, 5, line_color);
  }

  DrawLineV(UL, UR, line_color);
  DrawLineV(UR, LR, line_color);
  DrawLineV(LR, LL, line_color);
  DrawLineV(LL, UL, line_color);

}

class RectanglePoints {
  public:
    Vector2 P1;
    Vector2 P2;
    Vector2 P3;
    Vector2 P4;
};

static RectanglePoints GetRectanglePoints(Vector2 P1, Vector2 P2){

  if(Vector2Equals(P1, P2)){
    return {P1, P1, P1, P1};
  }

  Vector2 UL; // Upper Left
  Vector2 LR; // Lower Right
  Vector2 UR; // Upper Right
  Vector2 LL; // Lower Left

  if(P1.x <= P2.x){
    if(P1.y <= P2.y){
      UL = P1;
      LR = P2;
      UR = {P2.x, P1.y};
      LL = {P1.x, P2.y};
    }
    else if(P1.y > P2.y){
      UL = {P1.x, P2.y};
      LR = {P2.x, P1.y};
      UR = P2;
      LL = P1;
    }
  }
  else if(P1.x >= P2.x){
    if(P1.y <= P2.y){
      UL = {P2.x, P1.y};
      LR = {P1.x, P2.y};
      UR = P1;
      LL = P2;
    }
    else if(P1.y > P2.y){
      UL = P2;
      LR = P1;
      UR = {P1.x, P2.y};
      LL = {P2.x, P1.y};
    }
  }

  return {UL, UR, LL, LR};
}

static bool IsSameXY(Vector3 a, Vector3 b) {
    float epsilon = 0.001f;
    return (fabsf(a.x - b.x) < epsilon) && (fabsf(a.y - b.y) < epsilon);
}

static void DrawQuad(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Color color) {
    Vector3 pts[4] = {p1, p2, p3, p4};

    // Check if it's a Floor/Ceiling (all Z are effectively same)
    bool isPlanar = true;
    for (int i = 1; i < 4; i++) {
        if (fabsf(pts[i].z - pts[0].z) > 0.001f) {
            isPlanar = false;
            break;
        }
    }

    if (isPlanar) {
        // 1. Find centroid for angle-based sorting
        Vector2 center = {0, 0};
        for (const auto& p : pts) { center.x += p.x; center.y += p.y; }
        center.x /= 4.0f; center.y /= 4.0f;

        // 2. Sort by angle around center (Bubble sort for 4 items is very fast)
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 4; j++) {
                float a1 = atan2f(pts[i].y - center.y, pts[i].x - center.x);
                float a2 = atan2f(pts[j].y - center.y, pts[j].x - center.x);
                if (a1 > a2) std::swap(pts[i], pts[j]);
            }
        }
        // 3. Draw sorted planar quad
        DrawTriangle3D(pts[0], pts[1], pts[2], color);
        DrawTriangle3D(pts[0], pts[2], pts[3], color);
    } 
    else {
        // Logic for Vertical Walls (your existing code)
        struct Segment { Vector3 bot; Vector3 top; };
        Segment segs[2];
        int segIndex = 0;
        bool used[4] = {false, false, false, false};

        for (int i = 0; i < 4; i++) {
            if (used[i]) continue;
            for (int j = i + 1; j < 4; j++) {
                if (!used[j] && IsSameXY(pts[i], pts[j])) {
                    segs[segIndex] = (pts[i].z < pts[j].z) ? Segment{pts[i], pts[j]} : Segment{pts[j], pts[i]};
                    used[i] = true; used[j] = true;
                    segIndex++;
                    break;
                }
            }
        }
        DrawTriangle3D(segs[0].bot, segs[1].bot, segs[1].top, color);
        DrawTriangle3D(segs[0].bot, segs[1].top, segs[0].top, color);
    }
}

static Vector2 Get_Closest_Point_on_Segment(Vector2 p, Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    // Denominator is the squared length of AB
    float lengthSquared = dx * dx + dy * dy;

    // Handle the case where A and B are the same point
    if (lengthSquared == 0.0f) return a;

    // Project point P onto the line, normalized by the length squared
    float t = ((p.x - a.x) * dx + (p.y - a.y) * dy) / lengthSquared;

    // Clamp t to segment bounds [0, 1]
    t = std::clamp(t, 0.0f, 1.0f);

    return { a.x + t * dx, a.y + t * dy };
}

static void Draw_Rectangle_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width, Color color, Camera2D camera2){
  Vector2 dir = normalized_line_parallel_vector;
  Vector2 perp = {-dir.y, dir.x};

  Vector2 P1 = GetWorldToScreen2D(Vector2Add(Vector2Add(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width)), camera2);
  Vector2 P2 = GetWorldToScreen2D(Vector2Add(Vector2Subtract(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width)), camera2);
  Vector2 P3 = GetWorldToScreen2D(Vector2Subtract(Vector2Subtract(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width)), camera2);
  Vector2 P4 = GetWorldToScreen2D(Vector2Subtract(Vector2Add(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width)), camera2);

  DrawLineEx(P1, P2, 1, color);
  DrawLineEx(P2, P3, 1, color);
  DrawLineEx(P3, P4, 1, color);
  DrawLineEx(P4, P1, 1, color);
}

static RectanglePoints Get_Rectangle_Points_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width, Camera2D camera2){
  Vector2 dir = normalized_line_parallel_vector;
  Vector2 perp = {-dir.y, dir.x};

  Vector2 P1 = Vector2Add(Vector2Add(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width))          ;
  Vector2 P2 = Vector2Add(Vector2Subtract(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width))     ;
  Vector2 P3 = Vector2Subtract(Vector2Subtract(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width));
  Vector2 P4 = Vector2Subtract(Vector2Add(center, Vector2Scale(dir, length/2 * GRID::spacing)), Vector2Scale(perp, width))     ;

  return {P1, P2, P3, P4};
}

class Triangle {
  public:
    Vector3 P1;
    Vector3 P2;
    Vector3 P3;
};


static Vector3 Get_Centroid(Vector3 P1, Vector3 P2, Vector3 P3){
  float x = (P1.x + P2.x + P3.x) / 3;
  float y = (P1.y + P2.y + P3.y) / 3;
  float z = (P1.z + P2.z + P3.z) / 3;

  return {x, y, z};
}

static Triangle Get_Slightly_Bigger_Triangle(Vector3 P1, Vector3 P2, Vector3 P3){
  Vector3 Centroid = Get_Centroid(P1, P2, P3);

  Vector3 C_P1 = Vector3Normalize(Vector3Subtract(P1, Centroid));
  Vector3 C_P2 = Vector3Normalize(Vector3Subtract(P2, Centroid));
  Vector3 C_P3 = Vector3Normalize(Vector3Subtract(P3, Centroid));

  Vector3 New_P1 = Vector3Add(P1, Vector3Scale(C_P1, 0.01));
  Vector3 New_P2 = Vector3Add(P2, Vector3Scale(C_P2, 0.01));
  Vector3 New_P3 = Vector3Add(P3, Vector3Scale(C_P3, 0.01));

  return {New_P1, New_P2, New_P3};
}


static Vector3 Get_Closest_Point_to_Reference(std::vector<Vector3> points, Vector3 reference){
  float d = Vector3Distance(points[0], reference);
  Vector3 closest = points[0];
  for(int i = 0; i < points.size(); i++){
    if(Vector3Distance(points[i], reference) < d) {
      d = Vector3Distance(points[i], reference);
      closest = points[i];
    }
  }

  return closest;
}

static Rectangle Scale_Rec_Centered(Rectangle Rect, float x, float y){

  Rectangle Scaled_Rect = Rect;
  Scaled_Rect.x += x;
  Scaled_Rect.y += y;
  Scaled_Rect.width -= x*2;
  Scaled_Rect.height -= y*2;

  return Scaled_Rect;
}

static bool CheckCollisionLineRec(Vector2 p1, Vector2 p2, RectanglePoints Rect) {
  Rectangle R = {Rect.P1.x, Rect.P1.y, Rect.P4.x - Rect.P1.x, Rect.P4.y - Rect.P1.y};
  if (CheckCollisionPointRec(p1, R) || CheckCollisionPointRec(p2, R)) {
      return true;
  }

  Vector2 col_p;
  if(CheckCollisionLines(p1, p2, Rect.P1, Rect.P2, &col_p)) return true;
  if(CheckCollisionLines(p1, p2, Rect.P2, Rect.P4, &col_p)) return true;
  if(CheckCollisionLines(p1, p2, Rect.P4, Rect.P3, &col_p)) return true;
  if(CheckCollisionLines(p1, p2, Rect.P3, Rect.P1, &col_p)) return true;

  return false;
}

