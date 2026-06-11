#include"../include/View_or_Hide_With_Shortcuts.hpp"

#include<raylib.h>
#include"../include/Config.hpp"

void View_or_Hide_With_Shortcuts(){
  if(IsKeyDown(KEY_LEFT_ALT)){
    if(IsKeyPressed(KEY_G)) CONF::Show_Grid = !CONF::Show_Grid;
    else if(IsKeyPressed(KEY_T)) CONF::Show_Tools_Bar = !CONF::Show_Tools_Bar;
    else if(IsKeyPressed(KEY_Y)) CONF::Show_Tool_Options_Bar = !CONF::Show_Tool_Options_Bar;
    else if(IsKeyPressed(KEY_M)) CONF::Show_Right_Sidebar = !CONF::Show_Right_Sidebar;
    else if(IsKeyPressed(KEY_V)) CONF::Show_Viewport_Gizmo = !CONF::Show_Viewport_Gizmo;
    else if(IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
      CONF::Fullscreen_Mode = !CONF::Fullscreen_Mode;
    }
    else if(IsKeyPressed(KEY_L)) CONF::View_Walls_Wireframe = !CONF::View_Walls_Wireframe;
  }
}
