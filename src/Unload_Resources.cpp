#include"../include/Unload_Resources.hpp"

#include<raylib.h>
#include"Config.hpp"

void Unload_Resources(){

  // Fonts
  UnloadFont(CONF::Montserrat_Font_32);
  UnloadFont(CONF::Montserrat_Font_20);

  // shaders
  UnloadShader(CONF::Grid_3D_Shader);
  UnloadShader(CONF::Wall_Shader_2D);
  UnloadShader(CONF::Wall_Shader_3D);
  UnloadShader(CONF::Ceiling_Shader_2D);
  UnloadShader(CONF::ITU_NLOS_Shader_3D);
  UnloadShader(CONF::ITU_LOS_Shader_3D);

  // Images
  UnloadTexture(CONF::Walls_Tool_Icon);
  UnloadTexture(CONF::Doors_Tool_Icon);
  UnloadTexture(CONF::Windows_Tool_Icon);
  UnloadTexture(CONF::Floors_Tool_Icon);
  UnloadTexture(CONF::Ceilings_Tool_Icon);
  UnloadTexture(CONF::Transmitters_Tool_Icon);
  UnloadTexture(CONF::Receivers_Tool_Icon);
  UnloadTexture(CONF::Magnet_Icon);
  UnloadTexture(CONF::Transmitter_Image);
}
