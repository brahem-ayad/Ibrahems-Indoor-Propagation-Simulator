#include"../include/Draw_Ceiling_Placing.hpp"

#include<raylib.h>
#include<raymath.h>
#include<vector>
#include"../include/Config.hpp"
#include"../include/Utilities.hpp"
#include"../include/Floor_Plan.hpp"
#include"../include/Ceilings.hpp"

bool IsPointOverUI_Ceiling(Vector2 screen_pos) {
  if (!CONF::Show_Tools_Bar) return false;

  // 1. Main menu bar check
  if (CheckCollisionPointRec(screen_pos, {0, 0, (float)GetScreenWidth(), 25.0f})) return true;
 
  // 2. Toolbar check
  if (CheckCollisionPointRec(screen_pos, {0, 0, 138, (float)GetScreenHeight()})) return true;
 
  if (CONF::Show_Tool_Options_Bar) {
   // 3. Ceilings Tool Options check
   if (CheckCollisionPointRec(screen_pos, {128, 0, 150, 155})) return true;

   // 4. Ceilings Tool Options Dropdown Menu Check
   if (CONF::Is_Ceiling_Material_Dropdown_Menu_Down && 
     CheckCollisionPointRec(screen_pos, {164, 150, 110, 75})) return true;
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

void Draw_Ceiling_Placing() {
    Vector2 pos_w = CONF::Enable_Snapping
                    ? Get_Snapped_World_Mouse_Position() 
                    : GetScreenToWorld2D(GetMousePosition(), CONF::camera2);
    Vector2 pos_s = GetWorldToScreen2D(pos_w, CONF::camera2);

  // to check if the mouse is on top of a UI element in which case placing a point shouldn't be allowed
  bool Is_Placable = !IsPointOverUI_Ceiling(pos_s) && !IsKeyDown(KEY_SPACE);

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) && !FP::ceilings.empty()) {
        FP::ceilings.pop_back();
        CONF::Is_Ceiling_Start_Available = false;
        return;
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        CONF::Is_Ceiling_Start_Available = false;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (CONF::Is_Ceiling_Start_Available) {
            CONF::Is_Ceiling_Start_Available = false;
        } else {
            CONF::Selected_Tool = None_Tool;
            return;
        }
    }

    DrawCircleV(pos_s, 5.0f, RED);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CONF::Moving_2d && Is_Placable) {
        if (!CONF::Is_Ceiling_Start_Available) {
            CONF::Is_Ceiling_Start_Available = true;
            CONF::Current_Ceiling_Start_World = pos_w;
        } 
        else {
            Vector2 start_screen = GetWorldToScreen2D(CONF::Current_Ceiling_Start_World, CONF::camera2);
            RectanglePoints points = GetRectanglePoints(start_screen, pos_s);

            Vector2 P1 = GetScreenToWorld2D(points.P1, CONF::camera2);
            Vector2 P2 = GetScreenToWorld2D(points.P2, CONF::camera2);
            Vector2 P3 = GetScreenToWorld2D(points.P3, CONF::camera2);
            Vector2 P4 = GetScreenToWorld2D(points.P4, CONF::camera2);

            if (!Vector2Equals(P1, P4) && Is_Placable) {
                Ceiling ceiling(P1, P2, P3, P4, CONF::Ceiling_Height);
                FP::ceilings.push_back(ceiling);
            }

            CONF::Is_Ceiling_Start_Available = false;
            CONF::Current_Ceiling_Start_World = Vector2Zero(); 
        }
    }

    if (CONF::Is_Ceiling_Start_Available) {
        Vector2 start_screen = GetWorldToScreen2D(CONF::Current_Ceiling_Start_World, CONF::camera2);
        DrawRectanglePoints(start_screen, pos_s, RED, {255, 0, 0, 255}, 0.1f, true);
    }
}
