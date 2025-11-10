#include "raylib.h"
#include "raymath.h"

// From the Video : https://youtu.be/KHuI9bXZS74?si=2h0ZbzMvoj_7QBtT

class Wall{
  public:
    Vector2 Start;
    Vector2 End;
    Vector2 Normal;

    Wall(Vector2 Start, Vector2 End){
      this->Start = Start;
      this->End = End;
      this->Normal = Vector2Normalize({Start.y - End.y, End.x - Start.x});
    }

    void Draw() {
      DrawLineV(Start, End, WHITE);
    }
};

class Point{
  public:
    Vector2 Position;

    Point(Vector2 Position) {
      this->Position = Position;
    }

    void Draw() {
      DrawCircleV(Position, 5, RED);
    }

    float Distance_to_Line(Vector2 Line_Start, Vector2 Line_Normal){

      Vector2 Line_Start_to_Point = {Position.x - Line_Start.x, Position.y - Line_Start.y};

      float distance = Vector2DotProduct(Line_Start_to_Point, Line_Normal);

      return distance;
    }
};

int main(){

  Wall wall = Wall({600, 100}, {600, 500});
  Point point = Point({400, 300});

  InitWindow(800, 600, "Signed Distance");

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    point.Position = GetMousePosition();

    wall.Draw();
    point.Draw();

    DrawText(TextFormat("Distance = %f", point.Distance_to_Line(wall.Start, wall.Normal)), 10, 10, 25, BLUE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
