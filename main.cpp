#include "raylib.h"
#include "raymath.h"

#include<vector>
#include<math.h>
#include<cmath>
#include<iostream>


int frameCount = 0;
bool recording = false;

int main(void)
{
  const int screenWidth = 1600;
  const int screenHeight = 900;

  InitWindow(screenWidth, screenHeight, "Indoor Propagation Simulator");
  ToggleFullscreen();

  float Camera_Height = 8.0f;
  float Camera_Radius = 20.0f;

  Camera3D camera = { 0 };
  camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
  camera.fovy = 20.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  bool top_down_view = false;

  if(top_down_view){
    camera.position = {0, 30, 0};
    camera.up = (Vector3){ 1.0f, 0.0f, 0.0f };
    camera.projection = CAMERA_ORTHOGRAPHIC;
    camera.fovy = 8.0f;
  }

  DisableCursor();

  SetTargetFPS(60);

  std::vector<BoundingBox> walls;
  walls.push_back((BoundingBox){{3, 0, -3}, {3.01, 3, 3}});
  walls.push_back((BoundingBox){{-3, 3, -3}, {3, 3.01, 3}});
  walls.push_back((BoundingBox){{-3.01, 0, -3}, {-3, 3, 3}});
  walls.push_back((BoundingBox){{-3, -0.01, -3}, {3, 0, 3}});
  walls.push_back((BoundingBox){{-3, 0, 3}, {3, 3, 3.01}});
  walls.push_back((BoundingBox){{-3, 0, -3.01}, {3, 3, -3}});

  float number_of_reflections = 10;

  Vector3 BS_Position = {0, 1.5, 0};
  int number_of_rays = 3600;
  std::vector<Ray> rays;
  rays.reserve(number_of_rays * number_of_reflections); // Optimization: prevent multiple reallocations

  std::vector<bool> done;
  done.assign(number_of_rays * number_of_reflections, false);

  std::vector<bool> start;
  start.assign(number_of_rays * number_of_reflections, false);

  std::vector<float> q;
  q.assign(number_of_rays * number_of_reflections, 0.0f);

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
    start[i] = true;
  }

  for(int k = 0; k < number_of_reflections; k++){
  for(int i = number_of_rays*k; i < number_of_rays*(k+1); i++){
    RayCollision col;

    Vector3 Hit_Position;
    bool Hit_Trueness = false;
    float length = 100;
    Vector3 Normal = {0, 0, 0};

    for(int j = 0; j < walls.size(); j++){
      col = GetRayCollisionBox(rays[i], walls[j]);
      float distance = Vector3Distance(rays[i].position, col.point);
      if(col.hit == true and distance < length ){
        length = distance;
        Hit_Position = col.point;
        Hit_Trueness = true;
        Normal = col.normal;
      }
    }

    Vector3 Reflection_Direction = Vector3Normalize(Vector3Reflect(Vector3Normalize(rays[i].direction), Vector3Normalize(Normal))) ;
    rays.push_back( { Vector3Add(Hit_Position, Vector3Scale(Reflection_Direction, 0.01)), Reflection_Direction } );
  }
  }

  bool show_full_path = false;

  Color ray_color = {255, 0, 0, 100};
  float ray_end_size = 0.02;

  float q_speed = 0.02;
  float n = 10;
  float t = 2;
  // Main game loop
  while (!WindowShouldClose())        // Detect window close button or ESC key
  {
    //UpdateCamera(&camera, CAMERA_FREE);
    t += 0.005;
    if(!top_down_view){
      camera.position = {Camera_Radius*cosf(t), Camera_Height, Camera_Radius*sinf(t)};
    }
    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(camera);

    for(int j = 0; j < walls.size(); j++){
      DrawBoundingBox(walls[j], WHITE);
    }

    for(int i = 0; i < number_of_rays*n; i++){
      if(done[i] == false && start[i] == true or show_full_path){
        RayCollision col;

        Vector3 Hit_Position;
        bool Hit_Trueness = false;
        float length = 100;
        Vector3 Normal = {0, 0, 0};

        for(int j = 0; j < walls.size(); j++){
          col = GetRayCollisionBox(rays[i], walls[j]);
          float distance = Vector3Distance(rays[i].position, col.point);
          if(col.hit == true and distance < length ){
            length = distance;
            Hit_Position = col.point;
            Hit_Trueness = true;
            Normal = col.normal;
          }
        }
        if(i < number_of_rays){
          q[i] += q_speed;
        }
        else if( i > number_of_rays and done[i - number_of_rays]){
          q[i] += q_speed;
        }

        if(Hit_Trueness){
          Color color = {(unsigned char)(255*(std::abs(Normal.x)+std::abs(Normal.z))), 0, (unsigned char)(255*std::abs(Normal.y)), 150};
          float Length = Vector3Length(Vector3Subtract(Hit_Position, rays[i].position));
          Vector3 Interpolated_End_Pos = Vector3Add(rays[i].position, Vector3Scale( Vector3Normalize( Vector3Subtract(Hit_Position, rays[i].position) ) , q[i] ));
          float Segment_Length = 0.1;
          Vector3 Interpolated_Start_Pos = rays[i].position;
          if (Segment_Length < q[i]){
            Interpolated_Start_Pos = Vector3Add(rays[i].position, Vector3Scale( Vector3Normalize( Vector3Subtract(Hit_Position, rays[i].position) ) , q[i] - Segment_Length ));
          }
          if(show_full_path){
            DrawLine3D(rays[i].position, Hit_Position, {255, 0, 0, 100} );
            DrawSphereEx(Hit_Position, 0.015, 3, 3, color);
          }
          else{
            if(i < number_of_rays){
              if(q[i] < Length){
                DrawLine3D(Interpolated_Start_Pos, Interpolated_End_Pos, ray_color);
                DrawSphereEx(Interpolated_End_Pos, ray_end_size, 3, 3, ray_color);
              }
              else if(q[i] < Segment_Length + Length){
                DrawLine3D(Interpolated_Start_Pos, Hit_Position, ray_color);
                DrawSphereEx(Hit_Position, ray_end_size, 3, 3, color);
              }
              else {
                done[i] = true;
                start[i + number_of_rays] = true;
                //DrawSphereEx(Hit_Position, 0.015, 3, 3, color);
              }
            }
            else{
              if(done[i - number_of_rays]){
                if(q[i] < Length){
                  DrawLine3D(Interpolated_Start_Pos, Interpolated_End_Pos, ray_color);
                  DrawSphereEx(Interpolated_End_Pos, ray_end_size, 3, 3, ray_color);
                }
                else if(q[i] < Segment_Length + Length){
                  DrawLine3D(Interpolated_Start_Pos, Hit_Position, ray_color);
                  DrawSphereEx(Hit_Position, ray_end_size, 3, 3, color);
                }
                else {
                  done[i] = true;
                  if(i < number_of_rays*(number_of_reflections-1) ){
                    start[i + number_of_rays] = true;
                  }
                  //DrawSphereEx(Hit_Position, 0.015, 3, 3, color);
                }
              }
            }
          }
        }
        else{
          //DrawRay(rays[i], {0, 0, 255, 10});
        }
      }
    }

    EndMode3D();

    if (IsKeyPressed(KEY_UP))
    {
        if (n < number_of_reflections) n++;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        if (n > 0) n--;
    }

    EndDrawing();

    if(recording){
      char filename[128];
      sprintf(filename, "output/frame%05d.png", frameCount);

      Image img = LoadImageFromScreen();
      ExportImage(img, filename);
      UnloadImage(img);

      frameCount++;
    }
  }

  CloseWindow();

  return 0;
}
