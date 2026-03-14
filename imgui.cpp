#include<raylib.h>
#include<imgui.h>

#include"./include/external/rlImGui.h"

int main() {

  InitWindow(1600, 900, "ImGui Testing");
  ToggleFullscreen();

  rlImGuiSetup(false);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    rlImGuiBegin();

    //if(ImGui::BeginMainMenuBar()){
    //  if(ImGui::BeginMenu("File")){

    //  }
    //  ImGui::EndMainMenuBar();
    //}

    ImGui::ShowDemoWindow();

    rlImGuiEnd();

    EndDrawing();
  }

  rlImGuiShutdown();

  CloseWindow();

  return 0;
}
