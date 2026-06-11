#include"../include/Draw_The_Run_Simulation_Button.hpp"

#include<raylib.h>
#include"../include/Config.hpp"
#include"../include/Run_Simulation.hpp"

void Draw_The_Run_Simulation_Button(){
  DrawRectangleRoundedLinesEx({10, 22 + 10 + 2, 150, 30}, 0.5, 15, 2.0f, {128, 128, 128, 255});
  DrawRectangleRounded({10, 22 + 10, 150, 30}, 0.5, 15, {229, 229, 229, 200});
  DrawRectangleRoundedLinesEx({10, 22 + 10, 150, 30}, 0.5, 15, 2.0f, {128, 128, 128, 255});

  DrawTextEx(CONF::Montserrat_Font_20, "Run Simulation", {23, 38}, 20, 0, BLACK);

  if(CheckCollisionPointRec(GetMousePosition(), {10, 22 + 10, 150, 30})){
    DrawRectangleRounded({10, 22 + 10, 150, 30}, 0.5, 15, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      Run_Simulation();
    }
  }

}
