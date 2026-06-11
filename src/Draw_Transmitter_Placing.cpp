#include"../include/Draw_Transmitter_Placing.hpp"

#include<raylib.h>
#include<vector>
#include"../include/Config.hpp"
#include"../include/Utilities.hpp"
#include"../include/Floor_Plan.hpp"
#include "Transmitters.hpp"

bool IsPointOverUI_Transmitter(Vector2 screen_pos) {
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

void Draw_Transmitter_Placing(){
  Vector2 pos_w = CONF::Enable_Snapping
                  ? Get_Snapped_World_Mouse_Position() 
                  : GetScreenToWorld2D(GetMousePosition(), CONF::camera2);
  Vector2 pos_s = GetWorldToScreen2D(pos_w, CONF::camera2);

  // to check if the mouse is on top of a UI element in which case placing a point shouldn't be allowed
  bool Is_Placable = !IsPointOverUI_Transmitter(pos_s) && !IsKeyDown(KEY_SPACE);

  Vector2 Image_Pos = {pos_s.x - CONF::Transmitter_Image.width*CONF::camera2.zoom/2.f, pos_s.y - CONF::Transmitter_Image.height*CONF::camera2.zoom/2.f};
  DrawTextureEx(CONF::Transmitter_Image, Image_Pos, 0.0f, CONF::camera2.zoom, WHITE);

  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and Is_Placable and FP::Transmitters.size() < 32){
    TRANSMITTER T;
    T.Position.x = pos_w.x/CONF::Grid_Spacing;
    T.Position.y = -pos_w.y/CONF::Grid_Spacing;
    T.Position.z = CONF::Transmitter_Height;
    FP::Transmitters.push_back(T);
  }

  if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) or IsKeyPressed(KEY_ESCAPE)){
    CONF::Selected_Tool = None_Tool;
  }

  if(IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_Z)){
    if(FP::Transmitters.size() > 0) FP::Transmitters.pop_back();
  }

}
