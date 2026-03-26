// ----- includes from the standard library ----- //
#include <cmath>
#include<string>
#include<vector>

// ----- includes to libraries in my system ----- //
#include<raylib.h>
#include<raymath.h>
#include<imgui.h>

// ----- includes to libraries that are put in this library ----- //
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//#include"style_dark.h"
#include"portable-file-dialogs.h"
#include"rlImGui.h"

enum State {
  Splash_Screen_State, Floor_Planning_State, Imported_Image_Settings_State
};

void Load_Floor_Plan_Image(Texture2D &texture, bool &imageLoaded){
  // Open the dialog
  auto selection = pfd::open_file("Select an image to import", ".",
    { "Image Files (.png, .jpg, .bmp)", "*.png *.jpg *.jpeg *.bmp",
      "All Files", "*" },
    pfd::opt::none).result();

  if (selection.empty() == false) {
    // Unload previous texture if one exists
    if (imageLoaded) UnloadTexture(texture);

    // Load the new texture from the selected path
    texture = LoadTexture(selection[0].c_str());

    if (texture.id > 0) {
        imageLoaded = true;
    }
  }

  // There should be a state just for the imported image
  // to select its size and origin point
  // and then when everything is ready, we click enter and go back to the floor planning state
}

void Draw_Splash_Screen(Texture2D Splash_Screen_Image, bool &Unload_Splash_Screen_Image, State &state, Font font, int font_size) {

  // Drawing the Splash Screen Texture
  DrawTexture(Splash_Screen_Image, GetScreenWidth()/2 - Splash_Screen_Image.width/2, GetScreenHeight()/2 - Splash_Screen_Image.height/2, WHITE);


  // Checking if one of the buttons is pressed

  float offset = 50;
  float vertical_center = (float)GetScreenHeight()/2 + offset;

  float padding = 10;

  char* text1 = (char*)"Create a New Empty File";
  char* text2 = (char*)"Open an Existing File";
  char* text3 = (char*)"Load an Example";

  Vector2 text1_size = MeasureTextEx(font, text1, font_size, 1);
  Vector2 text1_pos = { (float)GetScreenWidth()/2 - text1_size.x/2, vertical_center};

  Vector2 text2_size = MeasureTextEx(font, text2, font_size, 1);
  Vector2 text2_pos = { (float)GetScreenWidth()/2 - text2_size.x/2, vertical_center + 65};

  Vector2 text3_size = MeasureTextEx(font, text3, font_size, 1);
  Vector2 text3_pos = { (float)GetScreenWidth()/2 - text3_size.x/2, vertical_center + 65 * 2};

  Rectangle Text_Rec_1 = { text1_pos.x - padding, text1_pos.y - padding/2, text1_size.x + padding*2, text1_size.y + padding };
  Rectangle Text_Rec_2 = { text1_pos.x - padding, text2_pos.y - padding/2, text1_size.x + padding*2, text2_size.y + padding };
  Rectangle Text_Rec_3 = { text1_pos.x - padding, text3_pos.y - padding/2, text1_size.x + padding*2, text3_size.y + padding };

  Color PINK_2 = {255, 165, 165, 255};

  DrawRectangleRounded(Text_Rec_1, 0.5, 10, Fade(PINK_2, 0.5));
  DrawRectangleRoundedLinesEx(Text_Rec_1, 0.5, 10, 3, MAROON);
  DrawTextEx(font, text1, text1_pos, font_size, 1, MAROON);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_1)){
    DrawRectangleRounded(Text_Rec_1, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_1, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      state = Floor_Planning_State;
      Unload_Splash_Screen_Image = true;
    }
  }

  DrawRectangleRounded(Text_Rec_2, 0.5, 10, Fade(PINK_2, 0.5));
  DrawRectangleRoundedLinesEx(Text_Rec_2, 0.5, 10, 3, MAROON);
  DrawTextEx(font, text2, text2_pos, font_size, 1, MAROON);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_2)){
    DrawRectangleRounded(Text_Rec_2, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_2, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    }
  }

  DrawRectangleRounded(Text_Rec_3, 0.5, 10, Fade(PINK_2, 0.5));
  DrawRectangleRoundedLinesEx(Text_Rec_3, 0.5, 10, 3, MAROON);
  DrawTextEx(font, text3, text3_pos, font_size, 1, MAROON);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_3)){
    DrawRectangleRounded(Text_Rec_3, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_3, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    }
  }


  // Checking if the outside of the splash screen has been pressed
  Rectangle REC = {
    (float)GetScreenWidth()/2 - (float)Splash_Screen_Image.width/2, (float)GetScreenHeight()/2 - (float)Splash_Screen_Image.height/2,
    (float)Splash_Screen_Image.width, (float)Splash_Screen_Image.height
  };

  if(CheckCollisionPointRec(GetMousePosition(), REC) == false){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      state = Floor_Planning_State;
      Unload_Splash_Screen_Image = true;
    }
  }

}

void Update_2D_Camera(Camera2D &camera, State state){
if(state == Floor_Planning_State){

  float speed = 3;

  float max_zoom = 0.3f;
  float min_zoom = 6.0f;

  // Zoom based on mouse wheel
  float wheel = GetMouseWheelMove();
  if (wheel != 0)
  {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      // Set the offset to where the mouse is
      camera.offset = GetMousePosition();

      // Set the target to match, so that the camera maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      camera.target = mouseWorldPos;

      // Zoom increment
      // Uses log scaling to provide consistent zoom speed
      float scale = 0.2f*wheel;
      camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
  }
  if(camera.zoom < max_zoom){
    camera.zoom = max_zoom;
  }
  if(camera.zoom > min_zoom){
    camera.zoom = min_zoom;
  }

  if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera.zoom);
    camera.target = Vector2Add(camera.target, delta);
  }

  if(IsKeyDown(KEY_LEFT)){
    camera.target.x -= speed;
  }
  if(IsKeyDown(KEY_RIGHT)){
    camera.target.x += speed;
  }
  if(IsKeyDown(KEY_UP)){
    camera.target.y -= speed;
  }
  if(IsKeyDown(KEY_DOWN)){
    camera.target.y += speed;
  }

}
}

void Draw_2D_Grid(Camera2D camera) {
  float spacing = 30;
  float margin = 60;

  Vector2 topLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
  Vector2 bottomRight = GetScreenToWorld2D((Vector2){ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

  float x_min_bound = topLeft.x - margin + spacing - (int)std::ceil( topLeft.x - margin ) % (int)spacing;
  float x_max_bound = bottomRight.x;
  float y_min_bound = topLeft.y - margin + spacing - (int)std::ceil( topLeft.y - margin ) % (int)spacing;
  float y_max_bound = bottomRight.y;

  float test = Remap(camera.zoom, 0.3f, 2.0f, 0.1f, 1.0f);

  for(int i = x_min_bound; i < x_max_bound; i += spacing){
    DrawLine(i, y_min_bound, i, y_max_bound, Fade(GRAY, test));
  }
  for(int i = y_min_bound; i < y_max_bound; i += spacing){
    DrawLine(x_min_bound, i, x_max_bound, i, Fade(GRAY, test));
  }
}

void Draw_Floor_Planning_State(Camera2D &camera, Font font, float font_size, State &state, Texture2D &texture, bool &imageLoaded){

  rlImGuiBegin();

  if (ImGui::BeginMainMenuBar())
  {
      if (ImGui::BeginMenu("File"))
      {
          if (ImGui::MenuItem("Create New Empty File", "Ctrl+N")) {}
          if (ImGui::MenuItem("Save the Current File", "Ctrl+S")) {}
          if (ImGui::MenuItem("Open an Existing File", "Ctrl+O")) {}
          if (ImGui::MenuItem("Import an Image", "Ctrl+I")) {
            Load_Floor_Plan_Image(texture, imageLoaded);
          }
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

  Update_2D_Camera(camera, state);

  BeginMode2D(camera);

  Draw_2D_Grid(camera);
  if(imageLoaded){
    DrawTexture(texture, 0, 0, Fade(WHITE, 0.5));
  }

  EndMode2D();

  DrawRectangle(0, 75, GetScreenWidth(), 2, Fade(GRAY, 0.5));
  DrawRectangle(0, 0, GetScreenWidth(), 75, { 240, 240, 240, 200 });
  DrawTextEx(font, "Walls", {20, 32}, 32, 1, DARKGRAY);

  rlImGuiEnd();
}

void Set_Cameras(Camera2D &camera2, Camera3D &camera3){
  camera3.position = {5, 5, 5};
  camera3.target = {0, 0, 0};
  camera3.up = {0, 1, 0};
  camera3.fovy = 45.0f;
  camera3.projection = CAMERA_PERSPECTIVE;

  camera2.offset = {0, 0};
  camera2.target = {0, 0};
  camera2.rotation = 0;
  camera2.zoom = 1.0f;
}

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1600, 900, "Crimson Indoor Simulator");
  SetTargetFPS(60);

  // Setting up the Cameras
  Camera2D camera2;
  Camera3D camera3;
  Set_Cameras(camera2, camera3);

  // Loading the font
  int Montserrat_Font_Size = 32;
  Font Montserrat_Font = LoadFontEx("./fonts/Montserrat-Bold.ttf", Montserrat_Font_Size, 0, 0);

  // Loading the Spash Screen Texture
  Texture2D Splash_Screen_Image = LoadTexture("Images/Crimson_Splash_Screen.png");
  bool Unload_Splash_Screen_Image = false;

  // Setting up ImGui
  rlImGuiSetup(false);

  // For the potential floor plan image to be imported
  Texture2D Floor_Plan_Texture;
  bool Is_Floor_Plan_Image_Loaded = false;

  enum State state = Splash_Screen_State;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if(state == Splash_Screen_State){
      Draw_Floor_Planning_State(camera2, Montserrat_Font, Montserrat_Font_Size, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded);
      Draw_Splash_Screen(Splash_Screen_Image, Unload_Splash_Screen_Image, state, Montserrat_Font, Montserrat_Font_Size);
    }
    else if(state == Floor_Planning_State){
      Draw_Floor_Planning_State(camera2, Montserrat_Font, Montserrat_Font_Size, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded);
    }

    EndDrawing();

    if(IsTextureValid(Splash_Screen_Image) && Unload_Splash_Screen_Image == true){
      UnloadTexture(Splash_Screen_Image);
      Splash_Screen_Image.id = 0;
    }
  }

  if(IsTextureValid(Splash_Screen_Image)){
    UnloadTexture(Splash_Screen_Image);
  }
  UnloadFont(Montserrat_Font);

  CloseWindow();

  return 0;
}
