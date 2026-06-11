#include"../include/Load_Resources.hpp"

#include<raylib.h>
#include"../include/Config.hpp"

void Load_Resources(){
  // Fonts
  CONF::Montserrat_Font_32 = LoadFontEx("../Resources/Fonts/Montserrat-Bold.ttf", 32, 0, 0);
  CONF::Montserrat_Font_20 = LoadFontEx("../Resources/Fonts/Montserrat-Bold.ttf", 20, 0, 0);

  // Shaders
  CONF::Grid_3D_Shader = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/Grid_3D_Shader.frag");
  CONF::Grid_3D_Shader_Uniform_Camera_Position_ID = GetShaderLocation(CONF::Grid_3D_Shader, "Camera_Position");
  CONF::Grid_3D_Shader_Uniform_Camera_Target_ID = GetShaderLocation(CONF::Grid_3D_Shader, "Camera_Target");
  CONF::Wall_Shader_2D = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/Wall_Shader_2D.frag");
  CONF::Wall_Shader_3D = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/Wall_Shader_3D.frag");
  CONF::Ceiling_Shader_2D = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/Ceiling_Shader_2D.frag");
  CONF::ITU_NLOS_Shader_3D = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/ITU_NLOS_Shader_3D.frag");
  CONF::ITU_NLOS_3D_Shader_Count_ID = GetShaderLocation(CONF::ITU_NLOS_Shader_3D, "u_transmitterCount");
  CONF::ITU_NLOS_3D_Shader_Positions_ID = GetShaderLocation(CONF::ITU_NLOS_Shader_3D, "u_BS_Positions[0]");
  CONF::ITU_LOS_Shader_3D = LoadShader("../Resources/Shaders/Standard_Vertex_Shader.vert", "../Resources/Shaders/ITU_LOS_Shader.frag");
  CONF::ITU_LOS_Shader_3D_Master_Visibility_Polygon_ID = GetShaderLocation(CONF::ITU_LOS_Shader_3D, "Master_Visibility_Polygon");
  CONF::ITU_LOS_3D_Shader_Count_ID = GetShaderLocation(CONF::ITU_LOS_Shader_3D, "u_transmitterCount");
  CONF::ITU_LOS_3D_Shader_Positions_ID = GetShaderLocation(CONF::ITU_LOS_Shader_3D, "u_BS_Positions[0]");

  // Images
  CONF::Walls_Tool_Icon = LoadTexture("../Resources/Images/Walls_Tool_Icon.png");
  CONF::Doors_Tool_Icon = LoadTexture("../Resources/Images/Doors_Tool_Icon.png");
  CONF::Windows_Tool_Icon = LoadTexture("../Resources/Images/Windows_Tool_Icon.png");
  CONF::Floors_Tool_Icon = LoadTexture("../Resources/Images/Floors_Tool_Icon.png");
  CONF::Ceilings_Tool_Icon = LoadTexture("../Resources/Images/Ceilings_Tool_Icon.png");
  CONF::Transmitters_Tool_Icon = LoadTexture("../Resources/Images/Transmitters_Tool_Icon.png");
  CONF::Receivers_Tool_Icon = LoadTexture("../Resources/Images/Receivers_Tool_Icon.png");
  CONF::Magnet_Icon = LoadTexture("../Resources/Images/Magnet_Icon.png");
  CONF::Transmitter_Image = LoadTexture("../Resources/Images/Transmitter_Image.png");
}
