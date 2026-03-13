#include <raylib.h>
#include<imgui.h>

#include "./include/rlImGui.h"

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1600, 900, "An Indoor Propagation Simulator by Ibrahem Ayad");
  //ToggleFullscreen();
  SetTargetFPS(60);

  bool IsDarkMode = true;

  rlImGuiSetup(IsDarkMode);

  while (!WindowShouldClose()) {
    BeginDrawing();

    if(IsDarkMode){
      ClearBackground(BLACK);
    }
    else {
      ClearBackground(RAYWHITE);
    }

    DrawCircle(500, 500, 100, RED);

    rlImGuiBegin();

		//ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //ShowExampleMenuFile();
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

    EndDrawing();
  }

  rlImGuiShutdown();

  CloseWindow();

  return 0;
}
