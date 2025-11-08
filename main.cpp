#include <iostream>
#include "raylib.h"
#include <vector>
#include <math.h>
#include <algorithm>

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
    rays.push_back( BRay({400, 300}, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, 20, 2.4e9, 100e-3/number_of_rays) );
  }

  std::vector<Wall> walls;
  walls.push_back( Wall({200, 200}, {700, 100}) );
  walls.push_back( Wall({700, 100}, {700, 500}) );
  walls.push_back( Wall({700, 500}, {100, 500}) );
  walls.push_back( Wall({100, 500}, {200, 200}) );

  InitWindow(800, 600, "Indoor Propagation Simulator");
  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    for(int j = 0; j < walls.size(); j++){
      walls[j].draw();
    }

    for(int i = 0; i < rays.size(); i++) {
    for(int j = 0; j < walls.size(); j++){
      rays[i].draw();
      rays[i].update(walls[j].start, walls[j].end, walls[j].GetNormal());
    }}

    rays.erase(
    std::remove_if(rays.begin(), rays.end(),
        [](const BRay& ra){ return ra.color.r < 5; }),
    rays.end()
    );

    if(IsKeyPressed(KEY_SPACE)){
      for(int i = 0; i < number_of_rays; i ++){
        rays.push_back( BRay({400, 300}, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, 20, 2.4e9, 100e-3/number_of_rays) );
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
