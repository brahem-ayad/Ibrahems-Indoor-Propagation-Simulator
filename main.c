#include<raylib.h>
#include<rlgl.h>

int main() {

  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(350, 300, "Title");

  Camera3D camera_3d = { 0 };
  camera_3d.position = (Vector3){5, 5, 5};
  camera_3d.fovy = 45.f;
  camera_3d.projection = CAMERA_PERSPECTIVE;
  camera_3d.target = (Vector3){0, 0, 0};
  camera_3d.up = (Vector3){0, 0, 1};

  Color color = (Color){0, 255, 153, 255};

  int n = 20;

  float z = 2;
  float x = 1;
  float y = 1;

  float speed = 10;

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground((Color){0, 0, 0, 150});

    BeginMode3D(camera_3d);

    for(int i = 0; i < n+1; i++){
      rlPushMatrix();

      rlRotatef(x*(i+speed*GetTime()), 1, 0, 0);
      rlRotatef(y*(i+speed*GetTime()), 0, 1, 0);
      rlRotatef(z*(i+speed*GetTime()), 0, 0, 1);

      DrawCubeWires((Vector3){0, 0, 0}, 1 + (float)i/n, 1 + (float)i/n, 1 + (float)i/n, Fade(color, 0.1 + (float)i/n));

      rlPopMatrix();
    }

    EndMode3D();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

