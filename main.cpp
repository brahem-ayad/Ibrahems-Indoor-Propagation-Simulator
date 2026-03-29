// ----- includes from the standard library ----- //
#include <cmath>
#include<string>
#include<vector>

// ----- includes to libraries in my system ----- //
#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<imgui.h>

// ----- includes to libraries that are put in this library ----- //
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//#include"style_dark.h"
#include"portable-file-dialogs.h"
#include"rlImGui.h"

bool Should_Exit_App = false;
bool Show_Exit_Pop_Up = false;

enum State {
  Splash_Screen_State, Floor_Planning_State, Imported_Image_Settings_State
};

enum View {
  View_2D, View_3D
};

enum Selected_Tool {
  None, Walls_Tool
};

namespace CAMERA {
  float speed = 0.01;
  float base_speed = 0.01;
  float max_speed = 0.2;
  float acceleration = 0.001;
  float rotation_speed = 1;
};

namespace GRID {
  float spacing = 30;
};

class Wall_2D{
  private:
    Vector3 V1;
    Vector3 V2;
    Vector3 V3;
    Vector3 V4;

  public:
    Vector2 Start;
    Vector2 End;
    float Height = 1;

  void Draw_2D() {
    DrawLineEx(Start, End, 3, BLACK);
  }

  void Draw_3D(float thickness = 1.0f) {
      float factor = GRID::spacing;
      float halfThick = (thickness / factor) / 2.0f;
 
      // 1. Calculate direction and perpendicular vector (Normal)
      float dx = (End.x - Start.x) / factor;
      float dy = (End.y - Start.y) / factor;
      float len = sqrt(dx * dx + dy * dy);
 
      // Normalize and scale by half-thickness
      float nx = (-dy / len) * halfThick;
      float ny = (dx / len) * halfThick;
 
      // 2. Define the 4 bottom corners (Z = 0)
      Vector3 b1 = { (Start.x / factor) + nx, (Start.y / factor) + ny, 0 };
      Vector3 b2 = { (Start.x / factor) - nx, (Start.y / factor) - ny, 0 };
      Vector3 b3 = { (End.x / factor) + nx, (End.y / factor) + ny, 0 };
      Vector3 b4 = { (End.x / factor) - nx, (End.y / factor) - ny, 0 };
 
      // 3. Define the 4 top corners (Z = Height)
      Vector3 t1 = { b1.x, b1.y, Height };
      Vector3 t2 = { b2.x, b2.y, Height };
      Vector3 t3 = { b3.x, b3.y, Height };
      Vector3 t4 = { b4.x, b4.y, Height };

      Color color = WHITE;

      // 4. Draw the faces (using your DrawTriangle3D helper)
      // Front Face
      DrawTriangle3D(b1, t1, b3, color); DrawTriangle3D(t1, t3, b3, color);
      // Back Face
      DrawTriangle3D(b2, t2, b4, color); DrawTriangle3D(t2, t4, b4, color);
      // Top Face
      DrawTriangle3D(t1, t2, t3, color); DrawTriangle3D(t2, t4, t3, color);
      // Side 1 (Start cap)
      DrawTriangle3D(b1, t1, b2, color); DrawTriangle3D(t1, t2, b2, color);
      // Side 2 (End cap)
      DrawTriangle3D(b3, t3, b4, color); DrawTriangle3D(t3, t4, b4, color);
  }

};

namespace FP {
  Vector2 starting_pos;
  bool is_starting_pos_available = false;

  std::vector<Wall_2D> walls_2d;
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

void Draw_Splash_Screen(Texture2D Splash_Screen_Image, bool &Unload_Splash_Screen_Image, State &state, Font font_32) {

  // Drawing the Splash Screen Texture
  DrawTexture(Splash_Screen_Image, GetScreenWidth()/2 - Splash_Screen_Image.width/2, GetScreenHeight()/2 - Splash_Screen_Image.height/2, WHITE);


  // Checking if one of the buttons is pressed

  float offset = 50;
  float vertical_center = (float)GetScreenHeight()/2 + offset;

  float padding = 10;

  char* text1 = (char*)"Create a New Empty File";
  char* text2 = (char*)"Open an Existing File";
  char* text3 = (char*)"Load an Example";

  Vector2 text1_size = MeasureTextEx(font_32, text1, 32, 1);
  Vector2 text1_pos = { (float)GetScreenWidth()/2 - text1_size.x/2, vertical_center};

  Vector2 text2_size = MeasureTextEx(font_32, text2, 32, 1);
  Vector2 text2_pos = { (float)GetScreenWidth()/2 - text2_size.x/2, vertical_center + 65};

  Vector2 text3_size = MeasureTextEx(font_32, text3, 32, 1);
  Vector2 text3_pos = { (float)GetScreenWidth()/2 - text3_size.x/2, vertical_center + 65 * 2};

  Rectangle Text_Rec_1 = { text1_pos.x - padding, text1_pos.y - padding/2, text1_size.x + padding*2, text1_size.y + padding };
  Rectangle Text_Rec_2 = { text1_pos.x - padding, text2_pos.y - padding/2, text1_size.x + padding*2, text2_size.y + padding };
  Rectangle Text_Rec_3 = { text1_pos.x - padding, text3_pos.y - padding/2, text1_size.x + padding*2, text3_size.y + padding };

  Color PINK_2 = {255, 165, 165, 255};

  DrawRectangleRounded(Text_Rec_1, 0.5, 10, Fade(PINK_2, 0.5));
  DrawRectangleRoundedLinesEx(Text_Rec_1, 0.5, 10, 3, MAROON);
  DrawTextEx(font_32, text1, text1_pos, 32, 1, MAROON);
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
  DrawTextEx(font_32, text2, text2_pos, 32, 1, MAROON);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_2)){
    DrawRectangleRounded(Text_Rec_2, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_2, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    }
  }

  DrawRectangleRounded(Text_Rec_3, 0.5, 10, Fade(PINK_2, 0.5));
  DrawRectangleRoundedLinesEx(Text_Rec_3, 0.5, 10, 3, MAROON);
  DrawTextEx(font_32, text3, text3_pos, 32, 1, MAROON);
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

void Update_3D_Camera(Camera3D &camera){
  Vector2 target_2d = {camera.target.x, camera.target.y};
  Vector2 position_2d = {camera.position.x, camera.position.y};
  Vector2 direction_2d = Vector2Normalize(Vector2Subtract(target_2d, position_2d));
  Vector2 direction_2d_perp = {direction_2d.y, -direction_2d.x};

  Vector3 direction = Vector3Scale({direction_2d.x, direction_2d.y, 0}, CAMERA::speed);
  Vector3 direction_perp = Vector3Scale({direction_2d_perp.x, direction_2d_perp.y, 0}, CAMERA::speed);

  float radius = Vector2Distance(target_2d, position_2d);

  if(IsKeyDown(KEY_W)){

    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }

    camera.position = Vector3Add(camera.position, direction);
    camera.target = Vector3Add(camera.target, direction);
  }

  if(IsKeyReleased(KEY_W)){
    CAMERA::speed = CAMERA::base_speed;
  }

  if(IsKeyDown(KEY_S)){

    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }

    camera.position = Vector3Subtract(camera.position, direction);
    camera.target = Vector3Subtract(camera.target, direction);
  }

  if(IsKeyReleased(KEY_S)){
    CAMERA::speed = CAMERA::base_speed;
  }

  if(IsKeyDown(KEY_A)){

    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }

    camera.position = Vector3Subtract(camera.position, direction_perp);
    camera.target = Vector3Subtract(camera.target, direction_perp);
  }

  if(IsKeyReleased(KEY_A)){
    CAMERA::speed = CAMERA::base_speed;
  }

  if(IsKeyDown(KEY_D)){

    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }

    camera.position = Vector3Add(camera.position, direction_perp);
    camera.target = Vector3Add(camera.target, direction_perp);
  }

  if(IsKeyReleased(KEY_D)){
    CAMERA::speed = CAMERA::base_speed;
  }

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) {
    if(IsKeyDown(KEY_LEFT_CONTROL)){
      float azimuth = atan2f(target_2d.y - position_2d.y, target_2d.x - position_2d.x);

      if (IsKeyDown(KEY_LEFT))  azimuth += CAMERA::rotation_speed * GetFrameTime();
      if (IsKeyDown(KEY_RIGHT)) azimuth -= CAMERA::rotation_speed * GetFrameTime();

      camera.target.x = camera.position.x + cosf(azimuth) * radius;
      camera.target.y = camera.position.y + sinf(azimuth) * radius;

    }
    else{
      float azimuth = atan2f(position_2d.y - target_2d.y, position_2d.x - target_2d.x);

      if (IsKeyDown(KEY_LEFT))  azimuth -= CAMERA::rotation_speed * GetFrameTime();
      if (IsKeyDown(KEY_RIGHT)) azimuth += CAMERA::rotation_speed * GetFrameTime();

      camera.position.x = camera.target.x + cosf(azimuth) * radius;
      camera.position.y = camera.target.y + sinf(azimuth) * radius;
    }
  }

  if(IsKeyDown(KEY_UP)){
    camera.position.z += CAMERA::speed;
    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }
  }
  if(IsKeyDown(KEY_DOWN)){
    camera.position.z -= CAMERA::speed;
    CAMERA::speed += CAMERA::acceleration;
    if(CAMERA::speed > CAMERA::max_speed){
      CAMERA::speed = CAMERA::max_speed;
    }
  }
  if(IsKeyReleased(KEY_UP) or IsKeyReleased(KEY_DOWN)){
    CAMERA::speed = CAMERA::base_speed;
  }

}

void Draw_2D_Grid(Camera2D camera) {
    const float spacing = GRID::spacing;

    // Get the world coordinates of the viewport corners
    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

    // Find the starting point by rounding the top-left coordinate DOWN to the nearest spacing
    // floorf(x / spacing) * spacing ensures we align perfectly to the grid globally
    float x_start = floorf(topLeft.x / spacing) * spacing;
    float y_start = floorf(topLeft.y / spacing) * spacing;

    // Opacity scaling based on zoom
    float alpha = Remap(camera.zoom, 0.3f, 2.0f, 0.1f, 1.0f);
    Color gridColor = Fade(GRAY, alpha);

    // Draw Vertical Lines
    for (float x = x_start; x <= bottomRight.x; x += spacing) {
        DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, gridColor);
        if(x == 0){
          DrawLineV({ x, topLeft.y }, { x, bottomRight.y }, DARKGRAY);
        }
    }

    // Draw Horizontal Lines
    for (float y = y_start; y <= bottomRight.y; y += spacing) {
        DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, gridColor);
        if(y == 0){
          DrawLineV({ topLeft.x, y }, { bottomRight.x, y }, DARKGRAY);
        }
    }
}


void Draw_Floor_Planning_Status_Bar(Font font, Camera2D camera) {
  float height = 20;
  DrawRectangle(0, GetScreenHeight()-height, GetScreenWidth(), GetScreenHeight(), Fade(LIGHTGRAY, 0.8));
  Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera);
  DrawTextEx(font, TextFormat("Mouse Position : {%.2f, %.2f}", pos.x/GRID::spacing, pos.y/GRID::spacing), {10, GetScreenHeight()-height+2}, 16, 1, DARKGRAY);
  DrawTextEx(font, TextFormat("Zoom : {%.1f}", camera.zoom), {(float)GetScreenWidth() - 96, GetScreenHeight()-height+2}, 16, 1, DARKGRAY);
}

Vector2 Get_Snapped_Mouse_Position(Camera2D camera){ // Needs Fixing 
  Vector2 position = GetScreenToWorld2D(GetMousePosition(), camera);

  float spacing = GRID::spacing;

  position.x = roundf(position.x / spacing) * spacing;
  position.y = roundf(position.y / spacing) * spacing;

  return position;
}



void Draw_Floor_Planning_Tool_Bar(Font font_32, Font font_20, Selected_Tool &selected_tool, Camera2D camera, View &view) {
  // Tool Bar Main Background
  DrawRectangle(0, 75, GetScreenWidth(), 2, Fade(GRAY, 0.5));
  DrawRectangle(0, 0, GetScreenWidth(), 75, { 240, 240, 240, 200 });

  if(view == View_2D){
    // Walls
    Rectangle Walls_Tool_Rec = {10, 26, 100, 40};
    if(selected_tool != Walls_Tool and CheckCollisionPointRec(GetMousePosition(), Walls_Tool_Rec)){
      DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, SKYBLUE);
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, Fade(WHITE, 0.2));
        selected_tool = Walls_Tool;
      }
    }
    else if(selected_tool == Walls_Tool){
      // the button
      DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, Fade(SKYBLUE, 0.6));
      if(CheckCollisionPointRec(GetMousePosition(), Walls_Tool_Rec)){
        DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, SKYBLUE);
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, Fade(WHITE, 0.2));
            selected_tool = None;
        }
      }

      // the mouse pointer
      if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), 75}) == false){
        Vector2 pos = Get_Snapped_Mouse_Position(camera);

        DrawCircleV(GetWorldToScreen2D(pos, camera), 5, BLUE);

        if(FP::is_starting_pos_available == true){
          DrawLineEx(GetWorldToScreen2D(FP::starting_pos, camera), GetWorldToScreen2D(pos, camera), 3, BLUE);

          // Wall Length Tool Tip
          Vector2 pos_screen = GetWorldToScreen2D(pos, camera);
          Vector2 wtt = {pos_screen.x+20, pos_screen.y-30}; // wtt = wall tool tip
          float length = Vector2Distance(FP::starting_pos, pos)/GRID::spacing;
          const char *text = TextFormat("Length = %.2f", length);
          Vector2 text_size = MeasureTextEx(font_20, text, 20, 1);
          float margin = 5;
          DrawRectangleRounded({wtt.x - margin, wtt.y - margin, text_size.x + margin*2, text_size.y + margin*2}, 0.5, 10, Fade(LIGHTGRAY, 0.5));

          DrawTextEx(font_20, text, wtt, 20, 1, BLACK);
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
          if(FP::is_starting_pos_available == true) {
            Wall_2D Wall;
            Wall.Start = FP::starting_pos;
            Wall.End = pos;
            FP::walls_2d.push_back(Wall);

            FP::starting_pos = pos;
          }
          else {
            FP::starting_pos = pos;
            FP::is_starting_pos_available = true;
          }
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
          FP::is_starting_pos_available = false;
        }

      }
    }
    else {
      DrawRectangleRounded(Walls_Tool_Rec, 0.5, 10, LIGHTGRAY);
    }
    DrawTextEx(font_32, "Walls", {20, 32}, 32, 1, DARKGRAY);
  }

  // Walls
  Rectangle View_3D_Option = {(float)GetScreenWidth() - 150 - 10, 26, 150, 40};

  DrawRectangleRounded(View_3D_Option, 0.5, 10, LIGHTGRAY);
  if(CheckCollisionPointRec(GetMousePosition(), View_3D_Option)){
    DrawRectangleRounded(View_3D_Option, 0.5, 10, SKYBLUE);
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      DrawRectangleRounded(View_3D_Option, 0.5, 10, Fade(WHITE, 0.2));
      selected_tool = None;
      if(view == View_2D){
        view = View_3D;
      }
      else if(view == View_3D){
        view = View_2D;
      }
    }
  }
  if(view == View_2D){
    DrawTextEx(font_32, "3D View", {View_3D_Option.x + 15, 32}, 32, 1, DARKGRAY);
  }
  else if(view == View_3D){
    DrawTextEx(font_32, "2D View", {View_3D_Option.x + 15, 32}, 32, 1, DARKGRAY);
  }
}

void Draw_Walls_2D(){
  for(int i = 0; i < FP::walls_2d.size(); i++){
    FP::walls_2d[i].Draw_2D();
  }
}
void Draw_Walls_3D(){
  rlDisableBackfaceCulling();

  for(int i = 0; i < FP::walls_2d.size(); i++){
    FP::walls_2d[i].Draw_3D();
  }

}

void Draw_Floor_Planning_State(Camera2D &camera2, Camera3D &camera3,Font font_32, Font font_20, State &state, Texture2D texture, bool imageLoaded, Selected_Tool &selected_tool, View &view, Shader shader){

  if(view == View_2D){
    Update_2D_Camera(camera2, state);

    BeginMode2D(camera2);

    Draw_2D_Grid(camera2);

    if(imageLoaded){
      DrawTexture(texture, 0, 0, Fade(WHITE, 0.5));
    }

    Draw_Walls_2D();

    EndMode2D();
  }
  else if(view == View_3D){

    Update_3D_Camera(camera3);

    BeginMode3D(camera3);
    BeginShaderMode(shader);

    Draw_Walls_3D();

    EndShaderMode();
    EndMode3D();
  }

  Draw_Floor_Planning_Tool_Bar(font_32, font_20, selected_tool, camera2, view);
  Draw_Floor_Planning_Status_Bar(font_32, camera2);
}

void Set_Cameras(Camera2D &camera2, Camera3D &camera3){
  camera3.position = {0, 5, 5};
  camera3.target = {0, 0, 0};
  camera3.up = {0, 0, 1};
  camera3.fovy = 45.0f;
  camera3.projection = CAMERA_PERSPECTIVE;

  camera2.offset = {0, 0};
  camera2.target = {0, 0};
  camera2.rotation = 0;
  camera2.zoom = 1.0f;
}

void Draw_Main_Menu_Bar(Texture2D &texture, bool &imageLoaded) {
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
}

void DrawExitConfirmationModal() {
  ImGui::OpenPopup("Exit Confirmation");

  // Centering the modal on the screen
  ImVec2 center = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Exit Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text("You have unsaved changes!\nDo you really want to quit?");
      ImGui::Separator();

      if (ImGui::Button("Yes, Quit", ImVec2(120, 0))) {
          Should_Exit_App = true;  // This breaks the main loop
          Show_Exit_Pop_Up = false;
      }

      ImGui::SameLine();

      if (ImGui::Button("Cancel", ImVec2(120, 0))) {
          Show_Exit_Pop_Up = false;     // This stops the function from opening the popup again
          ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
  }
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

  // Loading the Shader :
  Shader shader = LoadShader("shader.vert", "shader.frag");

  // Loading the font
  Font Montserrat_Font_32 = LoadFontEx("./fonts/Montserrat-Bold.ttf", 32, 0, 0);
  Font Montserrat_Font_20 = LoadFontEx("./fonts/Montserrat-Bold.ttf", 20, 0, 0);

  // Loading the Spash Screen Texture
  Texture2D Splash_Screen_Image = LoadTexture("Images/Crimson_Splash_Screen.png");
  bool Unload_Splash_Screen_Image = false;

  // Setting up ImGui
  rlImGuiSetup(false);
  ImGuiIO& io = ImGui::GetIO();
  ImFont* montserratImgui = io.Fonts->AddFontFromFileTTF("./fonts/Montserrat-Bold.ttf", 14.0f);

  ImGui::PushFont(montserratImgui);

  // For the potential floor plan image to be imported
  Texture2D Floor_Plan_Texture;
  bool Is_Floor_Plan_Image_Loaded = false;

  enum State state = Splash_Screen_State;
  enum Selected_Tool selected_tool = None;
  enum View view = View_2D;

  while (!Should_Exit_App) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    rlImGuiBegin();

    if(state == Splash_Screen_State){
      Draw_Floor_Planning_State(camera2, camera3, Montserrat_Font_32, Montserrat_Font_20, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded, selected_tool, view, shader);
      Draw_Splash_Screen(Splash_Screen_Image, Unload_Splash_Screen_Image, state, Montserrat_Font_32);
    }
    else if(state == Floor_Planning_State){
      Draw_Floor_Planning_State(camera2, camera3, Montserrat_Font_32, Montserrat_Font_20, state, Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded, selected_tool, view, shader);
    }

    Draw_Main_Menu_Bar(Floor_Plan_Texture, Is_Floor_Plan_Image_Loaded);

    if(WindowShouldClose() or IsKeyPressed(KEY_ESCAPE)){
      Show_Exit_Pop_Up = true;
    }

    if(Show_Exit_Pop_Up){
      DrawExitConfirmationModal();
    }

    rlImGuiEnd();
    EndDrawing();

    if(IsTextureValid(Splash_Screen_Image) && Unload_Splash_Screen_Image == true){
      UnloadTexture(Splash_Screen_Image);
      Splash_Screen_Image.id = 0;
    }
  }

  if(IsTextureValid(Splash_Screen_Image)){
    UnloadTexture(Splash_Screen_Image);
  }
  UnloadFont(Montserrat_Font_32);
  UnloadFont(Montserrat_Font_20);

  CloseWindow();

  return 0;
}
