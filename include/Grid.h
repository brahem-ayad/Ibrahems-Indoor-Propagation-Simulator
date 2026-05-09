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
    float alpha = Remap(camera.zoom, 0.1f, 1.0f, 0.0f, 1.0f);
    Color smallest_grid_lines_color;
    Color smaller_grid_lines_color;
    Color major_grid_lines_color;
    float smaller_lines_fade;
    float smallest_lines_fade;
    if(CONF::Theme == Light_Theme) {
      smallest_grid_lines_color = {210, 210, 210, 255};
      smaller_grid_lines_color = {170, 170, 170, 255};
      major_grid_lines_color = {120, 120, 120, 255};
      smaller_lines_fade = 0.2;
      smaller_lines_fade = Remap(camera.zoom, 0.8, 1.2, 0.0, 0.6);
      smallest_lines_fade = Remap(camera.zoom, 1.5, 2.0, 0.0, 0.6);
    }
    else {
      smaller_grid_lines_color = {40, 40, 40, 255};
      smallest_grid_lines_color = {20, 20, 20, 255};
      major_grid_lines_color = {60, 60, 60, 255};
      smaller_lines_fade = Remap(camera.zoom, 0.8, 1.2, 0.0, 0.6);
      smallest_lines_fade = Remap(camera.zoom, 1.5, 2.0, 0.0, 0.6);
    }


    // Vertical Lines - Smaller Step Size
    if(camera.zoom > 0.8f){

      if(camera.zoom > 1.5f){
        for (float x = x_start; x <= bottomRight.x; x += spacing/10) {
          DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smallest_grid_lines_color, smallest_lines_fade));
        }
      }

      for (float x = x_start; x <= bottomRight.x; x += spacing/2) {
        DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smaller_grid_lines_color, smaller_lines_fade));
      }

    }

    // Horizontal Lines - Smaller Step Size
    if(camera.zoom > 0.8f){

      if(camera.zoom > 1.5f){
        for (float y = y_start; y <= bottomRight.y; y += spacing/10) {
          DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smallest_grid_lines_color, smallest_lines_fade));
        }
      }

      for (float y = y_start; y <= bottomRight.y; y += spacing/2) {
        DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smaller_grid_lines_color, smaller_lines_fade));
      }

    }

    // Draw Vertical Lines
    for (float x = x_start; x <= bottomRight.x; x += spacing) {
      DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smaller_grid_lines_color, alpha));
    }

    // Draw Horizontal Lines

    for (float y = y_start; y <= bottomRight.y; y += spacing) {
      DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smaller_grid_lines_color, alpha));
    }

    // major vertical line
    DrawLineV({ 0, topLeft.y }, { 0, bottomRight.y }, Fade(major_grid_lines_color, alpha));

    // major horizontal line
    DrawLineV({ topLeft.x, 0 }, { bottomRight.x, 0 }, Fade(major_grid_lines_color, alpha));

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
    DrawLine3D({(float)i, -(float)n, -0.02}, {(float)i, (float)n, -0.02}, grid_lines_color);
    if(i == 0){
      DrawLine3D({(float)i, -(float)n, -0.02}, {(float)i, (float)n, -0.02}, major_grid_lines_color);
    }
  }
  for(int i = -n; i <= n; i++){
    DrawLine3D({-(float)n, (float)i, -0.02}, {(float)n, (float)i, -0.02}, grid_lines_color);
    if(i == 0){
      DrawLine3D({-(float)n, (float)i, -0.02}, {(float)n, (float)i, -0.02}, major_grid_lines_color);
    }
  }
}
