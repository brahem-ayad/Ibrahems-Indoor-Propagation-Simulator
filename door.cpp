#include<vector>

#include<raylib.h>
#include<raymath.h>

#include"./include/Soild_Bounding_Box.hpp"
#include"./include/Grid.hpp"

void SplitWall(std::vector<BoundingBox> &Walls, Camera3D camera, float Door_Height, float Door_Width) {

  Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);

  // Notes :
  // - Rotated Walls are not supported
  // - There is nothing to detect the closest wall, because I assume that the feuture is going to be used
  //   in 2D, from the top. It is now 3D just for me while coding it.

  for(int i = 0; i < Walls.size(); i++){
    if(GetRayCollisionBox(ray, Walls[i]).hit){
        DrawSolidBoundingBox(Walls[i], BLUE);
 
        Vector3 P = GetRayCollisionBox(ray, Walls[i]).point;
        Vector3 N;

        // Checking which side should the door be place on.
        if(Walls[i].max.x - Walls[i].min.x > Walls[i].max.y - Walls[i].min.y){
          N = {0, 1, 0};
        }
        else if(Walls[i].max.x - Walls[i].min.x < Walls[i].max.y - Walls[i].min.y){
          N = {1, 0, 0};
        }
        else{
          N = GetRayCollisionBox(ray, Walls[i]).normal;
        }

        Vector3 Pos;
        float depth;
 
        if(N.x == 1){
          Pos = {Walls[i].min.x + (Walls[i].max.x - Walls[i].min.x)/2, P.y, Door_Height/2};
          depth = Walls[i].max.x - Walls[i].min.x + 0.1;
          if(Pos.y - Walls[i].min.y > Door_Width/2 and Pos.y < Walls[i].max.y - Door_Width/2){
            DrawCube(Pos, depth, Door_Width, Door_Height, WHITE);
            DrawCubeWires(Pos, depth, Door_Width, Door_Height, BLACK);
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
              BoundingBox B1 = { Walls[i].min, {Walls[i].max.x, Pos.y - Door_Width/2, Walls[i].max.z} };
              BoundingBox B2 = { {Walls[i].min.x, Pos.y - Door_Width/2, Door_Height}, {Walls[i].max.x, Pos.y + Door_Width/2, Walls[i].max.z} };

              BoundingBox B3 = { {Walls[i].min.x, Pos.y + Door_Width/2, Walls[i].min.z}, Walls[i].max };

              DrawBoundingBox(B1, BLACK);
              DrawBoundingBox(B2, BLACK);
              DrawBoundingBox(B3, BLACK);

              // I have these three bounding boxes
              // I just need to remove the existing wall from the vector
              // and replace it with these three

              Walls.erase(Walls.begin() + i);
              Walls.push_back(B1);
              Walls.push_back(B2);
              Walls.push_back(B3);
              break;
            }
          }
          else{
            DrawCubeWires(Pos, depth, Door_Width, Door_Height, BLACK);
          }
        }
        else if(N.y == 1){
          Pos = {P.x, Walls[i].min.y + (Walls[i].max.y - Walls[i].min.y)/2, Door_Height/2};
          depth = Walls[i].max.y - Walls[i].min.y + 0.1;
          if(Pos.x - Walls[i].min.x > Door_Width/2 and Pos.x < Walls[i].max.x - Door_Width/2){
            DrawCube(Pos, Door_Width, depth, Door_Height, WHITE);
            DrawCubeWires(Pos, Door_Width, depth, Door_Height, BLACK);
          }
          else{
            DrawCubeWires(Pos, Door_Width, depth, Door_Height, BLACK);
          }
        }
 
 
      }
    else{
      DrawSolidBoundingBox(Walls[i], RED);
    }

    DrawBoundingBox(Walls[i], BLACK);
  }
}

int main() {

  InitWindow(1600, 900, "Door");
  ToggleFullscreen();

  Camera3D camera;
  camera.target = {0, 0, 1.5};
  camera.up = {0, 0, 1};
  camera.position = {8, 0, 5};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  BoundingBox B = {{-0.1, -1.5, 0}, {0.1, 1.5, 3}};

  std::vector<BoundingBox> Walls;
  Walls.push_back(B);

  bool Door_Mode = true;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    BeginMode3D(camera);

    DrawGrid3D(6, 0.5, GRAY);

    if(Door_Mode){
      SplitWall(Walls, camera, 2, 1);
    }


    EndMode3D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
