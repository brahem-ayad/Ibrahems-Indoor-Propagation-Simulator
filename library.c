#include "common.h"
#include <raylib.h>

// We use "extern" to make sure the symbol is visible to dlsym
void game_update(GameContext *ctx) {
  if (!ctx->initialized) {
    ctx->camera.position = (Vector3){2, 2, 3};
    ctx->camera.up = (Vector3){0, 0, 1};
    ctx->camera.target = (Vector3){0, 0, 0};
    ctx->camera.fovy = 45.0f;
    ctx->camera.projection = CAMERA_PERSPECTIVE;

    ctx->initialized = true;
  }

  // Logic

  UpdateCamera(&ctx->camera, CAMERA_ORBITAL);

  // Render
  BeginDrawing();
  ClearBackground(BLACK);
  BeginMode3D(ctx->camera);

  DrawCubeWires((Vector3){0, 0, 0}, 1, 1, 1, RED);

  EndMode3D();

  DrawFPS(10, 10);
  DrawText("HOT RELOAD: ONLINE", 10, 30, 20, DARKGREEN);
  EndDrawing();
}
