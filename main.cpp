#include "raylib.h"
#include "raymath.h"

#include<vector>
#include<math.h>


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");
    ToggleFullscreen();

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 20.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    std::vector<BoundingBox> walls;
    walls.push_back((BoundingBox){{5, 0, -5}, {5.1, 5, 5}});
    walls.push_back((BoundingBox){{-5, 5, -5}, {5, 5.1, 5}});

    Vector3 BS_Position = {0, 1, 0};

    int number_of_rays = 1000;
    std::vector<Ray> rays;

    for (int i = 0; i < number_of_rays; i++) {
      float u = (float)i / number_of_rays;
      float v = (float)(i * 296 % number_of_rays) / number_of_rays;

      float theta = 2.0f * PI * u;
      float z = 2.0f * v - 1.0f;   // cos(phi)
      float r = sqrtf(1.0f - z*z);

      Vector3 dir = {
          r * cosf(theta),
          r * sinf(theta),
          z
      };

      rays.push_back({ BS_Position, Vector3Normalize(dir) });
    }

    float t = 0;
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        //UpdateCamera(&camera, CAMERA_FREE);
        t += 0.01;
        camera.position = {20*cosf(t), 10.0f, 20*sinf(t)};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                for(int j = 0; j < walls.size(); j++){
                  DrawBoundingBox(walls[j], RED);
                }

                DrawSphere(BS_Position, 0.1, BLUE);

                DrawGrid(10, 1.0f);

                for(int i = 0; i < number_of_rays; i++){
                for(int j = 0; j < walls.size(); j++){
                  RayCollision col = GetRayCollisionBox(rays[i], walls[j]);

                  if(col.hit){
                    DrawLine3D(rays[i].position, col.point, BLUE);
                  }
                  else{
                    //DrawRay(rays[i], {255, 0, 0, 10});
                  }
                }}

            EndMode3D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
