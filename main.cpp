#include "raylib.h"
#include "raymath.h"

#include<vector>
#include<math.h>
#include<cmath>
#include<iostream>

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
    walls.push_back((BoundingBox){{-5.1, 0, -5}, {-5, 5, 5}});
    walls.push_back((BoundingBox){{-5, -0.1, -5}, {5, 0, 5}});
    walls.push_back((BoundingBox){{-5, 0, 5}, {5, 5, 5.1}});
    walls.push_back((BoundingBox){{-5, 0, -5.1}, {5, 5, -5}});

    walls.push_back((BoundingBox){{0, 0, 0}, {5, 5, 0.1}});

    Vector3 BS_Position = {4, 3, 4.5};
    int number_of_rays = 1000;
    std::vector<Ray> rays;
    rays.reserve(number_of_rays); // Optimization: prevent multiple reallocations
 
    const float phi = 1.61803398875f; // Golden Ratio
 
    for (int i = 0; i < number_of_rays; i++) {
        // Uniformly distribute Z from 1 to -1
        float z = 1.0f - (i / (float)(number_of_rays - 1)) * 2.0f; 
        float radius = sqrtf(1.0f - z * z);
 
        // Use the golden ratio to increment the angle
        float theta = 2.0f * PI * i / phi; 
 
        Vector3 dir = {
            radius * cosf(theta),
            radius * sinf(theta),
            z
        };
 
        // No need to normalize 'dir' if radius and z are calculated this way
        rays.push_back({ BS_Position, dir });
    }

    float t = 0;
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        //UpdateCamera(&camera, CAMERA_FREE);
        t += 0.005;
        camera.position = {20*cosf(t), 10.0f, 20*sinf(t)};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

                for(int j = 0; j < walls.size(); j++){
                  DrawBoundingBox(walls[j], WHITE);
                }

                DrawSphere(BS_Position, 0.1, RED);

                //DrawGrid(10, 1.0f);

                for(int i = 0; i < number_of_rays; i++){
                  RayCollision col;

                  Vector3 Hit_Position;
                  bool Hit_Trueness = false;
                  float length = 100;
                  Vector3 Normal = {0, 0, 0};

                  for(int j = 0; j < walls.size(); j++){
                    col = GetRayCollisionBox(rays[i], walls[j]);
                    if(col.hit == true and Vector3Distance(BS_Position, col.point) < length ){
                      length = Vector3Distance(BS_Position, col.point);
                      Hit_Position = col.point;
                      Hit_Trueness = true;
                      Normal = col.normal;
                    }
                  }

                  if(Hit_Trueness){
                    Color color = {255*(std::abs(Normal.x)+std::abs(Normal.z)), 255*std::abs(Normal.y), 0, 100};
                    DrawLine3D(BS_Position, Hit_Position, {255, 0, 0, 100} );
                    DrawCubeV(Hit_Position, {0.03, 0.03, 0.03}, color);
                    //DrawPoint3D(Hit_Position, {255, 0, 0, 255});
                  }
                  else{
                    //DrawRay(rays[i], {0, 0, 255, 10});
                  }
                }

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
