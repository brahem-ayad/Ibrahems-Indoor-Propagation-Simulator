#pragma once

#include<raylib.h>
#include"../../Grid.h"
#include"../Floor_Plan.h"

// A tool tip (a rectangle) that appears after you have specified the begining of the wall_line and shows you the length of the wall
// that you are about to place. It is the length between the starting pos and the current mouse pos
static void Draw_Wall_Length_Tooltip(Vector2 pos, Camera2D camera2, Font font_20){
    Vector2 pos_screen = GetWorldToScreen2D(pos, camera2);
    Vector2 wtt = {pos_screen.x+20, pos_screen.y-30}; // wtt = wall tool tip
    float length = Vector2Distance(FP::starting_pos, pos)/GRID::spacing;
    const char *text = TextFormat("Length = %.2f", length);
    Vector2 text_size = MeasureTextEx(font_20, text, 20, 1);
    float margin = 5;
    DrawRectangleRounded({wtt.x - margin, wtt.y - margin, text_size.x + margin*2, text_size.y + margin*2}, 0.5, 10, Fade(LIGHTGRAY, 0.5));

    DrawTextEx(font_20, text, wtt, 20, 1, BLACK);
}
