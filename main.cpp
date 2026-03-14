#include<raylib.h>

int main() {

  InitWindow(1600, 900, "Title");
  ToggleFullscreen();

  Camera3D camera_3d;
  camera_3d.position = {10, 10, 10};
  camera_3d.fovy = 45.f;
  camera_3d.projection = CAMERA_PERSPECTIVE;
  camera_3d.target = {0, 0, 0};
  camera_3d.up = {0, 0, 1};

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(WHITE);


    UpdateCamera(&camera_3d, CAMERA_ORBITAL);

    BeginMode3D(camera_3d);

      DrawCubeWires({0, 0, 0}, 2, 2, 2, BLACK);

    EndMode3D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

