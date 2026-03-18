#include<raylib.h>

int main() {

  InitWindow(1600, 900, "Triangle");
  ToggleFullscreen();

  DisableCursor();

  SetMousePosition(0, 0);

  Camera3D camera;
  camera.position = {5, 5, 5};
  camera.up = {0, 0, 1};
  camera.target = {0, 0, 0};
  camera.fovy = 45.f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Using the Right Hand Rule we can see that
  // Curling your fingers in the anti clock wise direction
  // will lead you thumb towards where the normal will be
  // which is the direction from which the triangle can be seen

  Vector3 V1 = { 0.0, 0.0, 0.0};
  Vector3 V2 = { 1.0, 0.0, 0.0};
  Vector3 V3 = { 1.0, 1.0, 0.0};
  Vector3 V4 = { 0.0, 1.0, 0.0};

  Vector3 Vs[4] = { V1, V2, V4, V3 };

  Ray ray;
  ray.position = {0, 0, 0};
  ray.direction = {1, 0, 0};

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    UpdateCamera(&camera, CAMERA_FREE);


    BeginMode3D(camera);

    DrawRay(ray, RED);

    DrawTriangleStrip3D(Vs, 4, RED);

    EndMode3D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
