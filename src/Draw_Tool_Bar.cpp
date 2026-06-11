#include"../include/Draw_Tool_Bar.hpp"
#include "Config.hpp"

#include<raylib.h>

void Draw_Tool_Bar(){

  if(GetMousePosition().x < 138 and GetScreenHeight() < 812 + 10 ){
    CONF::Tools_Bar_Scroll += GetMouseWheelMove() * 20;
  }
  if(CONF::Tools_Bar_Scroll < GetScreenHeight() - 812 - 50) CONF::Tools_Bar_Scroll =  GetScreenHeight() - 812 - 50;
  if(CONF::Tools_Bar_Scroll > 0) CONF::Tools_Bar_Scroll = 0;

  // the bacground
  DrawRectangleRoundedLinesEx({10, 22 + 10 + 2 + CONF::Tools_Bar_Scroll, 128, 584 + 228}, 0.3, 15, 2.0f, {128, 128, 128, 255});
  DrawRectangleRounded({10, 22 + 10 + CONF::Tools_Bar_Scroll, 128, 584 + 228}, 0.3, 15, {229, 229, 229, 200});
  DrawRectangleRoundedLinesEx({10, 22 + 10 + CONF::Tools_Bar_Scroll, 128, 584 + 228}, 0.3, 15, 2.0f, {128, 128, 128, 255});

  // A secondary Outline that is shifted down to look like a shadow and a gray rectangle for each button to hide the top part of the secondary outline
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  // 1 ) Walls Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 2 ) Doors Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 3 ) Windows Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 4 ) Floors Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 5 ) Ceilings Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 6 ) Transmitters Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});
  // 7 ) Receivers Shadow
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {229, 229, 229, 255});

  // if a tool is selected then its button should be blue
  // ----------------------------------------------------------------------------------
  // 1 ) Walls Selected Rect
  if(CONF::Selected_Tool == Walls_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 2 ) Doors Selected Rect
  if(CONF::Selected_Tool == Doors_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 3 ) Windows Selected Rect
  if(CONF::Selected_Tool == Windows_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 4 ) Floors Selected Rect
  if(CONF::Selected_Tool == Floors_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 5 ) Ceilings Selected Rect
  if(CONF::Selected_Tool == Ceilings_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 6 ) Transmitters Selected Rect
  if(CONF::Selected_Tool == Transmitter_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }
  // 7 ) Receivers Selected Rect
  if(CONF::Selected_Tool == Receiver_Tool){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, Fade(SKYBLUE,0.4));
  }

  // if a button is hovered over by the mouse it's color should change to indicate that
  // ----------------------------------------------------------------------------------
  // 1 ) Walls Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Walls_Tool) CONF::Selected_Tool = Walls_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 2 ) Doors Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Doors_Tool) CONF::Selected_Tool = Doors_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 3 ) Windows Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Windows_Tool) CONF::Selected_Tool = Windows_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 4 ) Floors Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Floors_Tool) CONF::Selected_Tool = Floors_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 5 ) Ceilings Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Ceilings_Tool) CONF::Selected_Tool = Ceilings_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 6 ) Transmitters Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Transmitter_Tool) CONF::Selected_Tool = Transmitter_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }
  // 7 ) Receivers Hover Rect
  if(CheckCollisionPointRec(GetMousePosition(), {10 + 14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100})){
    DrawRectangleRounded({10 + 14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, {255, 255, 255, 100});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and CONF::Selected_Tool != Receiver_Tool) CONF::Selected_Tool = Receiver_Tool;
    else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Selected_Tool = None_Tool;
  }

  // the button's outlines
  // ---------------------
  // 1 ) Walls Outlines
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the doors outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the windows outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the floors outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the ceilings outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the transmitter outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});
  // the receivers outline
  DrawRectangleRoundedLinesEx({10 + 14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, 100, 100}, 0.3, 15, 3.4f, {18, 18, 18, 255});

  // the icons
  DrawTexture(CONF::Walls_Tool_Icon, 10 + 14, 22 + 10 + 14 + 0*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Doors_Tool_Icon, 10+14, 22 + 10 + 14 + 1*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Windows_Tool_Icon, 10+14, 22 + 10 + 14 + 2*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Floors_Tool_Icon, 10+14, 22 + 10 + 14 + 3*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Ceilings_Tool_Icon, 10+14, 22 + 10 + 14 + 4*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Transmitters_Tool_Icon, 10+14, 22 + 10 + 14 + 5*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);
  DrawTexture(CONF::Receivers_Tool_Icon, 10+14, 22 + 10 + 14 + 6*(100 + 14) + CONF::Tools_Bar_Scroll, WHITE);

}
