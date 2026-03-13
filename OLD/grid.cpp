#include"grid.hpp"
#include<iostream>
#include<cmath>

#include<raylib.h>

void Draw_Grid(Vector2 camera_target, Vector2 camera_offset, float camera_zoom){

  float worldWidth  = conf::screen_width  / camera_zoom;
  float worldHeight = conf::screen_height / camera_zoom;
  float left = camera_target.x - camera_offset.x / camera_zoom;
  float top  = camera_target.y - camera_offset.y / camera_zoom;
  float right  = left + worldWidth;
  float bottom = top  + worldHeight;

  // drawing the vertical lines
  for(int i = conf::grid_size*std::floor(left/conf::grid_size); i < right; i += conf::grid_size){
    DrawRectangle(i, top, conf::grid_lines_width, bottom-top, conf::grid_color);
  }

  // drawing the horizontal lines
  for(int i = conf::grid_size*std::floor(top/conf::grid_size); i < bottom; i += conf::grid_size){
    DrawRectangle(left, i, right-left, conf::grid_lines_width, conf::grid_color);
  }

}
