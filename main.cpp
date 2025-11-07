#include <iostream>
#include "raylib.h"
#include <vector>
#include <math.h>

#include "Wall.h"
#include "Ray.h"

Vector2 GetSnappedMousePosition(int gridSize) {
    Vector2 pos = GetMousePosition();
    pos.x = round(pos.x / gridSize) * gridSize;
    pos.y = round(pos.y / gridSize) * gridSize;
    return pos;
}

int main() {

  std::vector<BRay> rays;

  int number_of_rays = 256;
  for(int i = 0; i < number_of_rays; i ++){
    rays.push_back( BRay({400, 300}, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, 10, 2.4e9, 100e-3/number_of_rays) );
  }

  std::vector<Wall> walls;
  walls.push_back( Wall({100, 100}, {700, 100}) );
  walls.push_back( Wall({700, 105}, {700, 500}) );
  walls.push_back( Wall({700, 500}, {500, 500}) );
  walls.push_back( Wall({500, 500}, {500, 405}) );
  walls.push_back( Wall({500, 400}, {400, 400}) );
  walls.push_back( Wall({400, 405}, {400, 500}) );
  walls.push_back( Wall({400, 500}, {100, 500}) );
  walls.push_back( Wall({100, 500}, {100, 100}) );


  InitWindow(800, 600, "Indoor Propagation Simulator");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    for(int i = 0; i < rays.size(); i++) {
    for(int j = 0; j < walls.size(); j++){
      walls[j].draw();

      rays[i].draw();
      rays[i].update(walls[j].wall_rect(), walls[j].GetNormal());
    }}


    EndDrawing();
  }

  CloseWindow();

  return 0;
}
