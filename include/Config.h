#pragma once

#include<raylib.h>

enum State {
  Splash_Screen_State, Floor_Planning_State, Imported_Image_Settings_State
};

enum VIEW {
  View_2D, View_3D
};

enum Selected_Tool {
  None, Walls_Tool, Doors_Tool, Windows_Tool
};

enum THEME {
  Light_Theme, Dark_Theme
};

enum GIMBAL_ARROWS_3D_MODE {
  Rotation, Movement
};

namespace CONF {
  static Selected_Tool tool_state = None;

  static enum VIEW View = View_2D;

  static THEME Theme = Light_Theme;

  static enum GIMBAL_ARROWS_3D_MODE Gimbal_Arrows_3D_Mode = Rotation;

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
  static float spacing = 30;
};


