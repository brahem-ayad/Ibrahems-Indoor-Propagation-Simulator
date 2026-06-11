#include"../include/Utilities.hpp"

#include"../include/Config.hpp"
#include<raylib.h>
#include<raymath.h>
#include<algorithm>
#include"../include/Generate_Wall_Collision_Quads.hpp"

Vector2 Get_Snapped_World_Mouse_Position(){
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), CONF::camera2);

  float spacing;
  if(CONF::camera2.zoom < 0.9f) spacing = CONF::Grid_Spacing;
  else if(CONF::camera2.zoom < 1.5f) spacing = CONF::Grid_Spacing/2;
  else spacing = CONF::Grid_Spacing/10;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  return position;
}

void Draw_Rectangle_Line_Thickness(Vector2 Line_Starting_World_Pos, Vector2 Line_End_World_Pos, float Rectangle_2D_Width, Color color){
  Vector2 V1_W, V2_W, V3_W, V4_W;

  Vector2 direction = Vector2Normalize(Vector2Subtract(Line_End_World_Pos, Line_Starting_World_Pos));
  Vector2 direction_perp = {direction.y, -direction.x};

  V1_W = Vector2Add(Line_End_World_Pos, Vector2Scale(direction_perp, Rectangle_2D_Width/2));
  V1_W = Vector2Add(V1_W, Vector2Scale(direction, Rectangle_2D_Width/2));
  DrawCircleV(GetWorldToScreen2D(V1_W, CONF::camera2), 5, color);

  V2_W = Vector2Add(Line_End_World_Pos, Vector2Scale(direction_perp, -Rectangle_2D_Width/2));
  V2_W = Vector2Add(V2_W, Vector2Scale(direction, Rectangle_2D_Width/2));
  DrawCircleV(GetWorldToScreen2D(V2_W, CONF::camera2), 5, color);

  V3_W = Vector2Add(Line_Starting_World_Pos, Vector2Scale(direction_perp, -Rectangle_2D_Width/2));
  V3_W = Vector2Add(V3_W, Vector2Scale(direction, -Rectangle_2D_Width/2));
  DrawCircleV(GetWorldToScreen2D(V3_W, CONF::camera2), 5, color);

  V4_W = Vector2Add(Line_Starting_World_Pos, Vector2Scale(direction_perp, Rectangle_2D_Width/2));
  V4_W = Vector2Add(V4_W, Vector2Scale(direction, -Rectangle_2D_Width/2));
  DrawCircleV(GetWorldToScreen2D(V4_W, CONF::camera2), 5, color);

  DrawLineEx(GetWorldToScreen2D(V1_W, CONF::camera2), GetWorldToScreen2D(V2_W, CONF::camera2), 3.0f, color);
  DrawLineEx(GetWorldToScreen2D(V2_W, CONF::camera2), GetWorldToScreen2D(V3_W, CONF::camera2), 3.0f, color);
  DrawLineEx(GetWorldToScreen2D(V3_W, CONF::camera2), GetWorldToScreen2D(V4_W, CONF::camera2), 3.0f, color);
  DrawLineEx(GetWorldToScreen2D(V4_W, CONF::camera2), GetWorldToScreen2D(V1_W, CONF::camera2), 3.0f, color);
}

void Draw_Oriented_Rectangle_on_Line(Vector2 LS, Vector2 LE, Vector2 P, float RW, float RT, bool &Is_Placable){
  // 1. Calculate the line segment vector and its squared length
  float dx = LE.x - LS.x;
  float dy = LE.y - LS.y;
  float line_len_sq = dx * dx + dy * dy;

  // Handle edge case where LS and LE are the same point
  if (line_len_sq == 0.0f) return; 

  // 2. Project point P onto the line segment to find the rectangle's center
  float t = ((P.x - LS.x) * dx + (P.y - LS.y) * dy) / line_len_sq;
 
  // Clamp t to ensure the rectangle stays bounded by the line segment endpoints
  if (t < 0.0f) t = 0.0f;
  if (t > 1.0f) t = 1.0f;

  // This is the center point of your rectangle on the line
  Vector2 center;
  center.x = LS.x + t * dx;
  center.y = LS.y + t * dy;

  if(Vector2Distance(center, LS) < RW/2 or Vector2Distance(center, LE) < RW/2){
    Is_Placable = false;
    return ;
  }

  // 3. Calculate unit vectors for orientation
  float line_len = std::sqrt(line_len_sq);
 
  // Direction vector ALONG the line (X-Axis of the rectangle)
  Vector2 u_along = { dx / line_len, dy / line_len };
 
  // Direction vector PERPENDICULAR to the line (Y-Axis of the rectangle)
  // (Rotated 90 degrees counter-clockwise)
  Vector2 u_perp = { -u_along.y, u_along.x };

  // 4. Calculate half-extents
  float half_width = RW * 0.5f;
  float half_thick = RT * 0.5f;

  // 5. Compute the 4 corner points
  // We combine the center with the directional offsets
  Vector2 P1, P2, P3, P4;

  // Top-Left (relative to line direction)
  P1.x = center.x - (u_along.x * half_width) + (u_perp.x * half_thick);
  P1.y = center.y - (u_along.y * half_width) + (u_perp.y * half_thick);

  // Bottom-Left
  P2.x = center.x - (u_along.x * half_width) - (u_perp.x * half_thick);
  P2.y = center.y - (u_along.y * half_width) - (u_perp.y * half_thick);

  // Bottom-Right
  P3.x = center.x + (u_along.x * half_width) - (u_perp.x * half_thick);
  P3.y = center.y + (u_along.y * half_width) - (u_perp.y * half_thick);

  // Top-Right
  P4.x = center.x + (u_along.x * half_width) + (u_perp.x * half_thick);
  P4.y = center.y + (u_along.y * half_width) + (u_perp.y * half_thick);

  Vector2 PW1 = GetWorldToScreen2D(P1, CONF::camera2);
  Vector2 PW2 = GetWorldToScreen2D(P2, CONF::camera2);
  Vector2 PW3 = GetWorldToScreen2D(P3, CONF::camera2);
  Vector2 PW4 = GetWorldToScreen2D(P4, CONF::camera2);

  Color color;
  if(CONF::Selected_Tool == Doors_Tool){
    if(CONF::Door_Material == Empty_Door) color = RAYWHITE;
    if(CONF::Door_Material == Wooden_Door) color = {217, 186, 140, 255};
    if(CONF::Door_Material == Metal_Door) color = {102, 128, 153, 255};
  }

  DrawTriangle(PW1, PW3, PW2, color);
  DrawTriangle(PW1, PW4, PW3, color);

  DrawLineEx(PW1, PW2, 2*CONF::camera2.zoom, BLACK);
  DrawLineEx(PW3, PW4, 2*CONF::camera2.zoom, BLACK);
}

RectanglePoints GetRectanglePoints(Vector2 P1, Vector2 P2){

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

void DrawRectanglePoints(Vector2 P1, Vector2 P2, Color line_color, Color rectangle_color, float rectangle_opacity, bool Draw_End_Points){

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

Vector2 Get_Closest_Point_on_Segment(Vector2 p, Vector2 a, Vector2 b) {
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

void Draw_Rectangle_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width, Color color){
  Vector2 dir = normalized_line_parallel_vector;
  Vector2 perp = {-dir.y, dir.x};

  Vector2 P1 = GetWorldToScreen2D(Vector2Add(Vector2Add(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width)), CONF::camera2);
  Vector2 P2 = GetWorldToScreen2D(Vector2Add(Vector2Subtract(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width)), CONF::camera2);
  Vector2 P3 = GetWorldToScreen2D(Vector2Subtract(Vector2Subtract(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width)), CONF::camera2);
  Vector2 P4 = GetWorldToScreen2D(Vector2Subtract(Vector2Add(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width)), CONF::camera2);

  DrawLineEx(P1, P2, 1, color);
  DrawLineEx(P2, P3, 1, color);
  DrawLineEx(P3, P4, 1, color);
  DrawLineEx(P4, P1, 1, color);
}

RectanglePoints Get_Rectangle_Points_Around_Lines(Vector2 center, Vector2 normalized_line_parallel_vector, float length, float width){
  Vector2 dir = normalized_line_parallel_vector;
  Vector2 perp = {-dir.y, dir.x};

  Vector2 P1 = Vector2Add(Vector2Add(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width));
  Vector2 P2 = Vector2Add(Vector2Subtract(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width));
  Vector2 P3 = Vector2Subtract(Vector2Subtract(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width));
  Vector2 P4 = Vector2Subtract(Vector2Add(center, Vector2Scale(dir, length/2 * CONF::Grid_Spacing)), Vector2Scale(perp, width));

  return {P1, P2, P3, P4};
}

Vector3 Get_Closest_Point_to_Reference(std::vector<Vector3> points, Vector3 reference){
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

RayCollision Get_Ray_Collision_Quad(Ray R, WallQuad3D Q){
  return GetRayCollisionQuad(R, Q.v1, Q.v2, Q.v3, Q.v4);
}
