#include "raylib.h"
#include <vector>
#include <math.h>

#include "Wall.h"
#include "Point.h"


int main() {

  std::vector<Point> points;

  float number_of_rays = 360;
  for (int i = 0; i < number_of_rays; i++){
    points.push_back( Point({150, 150} , {cosf(i/number_of_rays*2*M_PI), sinf(i/number_of_rays*2*M_PI)}, 2) );
  }

  std::vector<Wall> walls;
  walls.push_back( Wall({100, 100}, {700, 100}) );
  walls.push_back( Wall({700, 100}, {700, 500}) );
  walls.push_back( Wall({700, 500}, {500, 500}) );
  walls.push_back( Wall({500, 500}, {500, 400}) );
  walls.push_back( Wall({500, 400}, {300, 400}) );
  walls.push_back( Wall({300, 400}, {300, 500}) );
  walls.push_back( Wall({300, 500}, {100, 500}) );
  walls.push_back( Wall({100, 500}, {100, 100}) );

  InitWindow(800, 600, "Signed Distance");
  SetTargetFPS(60);

  bool run = true;

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    if(IsKeyPressed(KEY_ENTER) and run == false){
      run = true;
    }
    else if(IsKeyPressed(KEY_ENTER) and run == true){
      run = false;
    }


    float dt = GetFrameTime() * 60;
    if (IsKeyDown(KEY_SPACE) or run) {
        for (int i = 0; i < points.size(); ++i) {
            points[i].ResolveCollisions(walls, dt);
        }
    }

    for(int i = 0; i < walls.size(); i++){
      walls[i].Draw();
    }
    for(int i = 0; i < points.size(); i++){
      points[i].Draw();
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}


