#include"../include/Draw_Tool_Options_Bar.hpp"

#include"imgui.h"
#include"../include/Config.hpp"

void Draw_Walls_Tool_Options() {

  ImGui::SetNextWindowPos(ImVec2(10 + 128 + 10, 22 + 10), ImGuiCond_Always);
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

void Draw_Doors_Tool_Options() {

  ImGui::SetNextWindowPos(ImVec2(10 + 128 + 10, 22 + 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(130, 500), ImGuiCond_Always);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
                                | ImGuiWindowFlags_NoTitleBar
                                | ImGuiWindowFlags_NoResize;

  if(ImGui::Begin("Doors_Tool_Options", nullptr, window_flags)){
    ImGui::Text("Door Height:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Door Height", &CONF::Door_Height);

    ImGui::Text("Door Width:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Door Width", &CONF::Door_Width);

    ImGui::Text("Door Material:");
    ImGui::SetNextItemWidth(100.0f);

    const char* MaterialNames[] = {
      "Empty",
      "Wood",
      "Metal"
    };
    const char* preview_text = MaterialNames[static_cast<int>(CONF::Door_Material)];

    if(ImGui::BeginCombo("##Door Material", preview_text)){
      for(int i = 0; i < 3; i++){
        DOOR_MATERIAL current_item = static_cast<DOOR_MATERIAL>(i);
        bool is_selected = (CONF::Door_Material == current_item);

        if (ImGui::Selectable(MaterialNames[i], is_selected)) {
          CONF::Door_Material = current_item;
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
  }

  ImGui::End();
  }

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void Draw_Windows_Tool_Options() {

  ImGui::SetNextWindowPos(ImVec2(10 + 128 + 10, 22 + 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(130, 500), ImGuiCond_Always);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
                                | ImGuiWindowFlags_NoTitleBar
                                | ImGuiWindowFlags_NoResize;

  if(ImGui::Begin("Windows_Tool_Options", nullptr, window_flags)){
    ImGui::Text("Base Height:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Window Base Height", &CONF::Window_Base_Height);

    ImGui::Text("Frame Height:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Window Frame Height", &CONF::Window_Frame_Height);

    ImGui::Text("Frame Width:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Window Width", &CONF::Window_Width);

    ImGui::Text("Material:");
    ImGui::SetNextItemWidth(100.0f);

    const char* MaterialNames[] = {
      "Empty",
      "Glass"
    };
    const char* preview_text = MaterialNames[static_cast<int>(CONF::Window_Material)];

    if(ImGui::BeginCombo("##Window Material", preview_text)){
      for(int i = 0; i < 2; i++){
        WINDOW_MATERIAL current_item = static_cast<WINDOW_MATERIAL>(i);
        bool is_selected = (CONF::Window_Material == current_item);

        if (ImGui::Selectable(MaterialNames[i], is_selected)) {
          CONF::Window_Material = current_item;
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
  }

  ImGui::End();
  }

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void Draw_Ceilings_Tool_Options() {

  ImGui::SetNextWindowPos(ImVec2(10 + 128 + 10, 22 + 10), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(130, 500), ImGuiCond_Always);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
                                | ImGuiWindowFlags_NoTitleBar
                                | ImGuiWindowFlags_NoResize;

  if(ImGui::Begin("Windows_Tool_Options", nullptr, window_flags)){
    ImGui::Text("Height:");
    ImGui::SetNextItemWidth(100.0f);
    ImGui::InputFloat("##Ceiling Height", &CONF::Ceiling_Height);

    ImGui::Text("Material:");
    ImGui::SetNextItemWidth(100.0f);

    const char* MaterialNames[] = {
      "Acoustic Tile",
      "Concrete", 
      "Metal Decking"
    };
    const char* preview_text = MaterialNames[static_cast<int>(CONF::Ceiling_Material)];

    if(ImGui::BeginCombo("##Ceiling Material", preview_text)){
      CONF::Is_Ceiling_Material_Dropdown_Menu_Down = true;
      for(int i = 0; i < 3; i++){
        CEILING_MATERIAL current_item = static_cast<CEILING_MATERIAL>(i);
        bool is_selected = (CONF::Ceiling_Material == current_item);

        if (ImGui::Selectable(MaterialNames[i], is_selected)) {
          CONF::Ceiling_Material = current_item;
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    else{
      CONF::Is_Ceiling_Material_Dropdown_Menu_Down = false;
    }

  ImGui::End();
  }

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}

void Draw_Tool_Options_Bar(){
  if(CONF::Selected_Tool == Walls_Tool){
    // the bacground
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10 + 2, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});
    DrawRectangleRounded({10 + 128 + 10, 22 + 10, 130, 165}, 0.3, 15, {229, 229, 229, 200});
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});

    Draw_Walls_Tool_Options();
  }

  if(CONF::Selected_Tool == Doors_Tool){
    // the bacground
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10 + 2, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});
    DrawRectangleRounded({10 + 128 + 10, 22 + 10, 130, 165}, 0.3, 15, {229, 229, 229, 200});
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10, 130, 165}, 0.3, 15, 2.0f, {128, 128, 128, 255});

    Draw_Doors_Tool_Options();
  }

  if(CONF::Selected_Tool == Windows_Tool){
    // the bacground
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10 + 2, 130, 210}, 0.3, 15, 2.0f, {128, 128, 128, 255});
    DrawRectangleRounded({10 + 128 + 10, 22 + 10, 130, 210}, 0.3, 15, {229, 229, 229, 200});
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10, 130, 210}, 0.3, 15, 2.0f, {128, 128, 128, 255});

    Draw_Windows_Tool_Options();
  }

  if(CONF::Selected_Tool == Ceilings_Tool){
    // the bacground
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10 + 2, 130, 120}, 0.3, 15, 2.0f, {128, 128, 128, 255});
    DrawRectangleRounded({10 + 128 + 10, 22 + 10, 130, 120}, 0.3, 15, {229, 229, 229, 200});
    DrawRectangleRoundedLinesEx({10 + 128 + 10, 22 + 10, 130, 120}, 0.3, 15, 2.0f, {128, 128, 128, 255});

    Draw_Ceilings_Tool_Options();
  }
}
