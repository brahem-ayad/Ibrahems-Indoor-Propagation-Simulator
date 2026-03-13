#include<vector>

#include<raylib.h>
#include<raymath.h>
#include<imgui.h>

#include "include/config.hpp"

#include"./include/external/rlImGui.h"

void DrawImGui() {
    rlImGuiBegin();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Create New Empty File", "Ctrl+N")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    rlImGuiEnd();
}

class Cube{
  public:
    Vector3 Position;
    Vector3 Size;
};

void Draw_Grid(int slices, float spacing, float size_multiplier, bool isMode2D) {
  Color grid_lines_color;
  Color main_lines_color;

  if(conf::IsDarkMode){
    grid_lines_color = {50, 50, 50, 255};
    main_lines_color = {100, 100, 100, 255};
  }
  else{
    grid_lines_color = {150, 150, 150, 255};
    main_lines_color = {100, 100, 100, 255};
  }

  if(isMode2D){
    for(int i = -slices/2; i <= slices/2; i++){
      DrawLine(i*size_multiplier*spacing, -(float)slices/2*size_multiplier*spacing, i*size_multiplier*spacing, (float)slices/2*size_multiplier*spacing, grid_lines_color);
      DrawLine(-(float)slices/2*size_multiplier*spacing, i*size_multiplier*spacing, (float)slices/2*size_multiplier*spacing, i*size_multiplier*spacing, grid_lines_color);
    }
    DrawLine(0, -(float)slices/2*size_multiplier*spacing, 0, (float)slices/2*size_multiplier*spacing, main_lines_color);
    DrawLine(-(float)slices/2*size_multiplier*spacing, 0, (float)slices/2*size_multiplier*spacing, 0, main_lines_color);
  }
  else{
    for(int i = -slices/2; i <= slices/2; i++){
      DrawLine3D({i*spacing, -(float)slices/2*spacing, 0}, {i*spacing, (float)slices/2*spacing, 0}, grid_lines_color);
      DrawLine3D({-(float)slices/2*spacing, i*spacing, 0}, {(float)slices/2*spacing, i*spacing, 0}, grid_lines_color);
    }
    DrawLine3D({0, -(float)slices/2*spacing, 0}, {0, (float)slices/2*spacing, 0}, main_lines_color);
    DrawLine3D({-(float)slices/2*spacing, 0, 0}, {(float)slices/2*spacing, 0, 0}, main_lines_color);
  }
}

void InitCameras(Camera3D &camera_3d, Camera2D &camera_2d){
  camera_3d.position = {10, 10, 10};
  camera_3d.up = {0, 0, 1};
  camera_3d.fovy = 45.0f;
  camera_3d.target = {0, 0, 0};
  camera_3d.projection = CAMERA_PERSPECTIVE;

  camera_2d.target = {0, 0};
  camera_2d.rotation = 0.0f;
  camera_2d.zoom = 1.0f;
  camera_2d.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
}

int main() {

  InitWindow(1600, 900, "An Indoor EM Waves Propagation Simulator");
  ToggleFullscreen();
  SetTargetFPS(60);

  rlImGuiSetup(conf::IsDarkMode);

  Camera3D camera_3d;
  Camera2D camera_2d;
  InitCameras(camera_3d, camera_2d);

  std::vector<Cube> walls;

  bool isMode2D = true;

  float size_multiplier = 50;

  bool temp_is_drawing = false;
  Vector2 temp_start_pos;
  Vector2 temp_end_pos;
  bool is_end_pos_available = false;
  bool allow_end_pos_placement = false;
  bool allow_end_pos_movement = false;
  bool allow_start_pos_movement = false;

  while(!WindowShouldClose()){
    BeginDrawing();

    // Backggound Color
    if(conf::IsDarkMode) ClearBackground(BLACK);
    else ClearBackground(RAYWHITE);

    if(isMode2D){
      BeginMode2D(camera_2d);

      Draw_Grid(10, 1, size_multiplier, isMode2D);

      for(int i = 0; i < walls.size(); i++){
        DrawRectangle(size_multiplier*(walls[i].Position.x - walls[i].Size.x/2), size_multiplier*(walls[i].Position.y - walls[i].Size.y/2), size_multiplier*walls[i].Size.x, size_multiplier*walls[i].Size.y, RED);
      }

      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and temp_is_drawing == false){
        temp_start_pos = GetMousePosition();
        temp_is_drawing = true;
      }
      if(temp_is_drawing){

        Vector2 size;
        Vector2 starting_pos;

        if(!is_end_pos_available){
          if(GetMousePosition().x > temp_start_pos.x and GetMousePosition().y > temp_start_pos.y){
            size = {GetMousePosition().x - temp_start_pos.x, GetMousePosition().y - temp_start_pos.y};
            DrawRectangleV(GetScreenToWorld2D(temp_start_pos, camera_2d), size, BLUE);
          }
          else if (GetMousePosition().x > temp_start_pos.x and GetMousePosition().y < temp_start_pos.y){
            size = {GetMousePosition().x - temp_start_pos.x, temp_start_pos.y - GetMousePosition().y};
            starting_pos = {temp_start_pos.x, GetMousePosition().y};
            DrawRectangleV(GetScreenToWorld2D(starting_pos, camera_2d), size, BLUE);
          }
          else if (GetMousePosition().x < temp_start_pos.x and GetMousePosition().y > temp_start_pos.y){
            size = {temp_start_pos.x - GetMousePosition().x, GetMousePosition().y - temp_start_pos.y};
            starting_pos = {GetMousePosition().x, temp_start_pos.y};
            DrawRectangleV(GetScreenToWorld2D(starting_pos, camera_2d), size, BLUE);
          }
          else if (GetMousePosition().x < temp_start_pos.x and GetMousePosition().y < temp_start_pos.y){
            size = {temp_start_pos.x - GetMousePosition().x, temp_start_pos.y - GetMousePosition().y};
            DrawRectangleV(GetScreenToWorld2D(GetMousePosition(), camera_2d), size, BLUE);
          }

          if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            allow_end_pos_placement = true;
          }

          if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and allow_end_pos_placement){
          temp_end_pos = GetMousePosition();
          is_end_pos_available = true;
          }
          DrawCircleV(GetScreenToWorld2D(temp_start_pos, camera_2d), 5, BLACK);
          DrawCircleV(GetScreenToWorld2D(GetMousePosition(), camera_2d), 5, BLACK);
        }
        else if(is_end_pos_available == true){
          if(temp_end_pos.x > temp_start_pos.x and temp_end_pos.y > temp_start_pos.y){
            size = {temp_end_pos.x - temp_start_pos.x, temp_end_pos.y - temp_start_pos.y};
            DrawRectangleV(GetScreenToWorld2D(temp_start_pos, camera_2d), size, BLUE);
          }
          else if (temp_end_pos.x > temp_start_pos.x and temp_end_pos.y < temp_start_pos.y){
            size = {temp_end_pos.x - temp_start_pos.x, temp_start_pos.y - temp_end_pos.y};
            starting_pos = {temp_start_pos.x, temp_end_pos.y};
            DrawRectangleV(GetScreenToWorld2D(starting_pos, camera_2d), size, BLUE);
          }
          else if (temp_end_pos.x < temp_start_pos.x and temp_end_pos.y > temp_start_pos.y){
            size = {temp_start_pos.x - temp_end_pos.x, temp_end_pos.y - temp_start_pos.y};
            starting_pos = {temp_end_pos.x, temp_start_pos.y};
            DrawRectangleV(GetScreenToWorld2D(starting_pos, camera_2d), size, BLUE);
          }
          else if (temp_end_pos.x < temp_start_pos.x and temp_end_pos.y < temp_start_pos.y){
            size = {temp_start_pos.x - temp_end_pos.x, temp_start_pos.y - temp_end_pos.y};
            DrawRectangleV(GetScreenToWorld2D(temp_end_pos, camera_2d), size, BLUE);
          }
          DrawCircleV(GetScreenToWorld2D(temp_start_pos, camera_2d), 5, BLACK);
          DrawCircleV(GetScreenToWorld2D(temp_end_pos, camera_2d), 5, BLACK);

          if(CheckCollisionPointCircle(GetMousePosition(), temp_end_pos, 5)){
            DrawCircleV(GetScreenToWorld2D(temp_end_pos, camera_2d), 5, BLUE);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
              allow_end_pos_movement = true;
            }
          }
          if(allow_end_pos_movement){
            temp_end_pos = GetMousePosition();
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
              allow_end_pos_movement = false;
            }
          }

          if(CheckCollisionPointCircle(GetMousePosition(), temp_start_pos, 5)){
            DrawCircleV(GetScreenToWorld2D(temp_start_pos, camera_2d), 5, BLUE);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
              allow_start_pos_movement = true;
            }
          }
          if(allow_start_pos_movement){
            temp_start_pos = GetMousePosition();
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
              allow_start_pos_movement = false;
            }
          }

          if(IsKeyPressed(KEY_ENTER)){
            Cube wall;
            wall.Size = {size.x/size_multiplier, size.y/size_multiplier, 5};
            wall.Position = {temp_start_pos.x/size_multiplier - (float)GetScreenWidth()/2/size_multiplier + wall.Size.x/2, temp_start_pos.y/size_multiplier - (float)GetScreenHeight()/2/size_multiplier + wall.Size.y/2, wall.Size.z/2};
            walls.push_back( wall );
          }
        }
      }

      EndMode2D();
    }
    else {
      BeginMode3D(camera_3d);

      Draw_Grid(10, 1, size_multiplier, isMode2D);

      for(int i = 0; i < walls.size(); i++){
        DrawCubeV(walls[i].Position, walls[i].Size, RED);
        DrawCubeWiresV(walls[i].Position, walls[i].Size, MAROON);
      }

      EndMode3D();
    }

    DrawImGui();

    if(IsKeyPressed(KEY_TWO)) isMode2D = true;
    if(IsKeyPressed(KEY_THREE)) isMode2D = false;

    EndDrawing();
  }

  rlImGuiShutdown();

  CloseWindow();

  return 0;
}
