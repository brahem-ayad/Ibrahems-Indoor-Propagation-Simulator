#include <iostream>
#include "raylib.h"
#include <vector>
#include <math.h>
#include <algorithm>

#include "Wall.h"
#include "Ray.h"

int main() {

  std::vector<BRay> rays;

  float speed = 2;
  Vector2 Start_Pos = {400, 150};

  int number_of_rays = 360*3;
  for(int i = 0; i < number_of_rays; i++){
    rays.push_back( BRay(Start_Pos, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, speed, 2.4e9, 100e-3/number_of_rays) );
  }

  std::vector<Wall> walls;
  walls.push_back( Wall({100, 100}, {700, 100}) );
  walls.push_back( Wall({700, 100}, {700, 500}) );
  walls.push_back( Wall({700, 500}, {250, 500}) );
  walls.push_back( Wall({250, 500}, {250, 400}) );
  walls.push_back( Wall({250, 400}, {600, 400}) );
  walls.push_back( Wall({600, 400}, {600, 200}) );
  walls.push_back( Wall({600, 200}, {200, 200}) );
  walls.push_back( Wall({200, 200}, {200, 500}) );
  walls.push_back( Wall({200, 500}, {100, 500}) );
  walls.push_back( Wall({100, 500}, {100, 100}) );

  InitWindow(800, 600, "Indoor Propagation Simulator");
  SetTargetFPS(60);



  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    float dt = GetFrameTime() * 60;

    for (int i = 0; i < rays.size(); ++i) {
      rays[i].ResolveCollisions(walls, dt);
    }

    for (int i = 0; i < rays.size(); i++){
      rays[i].draw();
    }
    for (int i = 0; i < walls.size(); i++){
      walls[i].draw();
    }

    rays.erase(
    std::remove_if(rays.begin(), rays.end(),
        [](const BRay& ra){ return ra.color.r < 5; }),
    rays.end()
    );

    if(IsKeyPressed(KEY_SPACE)){
      for(int i = 0; i < number_of_rays; i ++){
        rays.push_back( BRay(Start_Pos, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, speed, 2.4e9, 100e-3/number_of_rays) );
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
