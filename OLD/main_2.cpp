#include<raylib.h>
#include"raymath.h"
#include<math.h>
#include<vector>

#include"config.hpp"
#include"grid.hpp"

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT); // to add antialiasing
  InitWindow(conf::screen_width, conf::screen_height, "Indoor Wave Propagation Simulator");
  ToggleFullscreen();
  SetTargetFPS(60);

  Camera2D camera2d = {0};
  camera2d.zoom = 1.0f;

  Camera3D camera3d = {0};
  camera3d.position = (Vector3){ 100.0f, 100.0f, 100.0f };
  camera3d.target = (Vector3){ 0.0f, 0.0f, 0.0f };
  camera3d.up = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera3d.fovy = 45.0f;
  camera3d.projection = CAMERA_PERSPECTIVE;

  Vector3 BS_Position = {800, 450, 10};

  int number_of_rays = 760;

  std::vector<Ray> rays;
  for(int i = 0; i < number_of_rays; i++){
    Vector3 dir = {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays), 0};
    rays.push_back((Ray){BS_Position, Vector3Normalize(dir)});
  }

  BoundingBox Wall;
  Wall.min = {1200, 100, 0};
  Wall.max = {1250, 800, 100};

  while(!WindowShouldClose()){

    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera2d.zoom);
        camera2d.target = Vector2Add(camera2d.target, delta);
    }

     // Zoom based on mouse wheel
     float wheel = GetMouseWheelMove();
     if (wheel != 0)
     {
         // Get the world point that is under the mouse
         Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera2d);

         // Set the offset to where the mouse is
         camera2d.offset = GetMousePosition();

         // Set the target to match, so that the camera2d maps the world space point
         // under the cursor to the screen space point under the cursor at any zoom
         camera2d.target = mouseWorldPos;

         // Zoom increment
         // Uses log scaling to provide consistent zoom speed
         float scale = 0.2f*wheel;
         camera2d.zoom = Clamp(expf(logf(camera2d.zoom)+scale), 0.125f, 64.0f);
     }


    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera2d);

    Draw_Grid(camera2d.target, camera2d.offset, camera2d.zoom);

    DrawBoundingBox(Wall, WHITE);

    for(int i = 0; i < number_of_rays; i++){
      rays[i].position.x = GetScreenToWorld2D(GetMousePosition(), camera2d).x;
      rays[i].position.y = GetScreenToWorld2D(GetMousePosition(), camera2d).y;

      RayCollision col = GetRayCollisionBox(rays[i], Wall);
      if(col.hit){
        DrawLine(rays[i].position.x, rays[i].position.y, col.point.x, col.point.y, RED);
      }
      else{
        DrawLine(rays[i].position.x, rays[i].position.y, rays[i].position.x+10000*rays[i].direction.x, rays[i].position.y+10000*rays[i].direction.y, RED);
      }
    }

    EndMode2D();


    EndDrawing();
  }

  CloseWindow();
  return 0;
}
