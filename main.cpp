#include "Floor_Plan.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "imgui.h"
#include "rlImGui.h"

#include "Config.hpp"
#include "Draw_Main_Menu_Bar.hpp"
#include "Grid.hpp"
#include "Cameras.hpp"
#include "Viewport_Gizmo.hpp"
#include "Draw_Tool_Bar.hpp"
#include "Load_Resources.hpp"
#include "Draw_Tool_Options_Bar.hpp"
#include "View_or_Hide_With_Shortcuts.hpp"
#include "Draw_Wall_Placing.hpp"
#include "Unload_Resources.hpp"
#include "Draw_Walls.hpp"
#include "Draw_Door_Placing.hpp"
#include "Draw_Floor_Placing.hpp"
#include "Draw_Floors.hpp"
#include "Draw_Doors.hpp"
#include "Draw_Window_Placing.hpp"
#include "Draw_Windows.hpp"
#include "Draw_Ceiling_Placing.hpp"
#include "Draw_Ceilings.hpp"
#include "Draw_Transmitter_Placing.hpp"
#include "Draw_Transmitters.hpp"
#include "Draw_The_Run_Simulation_Button.hpp"
#include "Draw_Wall_Collision_Quads_Debug.hpp"

void CalculateWorldBoundaries(Vector3& outCenter, float& outWorldWidth, float& outWorldHeight) {
  // 1. Initialize bounds with extreme opposites
  float minX =  999999.0f;
  float maxX = -999999.0f;
  float minY =  999999.0f;
  float maxY = -999999.0f;

  // Handle an empty vector edge-case safely
  if (FP::Walls_Vec.empty()) {
    outCenter = (Vector3){ 0.0f, 0.0f, 0.0f };
    outWorldWidth = 50.0f;   // Fallback defaults
    outWorldHeight = 25.0f;
    return;
  }

  // 2. Loop through every wall and check both its start and end coordinates
  for (const auto& wall : FP::Walls_Vec) {
    // --- Check Wall Start Point ---
    if (wall.Wall_Start_3D.x < minX) minX = wall.Wall_Start_3D.x;
    if (wall.Wall_Start_3D.x > maxX) maxX = wall.Wall_Start_3D.x;
    if (wall.Wall_Start_3D.y < minY) minY = wall.Wall_Start_3D.y;
    if (wall.Wall_Start_3D.y > maxY) maxY = wall.Wall_Start_3D.y;

    // --- Check Wall End Point ---
    if (wall.Wall_End_3D.x < minX) minX = wall.Wall_End_3D.x;
    if (wall.Wall_End_3D.x > maxX) maxX = wall.Wall_End_3D.x;
    if (wall.Wall_End_3D.y < minY) minY = wall.Wall_End_3D.y;
    if (wall.Wall_End_3D.y > maxY) maxY = wall.Wall_End_3D.y;
  }

  // 3. Calculate total width and height of the world
  outWorldWidth = maxX - minX;
  outWorldHeight = maxY - minY;

  // 4. Add a tiny padding margin (e.g., 5%) so your outer walls 
  // aren't hugging the absolute edge of the baked texture pixels.
  float padding = 2.0f; 
  outWorldWidth += padding * 2.0f;
  outWorldHeight += padding * 2.0f;

  // Recalculate minimums with padding taken into account
  minX -= padding;
  minY -= padding;

  // 5. Calculate the exact central point
  outCenter.x = minX + (outWorldWidth / 2.0f);
  outCenter.y = minY + (outWorldHeight / 2.0f);
  outCenter.z = 0.0f; // Kept at zero since we map on a flat plane
}

int main(){
  int screenWidth = 1600;
  int screenHeight = 900;

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "Crimson");
  SetTargetFPS(144);
  rlImGuiSetup(false);

  ImGuiIO& io = ImGui::GetIO();
  ImFont* myFont = io.Fonts->AddFontFromFileTTF("./Resources/Fonts/Montserrat-Bold.ttf", 16.0f);
  io.Fonts->Build();

  Set_Cameras();
 
  Load_Resources();

  SetExitKey(KEY_NULL);

  Camera3D Visibility_Polygon_Camera;
  Visibility_Polygon_Camera.projection = CAMERA_ORTHOGRAPHIC;
  Visibility_Polygon_Camera.target = {0, 0, 0};
  Visibility_Polygon_Camera.position = {0, 0, 10};
  Visibility_Polygon_Camera.up = {0, 1, 0};
  Visibility_Polygon_Camera.fovy = 10;

  Mesh boxMesh;
  Model boxModel;
  
  // Track allocation status to cleanly swap VRAM layers on keypress
  bool isSimulationGridReady = false;

  while(!WindowShouldClose()){
 
    BeginDrawing();
    ClearBackground(RAYWHITE);

    View_or_Hide_With_Shortcuts();

    // ------- 2D Mode ------ //
    if(CONF::View == View_2D){
      Update_2D_Camera();

      BeginMode2D(CONF::camera2);

      // Grid
      if(CONF::Show_Grid) Draw_2D_Grid();

      Draw_Floors_2D();

      Draw_Doors_2D();

      Draw_Windows_2D();

      Draw_Walls_2D();

      Draw_Ceilings_2D();

      Draw_Transmitters_2D();

      EndMode2D();

      if(CONF::Selected_Tool == Walls_Tool) Draw_Wall_Placing();
      else if(CONF::Selected_Tool == Doors_Tool) Draw_Door_Placing();
      else if(CONF::Selected_Tool == Floors_Tool) Draw_Floor_Placing();
      else if(CONF::Selected_Tool == Windows_Tool) Draw_Window_Placing();
      else if(CONF::Selected_Tool == Ceilings_Tool) Draw_Ceiling_Placing();
      else if(CONF::Selected_Tool == Transmitter_Tool) Draw_Transmitter_Placing();
    }

    // ------ 3D Mode ------ //
    if(CONF::View == View_3D){
      Update_3D_Camera();
      BeginMode3D(CONF::camera3);
      if(CONF::Show_Grid) Draw_3D_Grid();

      Draw_Floors_3D();

      if(CONF::ITU_Sim_Started){

        // Drawing the visibility polygon
        //for(int i = 0; i < FP::Visibility_Polygons.size(); i++){
        //  for(int j = 0; j < FP::Visibility_Polygons[i].size(); j++){
        //    DrawTriangle3D(FP::Transmitters[i].Position, FP::Visibility_Polygons[i][j], FP::Visibility_Polygons[i][(j+1)%FP::Visibility_Polygons[i].size()], RED);
        //  }
        //}

        Vector3 worldCenter;
        float worldWidth;
        float worldHeight;

        if(IsKeyPressed(KEY_P)){
          
          // Clean up old allocations from VRAM before spawning the new ones
          if (isSimulationGridReady) {
            UnloadModel(boxModel);
          }

          CalculateWorldBoundaries(worldCenter, worldWidth, worldHeight);

          Visibility_Polygon_Camera.target = worldCenter;
          Visibility_Polygon_Camera.position = {worldCenter.x, worldCenter.y, 10.f};
          Visibility_Polygon_Camera.fovy = worldHeight;

          int layerHeight = 256;
          int layerWidth = 256;

          // Test Start

          // 2. Setup the massive 8x4 master texture grid (8 columns, 4 rows)
          int columns = 8;
          int rows = 4;
          RenderTexture2D masterGrid = LoadRenderTexture(layerWidth * columns, layerHeight * rows);

          // 3. Begin drawing onto the massive master canvas
          BeginTextureMode(masterGrid);
              
            // Clear the entire sheet to pure black (total occlusion)
            ClearBackground(BLACK);

            // 4. Loop through every transmitter (Up to 32 max)
            for (int i = 0; i < FP::Visibility_Polygons.size(); i++) {
                
              // Calculate which cell column and row this transmitter belongs to
              int col = i % columns;
              int row = i / columns;

              // Find the pixel offsets for this specific cell slot
              int xOffset = col * layerWidth;
              int yOffset = row * layerHeight;

              // CRITICAL: Force the GPU to squeeze the camera's view into just this sub-cell
              rlViewport(xOffset, yOffset, layerWidth, layerHeight);

              // Turn on your static orthographic world camera
              BeginMode3D(Visibility_Polygon_Camera);
                  
                rlMatrixMode(RL_PROJECTION);
                rlLoadIdentity();
                rlOrtho(-worldWidth/2.0f, worldWidth/2.0f, -worldHeight/2.0f, worldHeight/2.0f, 0.0f, 20.0f);
                rlMatrixMode(RL_MODELVIEW);
                
                // Loop through and draw the triangles for transmitter 'i'
                for (int j = 0; j < FP::Visibility_Polygons[i].size(); j++) {
                  DrawTriangle3D(
                    FP::Transmitters[i].Position, 
                    FP::Visibility_Polygons[i][j], 
                    FP::Visibility_Polygons[i][(j + 1) % FP::Visibility_Polygons[i].size()], 
                    WHITE
                  );
                }

              EndMode3D();
            }

            // 5. CRITICAL RESET: Put the viewport back to full size so raylib doesn't break
            rlViewport(0, 0, masterGrid.texture.width, masterGrid.texture.height);

          EndTextureMode();

          // 6. Export to verify your 8x4 layout looks correct!
          Image img = LoadImageFromTexture(masterGrid.texture);
          ExportImage(img, "test.png");
          UnloadImage(img);

          boxMesh = GenMeshCube(worldWidth, worldHeight, 0.1f);
          boxModel = LoadModelFromMesh(boxMesh);

          boxModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = masterGrid.texture;
          boxModel.materials[0].shader = CONF::ITU_LOS_Shader_3D;

          // Test End

          isSimulationGridReady = true;

          //RenderTexture2D target = LoadRenderTexture(100, 100);
          //BeginTextureMode(target);
          //ClearBackground(BLACK);
          //BeginMode3D(Visibility_Polygon_Camera);
          //for(int i = 0; i < FP::Visibility_Polygons.size(); i++){
          //  for(int j = 0; j < FP::Visibility_Polygons[i].size(); j++){
          //    DrawTriangle3D(FP::Transmitters[i].Position, FP::Visibility_Polygons[i][j], FP::Visibility_Polygons[i][(j+1)%FP::Visibility_Polygons[i].size()], WHITE);
          //  }
          //}
          //EndMode3D();
          //EndTextureMode();
          //Image img = LoadImageFromTexture(target.texture);
          //ExportImage(img, "test.png");
        }

        BeginShaderMode(CONF::ITU_LOS_Shader_3D);

          // Draw the model centered perfectly over your wall coordinates
          DrawModel(boxModel, worldCenter, 1.0f, YELLOW);

        EndShaderMode();

        //BeginShaderMode(CONF::ITU_NLOS_Shader_3D);
        //DrawCubeV({0, 0, 0}, {100, 100, 0.02}, RED);
        //EndShaderMode();
      }

      if(CONF::View_Walls_Wireframe){
        Draw_Wall_Collision_Quads_Debug(CONF::Wall_Quads_3D, RED);
      }
      else{
        Draw_Walls_3D();
      }

      Draw_Ceilings_3D();

      Draw_Transmitters_3D();

      EndMode3D();
    }

    if(CONF::Show_Viewport_Gizmo) Draw_Viewport_Gizmo();

    if(CONF::Mode == Drawing_Mode and CONF::View == View_2D and CONF::Show_Tools_Bar){
      Draw_Tool_Bar();
    }
    else if(CONF::Mode == Simulation_Mode){
      Draw_The_Run_Simulation_Button();
    }

    rlImGuiBegin();

    bool open = true;

    ImGui::PushFont(myFont);

    Draw_ImGui_Main_Menu_Bar();

    if(CONF::Mode == Drawing_Mode and CONF::View == View_2D and CONF::Show_Tools_Bar and CONF::Show_Tool_Options_Bar){
      Draw_Tool_Options_Bar();
    }

    ImGui::PopFont();

    rlImGuiEnd();

    EndDrawing();
  }

  // Application exit clean up
  if (isSimulationGridReady) {
    UnloadModel(boxModel);
  }

  Unload_Resources();
  rlImGuiShutdown();
  CloseWindow();

  return 0;
}
