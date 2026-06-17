#include"../include/Run_Simulation.hpp"

#include<raylib.h>
#include<rlgl.h>
#include<vector>
#include"../include/Generate_Wall_Collision_Quads.hpp"
#include"../include/Config.hpp"
#include"../include/Floor_Plan.hpp"
#include"../include/Generate_Visibility_Polygon.hpp"
#include"../include/Calculate_World_Boundaries.hpp"

void Bake_Visibility_Texture_Grid(){
  // Clean up old allocations from VRAM before spawning the new ones
  if (CONF::Is_Simulation_Grid_Ready) {
    UnloadModel(CONF::boxModel);
  }

  Calculate_World_Boundaries();

  CONF::Visibility_Polygon_Camera.target = CONF::World_Center;
  CONF::Visibility_Polygon_Camera.position = {CONF::World_Center.x, CONF::World_Center.y, 10.f};
  CONF::Visibility_Polygon_Camera.fovy = CONF::World_Height;

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
      BeginMode3D(CONF::Visibility_Polygon_Camera);

        rlMatrixMode(RL_PROJECTION);
        rlLoadIdentity();
        rlOrtho(-CONF::World_Width/2.0f, CONF::World_Width/2.0f, -CONF::World_Height/2.0f, CONF::World_Height/2.0f, 0.0f, 20.0f);
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

  CONF::boxMesh = GenMeshCube(CONF::World_Width, CONF::World_Height, 0.1f);
  CONF::boxModel = LoadModelFromMesh(CONF::boxMesh);

  CONF::boxModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = masterGrid.texture;
  CONF::boxModel.materials[0].shader = CONF::ITU_Shader_3D;


  CONF::Is_Simulation_Grid_Ready = true;
}

void Run_Simulation() {

  FP::Wall_Quads_3D.clear();
  FP::Wall_Quads_3D = Generate_Wall_Collision_Quads();

  Generate_Visibility_Polygon();

  CONF::activeCount = FP::Transmitters.size();
  for (int i = 0; i < CONF::activeCount; i++) {
    CONF::shaderPositions[i] = FP::Transmitters[i].Position;
  }
 
  CONF::ITU_Sim_Started = true;
 
  SetShaderValue(CONF::ITU_Shader_3D, CONF::ITU_Shader_3D_Count_ID, &CONF::activeCount, SHADER_UNIFORM_INT);
  SetShaderValueV(CONF::ITU_Shader_3D, CONF::ITU_Shader_3D_Positions_ID, CONF::shaderPositions, SHADER_UNIFORM_VEC3, CONF::activeCount);

  Bake_Visibility_Texture_Grid();
}

