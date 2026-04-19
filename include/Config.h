#pragma once

#include<raylib.h>
#include<string>
#include<vector>

enum State {
  Splash_Screen_State, Floor_Planning_State, Imported_Image_Settings_State
};

enum VIEW {
  View_2D, View_3D
};

enum Selected_Tool {
  None, Walls_Tool, Doors_Tool, Windows_Tool, Floor_Tool, Ceiling_Tool
};

enum THEME {
  Light_Theme, Dark_Theme
};

enum GIMBAL_ARROWS_3D_MODE {
  Rotation, Movement
};

enum CAMERA_3D_PROJECTION {
  Orthographic, Perspective
};

namespace CONF {

  static enum State state = Splash_Screen_State;

  static Selected_Tool tool_state = None;

  static enum VIEW View = View_2D;
  static enum CAMERA_3D_PROJECTION Camera_3D_Projection = Perspective;

  static bool Moving_2d = false;

  static THEME Theme = Light_Theme;

  static enum GIMBAL_ARROWS_3D_MODE Gimbal_Arrows_3D_Mode = Rotation;

  static bool Show_All_Walls = true;

  static float MMB_height = 25.0f;
  static float Tool_Bar_height = 60;
  static float Tool_Options_Bar_height = 50;

  static std::string wall_height_field_text = "3";
  static bool wall_height_field_active = false;
  static float Inputed_Wall_Height = 3.0f;

  static bool Floor_Start_Available = false;
  static Vector2 Floor_Start_2D;

  static bool Ceiling_Start_Available = false;
  static Vector2 Ceiling_Start_2D;
  static std::string ceiling_height_field_text = "3";
  static bool ceiling_height_field_active = false;
  static float Inputed_Ceiling_Height = 3.0f;

  static std::string door_height_field_text = "2";
  static bool door_height_field_active = false;
  static float Inputed_Door_Height = 2.0f;
  static std::string door_width_field_text = "1.5";
  static bool door_width_field_active = false;
  static float Inputed_Door_Width = 1.5f;
  static bool Door_Snapping = true;
  static std::vector<int> doors_on_walls_ids;

  // colors
  static Color Vibrant_Blue_Green = {20, 220, 180, 255};
  static Color Crimson = {220, 20, 60, 255};
  static Color Spicy_Pastel_Pink = {255, 201, 201, 255};

  static Color Ibo_Light_Gray = {200, 200, 200, 255};
  static Color Ibo_Dark_Gray = {124, 124, 124, 255};
  static Color Ibo_Darker_Gray = {72, 72, 72, 255};
  static Color Ibo_Darkest_Gray = {13, 13, 13, 255};
};

namespace GRID {
  static float spacing = 50;
};


