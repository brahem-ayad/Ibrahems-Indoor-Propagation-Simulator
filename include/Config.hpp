#pragma once

#include<raylib.h>
#include"./Generate_Wall_Collision_Quads.hpp"

enum MODE { Drawing_Mode, Simulation_Mode };
enum VIEW { View_2D, View_3D };
enum GIZMO_ARROWS_3D_MODE { Gizmo_Arrows_Rotation_Mode, Gizmo_Arrows_Movement_Mode };
enum CAMERA_3D_PROJECTION { Perspective, Orthographic };
enum SELECTED_TOOL { None_Tool, Walls_Tool, Doors_Tool, Windows_Tool, Floors_Tool, Ceilings_Tool, Transmitter_Tool, Receiver_Tool };

enum WALL_MATERIAL { Drywall, Concrete_Wall, Brick_Wall, Wooden_Wall, Glass_Wall, Metal_Wall };
enum DOOR_MATERIAL { Empty_Door, Wooden_Door, Metal_Door };
enum WINDOW_MATERIAL { Empty_Window, Glass_Window };
enum FLOOR_MATERIAL { Concrete_Floor, Wooden_Floor, Raised_Access_Floor };
enum CEILING_MATERIAL { Acoustic_Tile, Concrete_Ceiling, Metal_Decking };

namespace CONF{
  inline MODE Mode = Drawing_Mode;
  inline VIEW View = View_2D;
  inline GIZMO_ARROWS_3D_MODE Gizmo_Arrows_3D_Mode = Gizmo_Arrows_Rotation_Mode;
  inline CAMERA_3D_PROJECTION Camera_3D_Projection = Perspective;
  inline SELECTED_TOOL Selected_Tool = None_Tool;

  inline Camera2D camera2;
  inline Camera3D camera3;
  inline Camera3D camerafps;

  inline bool Moving_2d;

  inline float Grid_Spacing = 50.0f;

  inline float Tools_Bar_Scroll = 0.0f;

  inline bool Enable_Snapping = true;

  // View Options
  inline bool Show_Grid = true;
  inline bool Show_Tools_Bar = true;
  inline bool Show_Tool_Options_Bar = true;
  inline bool Show_Right_Sidebar = false;
  inline bool Show_Viewport_Gizmo = true;
  inline bool Show_Mode_Selector = true;

  inline bool Fullscreen_Mode = false;

  // Fonts
  inline Font Montserrat_Font_32;
  inline Font Montserrat_Font_20;

  // Shaders
  inline Shader Grid_3D_Shader;
  inline int Grid_3D_Shader_Uniform_Camera_Target_ID;
  inline int Grid_3D_Shader_Uniform_Camera_Position_ID;
  inline Shader Wall_Shader_2D;
  inline Shader Wall_Shader_3D;
  inline Shader Ceiling_Shader_2D;
  inline Shader ITU_NLOS_Shader_3D;
  inline int ITU_NLOS_3D_Shader_Count_ID;
  inline int ITU_NLOS_3D_Shader_Positions_ID;
  inline Shader ITU_LOS_Shader_3D;
  inline int ITU_LOS_Shader_3D_Master_Visibility_Polygon_ID;
  inline int ITU_LOS_3D_Shader_Count_ID;
  inline int ITU_LOS_3D_Shader_Positions_ID;

  // Images
  inline Texture2D Walls_Tool_Icon;
  inline Texture2D Doors_Tool_Icon;
  inline Texture2D Windows_Tool_Icon;
  inline Texture2D Floors_Tool_Icon;
  inline Texture2D Ceilings_Tool_Icon;
  inline Texture2D Transmitters_Tool_Icon;
  inline Texture2D Receivers_Tool_Icon;
  inline Texture2D Magnet_Icon;
  inline Texture2D Transmitter_Image;

  // ----- Tool Options ---- //
  inline float Wall_Height = 3.0f;
  inline float Wall_Width = 0.1f;
  inline WALL_MATERIAL Wall_Material = Concrete_Wall;
  inline bool Is_Wall_Start_Available = false;
  inline Vector2 Current_Wall_Start_Screen;
  inline Vector2 Current_Wall_Start_World;
  inline bool Is_Wall_Material_Dropdown_Menu_Down = false;

  inline float Door_Height = 2.0f;
  inline float Door_Width = 1.0f;
  inline DOOR_MATERIAL Door_Material = Empty_Door;

  inline float Window_Base_Height = 1.0f;
  inline float Window_Frame_Height = 1.0f;
  inline float Window_Width = 1.0f;
  inline WINDOW_MATERIAL Window_Material = Empty_Window;

  inline bool Is_Floor_Start_Available = false;
  inline Vector2 Current_Floor_Start_World;

  inline bool Is_Ceiling_Start_Available = false;
  inline Vector2 Current_Ceiling_Start_World;
  inline float Ceiling_Height = 3.0f;
  inline CEILING_MATERIAL Ceiling_Material = Concrete_Ceiling;
  inline bool Is_Ceiling_Material_Dropdown_Menu_Down = false;

  inline float Transmitter_Height = 1.5f;

  inline std::vector<WallQuad3D> Wall_Quads_3D;
  inline bool View_Walls_Wireframe = false;

  inline bool ITU_Sim_Started = false;
};



