#include "raylib.h"

#include "Point.h"
#include "Square.h"

int main() {

  Point point = Point({400, 300});
  Square square = Square({100, 100}, 100);

  InitWindow(800, 600, "Something");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    square.Draw();

    point.Draw();
    point.Update();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
