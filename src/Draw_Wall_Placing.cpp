#include"../include/Draw_Wall_Placing.hpp"

#include<raylib.h>
#include<algorithm>
#include"../include/Config.hpp"
#include"../include/Utilities.hpp"
#include"../include/Walls.hpp"
#include"../include/Floor_Plan.hpp"

// the walls ends should have three modes :
// - Circular
// - Rectangular
// - Automatic (default) : if the wall is not connected or if it is connected in a 90 degree angle make it rectangular else make it circular

bool IsPointOverUI(Vector2 screen_pos) {
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

void Draw_Wall_Placing(){

  Vector2 Mouse_Screen_Position = GetMousePosition();
  Vector2 Mouse_World_Position = GetScreenToWorld2D(Mouse_Screen_Position, CONF::camera2);
  Vector2 Snapped_World_Mouse_Position = Get_Snapped_World_Mouse_Position();
  Vector2 Snapped_Screen_Mouse_Position = GetWorldToScreen2D(Snapped_World_Mouse_Position, CONF::camera2);

  Vector2 Active_Screen_Pos = CONF::Enable_Snapping ? Snapped_Screen_Mouse_Position : Mouse_Screen_Position;
  Vector2 Active_World_Pos  = CONF::Enable_Snapping ? Snapped_World_Mouse_Position : Mouse_World_Position;

  DrawCircleV(Active_Screen_Pos, 5, BLUE);

  // to check if the mouse is on top of a UI element in which case placing a point shouldn't be allowed
  bool Is_Placable = !IsPointOverUI(Active_Screen_Pos) && !IsKeyDown(KEY_SPACE);

  // Setting the start of the wall
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and Is_Placable){
    if(!CONF::Is_Wall_Start_Available){
      CONF::Is_Wall_Start_Available = true;
      CONF::Current_Wall_Start_World  = Active_World_Pos;
    }
    else{
      WALL W;
      W.Wall_Start_2D = CONF::Current_Wall_Start_World;
      W.Wall_End_2D = Active_World_Pos;

      W.Wall_Start_3D = { W.Wall_Start_2D.x / CONF::Grid_Spacing, (W.Wall_Start_2D.y / CONF::Grid_Spacing) * -1.0f };
      W.Wall_End_3D   = { W.Wall_End_2D.x / CONF::Grid_Spacing,   (W.Wall_End_2D.y / CONF::Grid_Spacing) * -1.0f };

      W.Wall_Height = CONF::Wall_Height;
      W.Wall_Width = CONF::Wall_Width;
      W.Wall_Material = CONF::Wall_Material;
      FP::Walls_Vec.push_back(W);

      CONF::Current_Wall_Start_World = W.Wall_End_2D;
    }
  }
  CONF::Current_Wall_Start_Screen = GetWorldToScreen2D(CONF::Current_Wall_Start_World, CONF::camera2);

  // Drawing Active Preview
  if(CONF::Is_Wall_Start_Available){
      // the main blue line
      DrawLineEx(CONF::Current_Wall_Start_Screen, Active_Screen_Pos, 3.0f, BLUE);
      // the rectangle red lines
      Draw_Rectangle_Line_Thickness(CONF::Current_Wall_Start_World, Active_World_Pos, CONF::Wall_Width * CONF::Grid_Spacing, RED);
  }

  // to go out of drawing mode
  if(IsKeyPressed(KEY_ESCAPE) or IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
    if(CONF::Is_Wall_Start_Available) CONF::Is_Wall_Start_Available = false;
    else CONF::Selected_Tool = None_Tool;
  }

  // --- UNDO WALL SYSTEM (Ctrl + Z) ---
  if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) && !FP::Walls_Vec.empty()) {
    // Reference the target wall slated for deletion
    const auto& last_wall = FP::Walls_Vec.back();

    // 1. Purge all child doors associated with this wall from the global states
    for (const auto& door : last_wall.Doors_Vec) {
        int id = door.ID;

        // Remove from global floor plan list
        FP::Doors.erase(
            std::remove_if(FP::Doors.begin(), FP::Doors.end(), [id](const DOOR& d) { 
                return d.ID == id; 
            }),
            FP::Doors.end()
        );

        // Remove from tracking index lookup vector
        FP::doors_on_walls_ids.erase(
            std::remove_if(FP::doors_on_walls_ids.begin(), FP::doors_on_walls_ids.end(), [id](int tracking_id) { 
                return tracking_id == id; 
            }),
            FP::doors_on_walls_ids.end()
        );
    }

    // 2. Purge all child windows associated with this wall from the global states
    for (const auto& window : last_wall.Windows_Vec) {
        int id = window.ID;

        // Remove from global floor plan list
        FP::Windows.erase(
            std::remove_if(FP::Windows.begin(), FP::Windows.end(), [id](const WINDOW& w) { 
                return w.ID == id; 
            }),
            FP::Windows.end()
        );

        // Remove from tracking index lookup vector
        FP::windows_on_walls_ids.erase(
            std::remove_if(FP::windows_on_walls_ids.begin(), FP::windows_on_walls_ids.end(), [id](int tracking_id) { 
                return tracking_id == id; 
            }),
            FP::windows_on_walls_ids.end() // Kept CONF namespace here assuming windows vector is stored there based on your old snippet
        );
    }

    // 3. Drop the wall itself and clear placement anchors
    FP::Walls_Vec.pop_back();
    CONF::Is_Wall_Start_Available = false;
}

}
