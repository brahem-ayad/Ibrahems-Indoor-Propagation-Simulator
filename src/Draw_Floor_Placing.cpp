#include "../include/Draw_Floor_Placing.hpp"
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "../include/Config.hpp"
#include "../include/Utilities.hpp"
#include "../include/Floor_Plan.hpp"
#include "../include/Floors.hpp"

bool IsPointOverUI_Floor(Vector2 screen_pos) {
  if (!CONF::Show_Tools_Bar) return false;

  // 1. Main menu bar check
  if (CheckCollisionPointRec(screen_pos, {0, 0, (float)GetScreenWidth(), 25.0f})) return true;
 
  // 2. Toolbar check
  if (CheckCollisionPointRec(screen_pos, {0, 0, 138, (float)GetScreenHeight()})) return true;
 
  if (CONF::Show_Tool_Options_Bar) {
   // 3. Walls Tool Options check
   if (CheckCollisionPointRec(screen_pos, {128, 0, 150, 197})) return true;
 
   // 4. Walls Tool Options Dropdown Menu Check
   if (CONF::Is_Wall_Material_Dropdown_Menu_Down && 
     CheckCollisionPointRec(screen_pos, {160, 190, 110, 140})) return true;
  }
 
  // 5. Viewport gizmo check
  if (CONF::Show_Viewport_Gizmo) {
      if (CheckCollisionPointCircle(screen_pos, {(float)GetScreenWidth() - 70, 22 + 10 + 50}, 60) ||
        CheckCollisionPointCircle(screen_pos, {(float)GetScreenWidth() - 70 + 35, 22 + 10 + 50 + 35 + 35}, 18)) {
        return true;
      }
  }

  return false;
}

void Draw_Floor_Placing() {
  // 1. GATHER INTERACTION INPUTS UPFRONT
  Vector2 pos_w = CONF::Enable_Snapping
                  ? Get_Snapped_World_Mouse_Position() 
                  : GetScreenToWorld2D(GetMousePosition(), CONF::camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, CONF::camera2);

  // to check if the mouse is on top of a UI element in which case placing a point shouldn't be allowed
  bool Is_Placable = !IsPointOverUI_Floor(pos_s) && !IsKeyDown(KEY_SPACE);

  // 2. UNDO PATTERN (Early Exit Strategy)
  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) && !FP::floors.empty()) {
      FP::floors.pop_back();
      CONF::Is_Floor_Start_Available = false; 
      return;
  }

  // 3. CANCEL INPUT LOOKUPS
  if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      CONF::Is_Floor_Start_Available = false;
  }

  if (IsKeyPressed(KEY_ESCAPE)) {
      if (CONF::Is_Floor_Start_Available) {
          CONF::Is_Floor_Start_Available = false;
      } else {
          CONF::Selected_Tool = None_Tool;
          return;
      }
  }

  // 4. MOUSE PLACEMENT HANDLING
  // Render the snap indicator anchor circle
  DrawCircleV(pos_s, 5.0f, DARKGRAY);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CONF::Moving_2d) {
      if (!CONF::Is_Floor_Start_Available and Is_Placable) {
          // First Click: Set anchor point in world space
          CONF::Is_Floor_Start_Available = true;
          CONF::Current_Floor_Start_World = pos_w;
      } 
      else {
          // Second Click: Extract bounding box configurations
          Vector2 start_screen = GetWorldToScreen2D(CONF::Current_Floor_Start_World, CONF::camera2);
          RectanglePoints points = GetRectanglePoints(start_screen, pos_s);

          // Translate geometric corners back safely to world space structures
          Vector2 P1 = GetScreenToWorld2D(points.P1, CONF::camera2);
          Vector2 P2 = GetScreenToWorld2D(points.P2, CONF::camera2);
          Vector2 P3 = GetScreenToWorld2D(points.P3, CONF::camera2);
          Vector2 P4 = GetScreenToWorld2D(points.P4, CONF::camera2);

          // Filter out empty clicks/lines
          if (!Vector2Equals(P1, P4) and Is_Placable) {
              Floor new_floor(P1, P2, P3, P4);
              FP::floors.push_back(new_floor);
          }

          // Fixed coordinate bug: Reset states cleanly
          CONF::Is_Floor_Start_Available = false;
          CONF::Current_Floor_Start_World = Vector2Zero(); 
      }
  }

  // 5. LIVE DRAG PREVIEW (Moved outside the click check so it draws continuously)
  if (CONF::Is_Floor_Start_Available) {
      Vector2 start_screen = GetWorldToScreen2D(CONF::Current_Floor_Start_World, CONF::camera2);
      DrawRectanglePoints(start_screen, pos_s, DARKGRAY, GRAY, 0.3f, true);
  }
}
