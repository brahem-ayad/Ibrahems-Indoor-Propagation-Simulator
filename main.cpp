// ----- includes to libraries in my system ----- //
#include<raylib.h>
#include<raymath.h>

// ----- includes to libraries that are put in this library ----- //
#include"include/Cameras.h"
#include"include/Config.h"
#include"include/Floor_Plan.h"
#include"include/Splash_Screen.h"
//#include"include/Load_Image.h"
#include"include/Main_Menu_Bar.h"

bool Should_Exit_App = false;
bool Show_Exit_Pop_Up = false;

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1600, 900, "Crimson Indoor Simulator");
  SetTargetFPS(60);

  // Setting up the Cameras
  Camera2D camera2;
  Camera3D camera3;
  Set_Cameras(camera2, camera3);

  // Loading the Shader :
  Shader shader = LoadShader("./resources/shaders/shader.vert", "./resources/shaders/shader.frag");

  Shader grid_shader = LoadShader("./resources/shaders/shader.vert", "./resources/shaders/Grid_Shader.frag");
  int grid_shader_uniform_camera_target = GetShaderLocation(grid_shader, "Camera_Target");
  int grid_shader_uniform_camera_position = GetShaderLocation(grid_shader, "Camera_Position");

  // Loading the font
  Font Montserrat_Font_32 = LoadFontEx("./resources/fonts/Montserrat-Bold.ttf", 32, 0, 0);
  Font Montserrat_Font_20 = LoadFontEx("./resources/fonts/Montserrat-Bold.ttf", 20, 0, 0);

  // Loading the Splash Screen Texture
  Texture2D Splash_Screen_Image_Light = LoadTexture("./resources/images/Crimson_Splash_Screen_Image_Light.png");
  Texture2D Splash_Screen_Image_Dark = LoadTexture("./resources/images/Crimson_Splash_Screen_Image_Dark.png");
  Texture2D Sun_Icon = LoadTexture("./resources/images/Sun_Icon.png");
  Texture2D Moon_Icon = LoadTexture("./resources/images/Moon_Icon.png");
  bool Unload_Splash_Screen_Image = false;

  // For the potential floor plan image to be imported
  Texture2D Floor_Plan_Texture;
  bool Is_Floor_Plan_Image_Loaded = false;

  enum State state = Splash_Screen_State;
  enum Selected_Tool selected_tool = None;

  while (!WindowShouldClose()) {
 
    SetShaderValue(grid_shader, grid_shader_uniform_camera_target, &camera3.target, SHADER_UNIFORM_VEC3);
    SetShaderValue(grid_shader, grid_shader_uniform_camera_position, &camera3.position, SHADER_UNIFORM_VEC3);

    if(IsWindowResized()){
      camera2.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
    }

    BeginDrawing();
    if(CONF::Theme == Light_Theme) ClearBackground(WHITE);
    else ClearBackground(BLACK);

    if(state == Splash_Screen_State){
      Draw_Floor_Planning_State(camera2, camera3, Montserrat_Font_32, Montserrat_Font_20, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded, selected_tool, shader, grid_shader);
      Draw_Splash_Screen(Splash_Screen_Image_Light, Splash_Screen_Image_Dark, Unload_Splash_Screen_Image, state, Montserrat_Font_32, Sun_Icon, Moon_Icon);
    }
    else if(state == Floor_Planning_State){
      Draw_Floor_Planning_State(camera2, camera3, Montserrat_Font_32, Montserrat_Font_20, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded, selected_tool, shader, grid_shader);
    }

    Draw_Main_Menu_Bar(Montserrat_Font_20);

    EndDrawing();

    if(IsTextureValid(Splash_Screen_Image_Light)&& Unload_Splash_Screen_Image == true){
      UnloadTexture(Splash_Screen_Image_Light);
      UnloadTexture(Splash_Screen_Image_Dark);
      UnloadTexture(Sun_Icon);
      UnloadTexture(Moon_Icon);
      Splash_Screen_Image_Light.id = 0;
      Splash_Screen_Image_Dark.id = 0;
      Sun_Icon.id = 0;
      Moon_Icon.id = 0;
    }
  }

  if(IsTextureValid(Splash_Screen_Image_Light)){
    UnloadTexture(Splash_Screen_Image_Light);
    UnloadTexture(Splash_Screen_Image_Dark);
    UnloadTexture(Sun_Icon);
    UnloadTexture(Moon_Icon);
  }
  UnloadFont(Montserrat_Font_32);
  UnloadFont(Montserrat_Font_20);

  CloseWindow();

  return 0;
}
