#include "raylib.h"
#include <math.h>

int main() {

  InitWindow(800, 600, "Title");
  SetTargetFPS(60);

  Camera3D camera1;
    camera1.position = {0.0f, 5.0f, 10.0f};
    camera1.target = {0.0f, 0.0f, 0.0f};
    camera1.up = {0.0f, 1.0f, 0.0f};
    camera1.fovy = 45.0f;
    camera1.projection = CAMERA_PERSPECTIVE;

  float t = 0;

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

      BeginMode3D(camera1);

      DrawCubeWires({0.0f, 0.0f, 0.0f}, 2.0f, 2.0f, 2.0f, RED);
      DrawLine3D({0,0,0}, {2,2,2}, WHITE);

      EndMode3D();

    EndDrawing();

    camera1.position.x = 10 * sin(t);
    camera1.position.z = 10 * cos(t);
    t += GetFrameTime() * 60 * 0.01;
  }

  CloseWindow();

  return 0;
}
