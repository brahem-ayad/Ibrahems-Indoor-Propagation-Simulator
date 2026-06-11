#include"../include/Grid.hpp"

#include<raylib.h>
#include<raymath.h>

#include<cmath>

#include"Config.hpp"

void Draw_2D_Grid(){
    // Get the world coordinates of the viewport corners
    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, CONF::camera2);
    Vector2 bottomRight = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, CONF::camera2);

    // Find the starting point by rounding the top-left coordinate DOWN to the nearest CONF::Grid_Spacing
    // floorf(x / CONF::Grid_Spacing) * CONF::Grid_Spacing ensures we align perfectly to the grid globally
    float x_start = floorf(topLeft.x / CONF::Grid_Spacing) * CONF::Grid_Spacing;
    float y_start = floorf(topLeft.y / CONF::Grid_Spacing) * CONF::Grid_Spacing;

    // Opacity scaling based on zoom
    float alpha = Remap(CONF::camera2.zoom, 0.1f, 1.0f, 0.0f, 1.0f);
    Color smallest_grid_lines_color;
    Color smaller_grid_lines_color;
    Color major_grid_lines_color;
    float smaller_lines_fade;
    float smallest_lines_fade;
      smallest_grid_lines_color = {210, 210, 210, 255};
      smaller_grid_lines_color = {170, 170, 170, 255};
      major_grid_lines_color = {120, 120, 120, 255};
      smaller_lines_fade = 0.2;
      smaller_lines_fade = Remap(CONF::camera2.zoom, 0.8, 1.2, 0.0, 0.6);
      smallest_lines_fade = Remap(CONF::camera2.zoom, 1.5, 2.0, 0.0, 0.6);



    // Vertical Lines - Smaller Step Size
    if(CONF::camera2.zoom > 0.8f){

      if(CONF::camera2.zoom > 1.5f){
        for (float x = x_start; x <= bottomRight.x; x += CONF::Grid_Spacing/10) {
          DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smallest_grid_lines_color, smallest_lines_fade));
        }
      }

      for (float x = x_start; x <= bottomRight.x; x += CONF::Grid_Spacing/2) {
        DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smaller_grid_lines_color, smaller_lines_fade));
      }

    }

    // Horizontal Lines - Smaller Step Size
    if(CONF::camera2.zoom > 0.8f){

      if(CONF::camera2.zoom > 1.5f){
        for (float y = y_start; y <= bottomRight.y; y += CONF::Grid_Spacing/10) {
          DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smallest_grid_lines_color, smallest_lines_fade));
        }
      }

      for (float y = y_start; y <= bottomRight.y; y += CONF::Grid_Spacing/2) {
        DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smaller_grid_lines_color, smaller_lines_fade));
      }

    }

    // Draw Vertical Lines
    for (float x = x_start; x <= bottomRight.x; x += CONF::Grid_Spacing) {
      DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, Fade(smaller_grid_lines_color, alpha));
    }

    // Draw Horizontal Lines

    for (float y = y_start; y <= bottomRight.y; y += CONF::Grid_Spacing) {
      DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, Fade(smaller_grid_lines_color, alpha));
    }

    // major vertical line
    DrawLineV({ 0, topLeft.y }, { 0, bottomRight.y }, Fade(major_grid_lines_color, alpha));

    // major horizontal line
    DrawLineV({ topLeft.x, 0 }, { bottomRight.x, 0 }, Fade(major_grid_lines_color, alpha));

}

void Draw_3D_Grid() {
  int n = 100;

  Color grid_lines_color;
  Color major_grid_lines_color;
  grid_lines_color = Fade(GRAY, 0.5);
  major_grid_lines_color = Fade(DARKGRAY, 0.5);

  SetShaderValue(CONF::Grid_3D_Shader, CONF::Grid_3D_Shader_Uniform_Camera_Position_ID, &CONF::camera3.position, SHADER_UNIFORM_VEC3);
  SetShaderValue(CONF::Grid_3D_Shader, CONF::Grid_3D_Shader_Uniform_Camera_Target_ID, &CONF::camera3.target, SHADER_UNIFORM_VEC3);

  BeginShaderMode(CONF::Grid_3D_Shader);
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
  EndShaderMode();
}
