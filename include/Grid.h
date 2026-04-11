#pragma once

#include<raylib.h>
#include<raymath.h>

#include"Config.h"

static void Draw_2D_Grid(Camera2D camera) {
    const float spacing = GRID::spacing;

    // Get the world coordinates of the viewport corners
    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

    // Find the starting point by rounding the top-left coordinate DOWN to the nearest spacing
    // floorf(x / spacing) * spacing ensures we align perfectly to the grid globally
    float x_start = floorf(topLeft.x / spacing) * spacing;
    float y_start = floorf(topLeft.y / spacing) * spacing;

    // Opacity scaling based on zoom
    float alpha = Remap(camera.zoom, 0.3f, 2.0f, 0.2f, 0.9f);
    Color grid_lines_color;
    Color major_grid_lines_color;
    if(CONF::Theme == Light_Theme) {
      grid_lines_color = Fade(GRAY, alpha);
      major_grid_lines_color = Fade(DARKGRAY, alpha);
    }
    else {
      grid_lines_color = Fade({50, 50, 50, 255}, alpha);
      major_grid_lines_color = Fade({80, 80, 80, 255}, alpha);
    }

    // Draw Vertical Lines
    for (float x = x_start; x <= bottomRight.x; x += spacing) {
        DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, grid_lines_color);
        if(x == 0){
          DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, major_grid_lines_color);
        }
    }

    // Draw Horizontal Lines
    for (float y = y_start; y <= bottomRight.y; y += spacing) {
        DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, grid_lines_color);
        if(y == 0){
          DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, major_grid_lines_color);
        }
    }
}

static void Draw_3D_Grid() {
  const float spacing = GRID::spacing;

  int n = 100;

    Color grid_lines_color;
    Color major_grid_lines_color;
    if(CONF::Theme == Light_Theme) {
      grid_lines_color = Fade(GRAY, 0.5);
      major_grid_lines_color = Fade(DARKGRAY, 0.5);
    }
    else {
      grid_lines_color = Fade({50, 50, 50, 255}, 0.5);
      major_grid_lines_color = Fade({80, 80, 80, 255}, 0.5);
    }

  for(int i = -n; i <= n; i++){
    DrawLine3D({(float)i, -(float)n, 0}, {(float)i, (float)n, 0}, grid_lines_color);
    if(i == 0){
      DrawLine3D({(float)i, -(float)n, 0}, {(float)i, (float)n, 0}, major_grid_lines_color);
    }
  }
  for(int i = -n; i <= n; i++){
    DrawLine3D({-(float)n, (float)i, 0}, {(float)n, (float)i, 0}, grid_lines_color);
    if(i == 0){
      DrawLine3D({-(float)n, (float)i, 0}, {(float)n, (float)i, 0}, major_grid_lines_color);
    }
  }
}
