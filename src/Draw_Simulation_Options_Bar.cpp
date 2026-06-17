#include"../include/Draw_Simulation_Options_Bar.hpp"

#include<raylib.h>
#include"imgui.h"
#include"../include/Config.hpp"

void Draw_Simulation_Options_Bar(){
  DrawRectangleRoundedLinesEx({10, 22 + 10 + 2 + 30 + 10, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});
  DrawRectangleRounded({10, 22 + 10 + 30 + 10, 130, 165}, 0.3, 15, {229, 229, 229, 200});
  DrawRectangleRoundedLinesEx({10, 22 + 10 + 30 + 10, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});

  ImGui::SetNextWindowPos(ImVec2(10, 22 + 10 + 30 + 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(130, 500), ImGuiCond_Always);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
                                | ImGuiWindowFlags_NoTitleBar
                                | ImGuiWindowFlags_NoResize;

  if(ImGui::Begin("Walls_Tool_Options", nullptr, window_flags)){
    ImGui::Text("Wall Height:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Wall Height", &CONF::Wall_Height);

    ImGui::Text("Wall Width:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Wall Width", &CONF::Wall_Width);

    ImGui::Text("Wall Material:");
    ImGui::SetNextItemWidth(100.0f);

    const char* MaterialNames[] = {
        "Drywall",
        "Concrete",
        "Brick",
        "Wood",
        "Glass",
        "Metal"
    };
    const char* preview_text = MaterialNames[static_cast<int>(CONF::Wall_Material)];

    if(ImGui::BeginCombo("##Wall Material", preview_text)){
      CONF::Is_Wall_Material_Dropdown_Menu_Down = true;
      for(int i = 0; i < 6; i++){
        WALL_MATERIAL current_item = static_cast<WALL_MATERIAL>(i);
        bool is_selected = (CONF::Wall_Material == current_item);

        if (ImGui::Selectable(MaterialNames[i], is_selected)) {
          CONF::Wall_Material = current_item;
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    else{
      CONF::Is_Wall_Material_Dropdown_Menu_Down = false;
    }

  ImGui::End();
  }

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();

}
