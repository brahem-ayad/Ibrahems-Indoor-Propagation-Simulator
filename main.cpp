#include"raylib.h"
#include"rlgl.h"
#include"imgui.h"
#include"rlImGui.h"
#include<raymath.h>
#include<vector>

#include"Config.hpp"
#include"Draw_Main_Menu_Bar.hpp"
#include"Grid.hpp"
#include"Cameras.hpp"
#include"Viewport_Gizmo.hpp"
#include"Draw_Tool_Bar.hpp"
#include"Load_Resources.hpp"
#include"Draw_Tool_Options_Bar.hpp"
#include"View_or_Hide_With_Shortcuts.hpp"
#include"Draw_Wall_Placing.hpp"
#include"Unload_Resources.hpp"
#include"Draw_Walls.hpp"
#include"Draw_Door_Placing.hpp"
//#include"Draw_Floor_Placing.hpp"
//#include"Draw_Floors.hpp"
#include"Draw_Doors.hpp"
#include"Draw_Window_Placing.hpp"
#include"Draw_Windows.hpp"
//#include"Draw_Ceiling_Placing.hpp"
//#include"Draw_Ceilings.hpp"
#include"Draw_Transmitter_Placing.hpp"
#include"Draw_Transmitters.hpp"
#include"Draw_The_Run_Simulation_Button.hpp"
#include"Draw_Wall_Collision_Quads_Debug.hpp"
#include"Floor_Plan.hpp"
#include"Draw_Simulation_Options_Bar.hpp"

Color jet(float t) {
    return {
        (unsigned char) (255*Clamp(1.5 - abs(4.0 * t - 3.0), 0.0, 1.0)), // Red
        (unsigned char) (255*Clamp(1.5 - abs(4.0 * t - 2.0), 0.0, 1.0)), // Green
        (unsigned char) (255*Clamp(1.5 - abs(4.0 * t - 1.0), 0.0, 1.0)),  // Blue
        255
    };
}



void Draw_Enhanced_ITU(){
  Color color = PURPLE;

  float T_Power = 10.0;
  float T_Gain = 1.0;
  float R_Gain = 1.0;
  float frequency = 2.4;
  float lambda = 0.299792458/frequency;

  float step_size = 0.2f;
  // going through the cells one by one
  for(float i = -10; i <= 10; i += step_size){
    for(float j = -10; j <= 10; j += step_size){
      float value = -999999.0f;
      for(int k = 0; k < FP::Transmitters.size(); k++){
        Ray R;
        R.position = {i, j, 0.0f};
        R.direction = Vector3Normalize(Vector3Subtract(FP::Transmitters[k].Position, R.position));

        float distance = Vector3Distance({i, j, 0}, FP::Transmitters[k].Position);
        if (distance < 0.1f) distance = 0.1f;

        float L = 20.0f * std::log10(distance) + 20.0f * std::log10(frequency) + 32.44;
        float R_Power = T_Power + T_Gain - L + R_Gain;

        float losses = 0.0f;
        for(int w = 0; w < FP::Wall_Quads_3D.size(); w++){
          RayCollision col = GetRayCollisionQuad(R, FP::Wall_Quads_3D[w].v1, FP::Wall_Quads_3D[w].v2, FP::Wall_Quads_3D[w].v3, FP::Wall_Quads_3D[w].v4);
          if(col.hit and Vector3Distance({i, j, 0.0f}, col.point) < distance){
            if(FP::Wall_Quads_3D[w].Material == Concrete_Wall){
              losses += FP::Wall_Quads_3D[w].Wall_Width*100.0f * 1.5f;
            }
          }
        }
        losses = losses / 2.0f;

        R_Power = R_Power - losses;

        if(R_Power > value) value = R_Power;
      }

      float minDBM = -90.0;
      float maxDBM = -30.0;
      float range = maxDBM - minDBM;

      float normalized = Clamp((value - minDBM) / range, 0.0, 1.0);
      color = jet(normalized);

      DrawCubeV({i, j, 0}, {step_size, step_size, step_size}, color);
    }
  }
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

      //Draw_Floors_2D();

      Draw_Doors_2D();

      Draw_Windows_2D();

      Draw_Walls_2D();

      //Draw_Ceilings_2D();

      Draw_Transmitters_2D();

      EndMode2D();

      if(CONF::Selected_Tool == Walls_Tool) Draw_Wall_Placing();
      else if(CONF::Selected_Tool == Doors_Tool) Draw_Door_Placing();
      //else if(CONF::Selected_Tool == Floors_Tool) Draw_Floor_Placing();
      else if(CONF::Selected_Tool == Windows_Tool) Draw_Window_Placing();
      //else if(CONF::Selected_Tool == Ceilings_Tool) Draw_Ceiling_Placing();
      else if(CONF::Selected_Tool == Transmitter_Tool) Draw_Transmitter_Placing();
    }

    // ------ 3D Mode ------ //
    if(CONF::View == View_3D){
      Update_3D_Camera();
      BeginMode3D(CONF::camera3);
      if(CONF::Show_Grid) Draw_3D_Grid();

      //Draw_Enhanced_ITU();

      //Draw_Floors_3D();

      if(CONF::ITU_Sim_Started){

        // Drawing the visibility polygon
        //for(int i = 0; i < FP::Visibility_Polygons.size(); i++){
        //  for(int j = 0; j < FP::Visibility_Polygons[i].size(); j++){
        //    DrawTriangle3D(FP::Transmitters[i].Position, FP::Visibility_Polygons[i][j], FP::Visibility_Polygons[i][(j+1)%FP::Visibility_Polygons[i].size()], RED);
        //  }
        //}

        BeginShaderMode(CONF::ITU_Shader_3D);

          // Draw the model centered perfectly over your wall coordinates
          DrawModel(CONF::boxModel, CONF::World_Center, 1.0f, YELLOW);

        EndShaderMode();

      }


      if(CONF::View_Walls_Wireframe){
        Draw_Wall_Collision_Quads_Debug(FP::Wall_Quads_3D, RED);
      }
      else{
        Draw_Walls_3D();
      }

      //Draw_Ceilings_3D();

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

    if(CONF::Mode == Simulation_Mode and CONF::View == View_3D){
      Draw_Simulation_Options_Bar();
    }

    ImGui::PopFont();

    rlImGuiEnd();

    EndDrawing();
  }

  // Application exit clean up
  if (CONF::Is_Simulation_Grid_Ready) {
    UnloadModel(CONF::boxModel);
  }

  Unload_Resources();
  rlImGuiShutdown();
  CloseWindow();

  return 0;
}
