#include"../include/Draw_Main_Menu_Bar.hpp"

#include<raylib.h>
#include"imgui.h"
#include"../include/Config.hpp"

//void Draw_Mode_Selector() {
//    float comboWidth = 160.0f; 
// 
//    // Calculate center relative to the menu bar width itself
//    float menuBarWidth = (float)ImGui::GetWindowWidth();
//    float centerCursorX = (menuBarWidth - comboWidth) * 0.5f;
//
//    // Use SameLine to jump the cursor layout over to the calculated center point
//    if (ImGui::GetCursorPosX() < centerCursorX) {
//        ImGui::SameLine(centerCursorX);
//    }
//
//    // Match your light-mode background values flat onto the menu bar
//    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f)); 
//    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.78f, 0.78f, 0.78f, 1.00f)); 
//    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
//
//    // triangle box
//    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
//    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.78f, 0.78f, 1.00f));
//    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.70f, 0.70f, 1.00f));
//
//    // Strip borders and roundings so it embeds natively into the menu panel strip
//    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
//    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
// 
//    ImGui::PushItemWidth(comboWidth);
//
//    const char* modes[] = { "Drawing Mode", "Simulation Mode" };
//    int currentItem = static_cast<int>(CONF::Mode);
//
//    if (ImGui::Combo("##ModeCombo", &currentItem, modes, IM_ARRAYSIZE(modes))) {
//        CONF::Mode = static_cast<MODE>(currentItem);
//        CONF::Selected_Tool = None_Tool;
//    }
//
//    ImGui::PopItemWidth();
//    ImGui::PopStyleVar(2);
//    ImGui::PopStyleColor(6);
//}

void Draw_ImGui_Main_Menu_Bar(){
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("File")){
      if(ImGui::MenuItem("New", "Ctrl+N")){
 
      }
      if(ImGui::MenuItem("Open", "Ctrl+O")){
 
      }
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Edit")){
      if(ImGui::MenuItem("Undo", "Ctrl+Z")){
 
      }
      if(ImGui::MenuItem("Redo", "Ctrl+Y")){
 
      }
      ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("View")){
      if(ImGui::MenuItem("Grid", "Alt+G", &CONF::Show_Grid)){}
      if(ImGui::MenuItem("Tools Bar", "Alt+T", &CONF::Show_Tools_Bar)){}
      if(ImGui::MenuItem("Tool Options Bar", "Alt+Y", &CONF::Show_Tool_Options_Bar)){}
      if(ImGui::MenuItem("Right Sidebar", "Alt+N", &CONF::Show_Right_Sidebar)){}
      if(ImGui::MenuItem("Viewport Gizmo", "Alt+V", &CONF::Show_Viewport_Gizmo)){}
      if(ImGui::MenuItem("Mode Selector", "Alt+M", &CONF::Show_Mode_Selector)){}
      if(ImGui::MenuItem("Full Screen", "Alt+F", &CONF::Fullscreen_Mode)){
        ToggleFullscreen();
      }
      if(ImGui::MenuItem("Walls Wireframes (Debug)", "Alt+L", &CONF::View_Walls_Wireframe)){}
      ImGui::EndMenu();
    }

    ImGui::PopStyleVar();

    //Draw_Mode_Selector();

    ImGui::EndMainMenuBar();
  }

}
