#include "raylib.h"
#include "raymath.h"
#include <algorithm>

// From the Video : https://youtu.be/KHuI9bXZS74?si=2h0ZbzMvoj_7QBtT
// And this Video : https://youtu.be/LyQWZRfWotQ?si=DsBpO_zhN__4HOR-

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
    Vector2 Previous_Position;

    Point(Vector2 Position) {
      this->Position = Position;
      this->Previous_Position = Position;
    }

    void Draw() {
      DrawCircleV(Position, 5, RED);
      DrawCircleV(Previous_Position, 5, {100, 0, 0, 255});
    }

    float Distance_to_Line(Vector2 pos, Vector2 Line_Start, Vector2 Line_End, Vector2 Line_Normal){

      Vector2 Line_Start_to_Point = {pos.x - Line_Start.x, pos.y - Line_Start.y};

      Vector2 Line_Vector = {Line_End.x - Line_Start.x, Line_End.y - Line_Start.y};

      float t = std::clamp(Vector2DotProduct(Line_Start_to_Point, Line_Vector)/Vector2DotProduct(Line_Vector, Line_Vector), 0.0f, 1.0f);

      Vector2 Closest = Vector2Add(Line_Start, Vector2Scale(Line_Vector, t));

      DrawLineV(Position, Closest, YELLOW);

      float distance = Vector2Distance(pos, Closest);

      float crossVal = (Line_Vector.x * Line_Start_to_Point.y - Line_Vector.y * Line_Start_to_Point.x);  // 2D cross product (scalar)
      float sign = (crossVal < 0.0) ? -1.0 : 1.0;

      return sign * distance;
    }

    void HitDetection(Vector2 Line_Start, Vector2 Line_End, Vector2 Line_Normal) {

      Vector2 Line_Vector = {Line_End.x - Line_Start.x, Line_End.y - Line_Start.y};

      float d0 = Distance_to_Line(Previous_Position, Line_Start, Line_End, Line_Normal);
      float d1 = Distance_to_Line(Position, Line_Start, Line_End, Line_Normal);

      float t = d0 / (d0 - d1);
      Vector2 motion = {Position.x - Previous_Position.x, Position.y - Previous_Position.y};
      Vector2 hitPos = Vector2Add(Previous_Position, Vector2Scale(motion, t) );

      bool hit;

      float segParam = Vector2DotProduct({hitPos.x - Line_Start.x, hitPos.y - Line_Start.y}, Line_Vector) / Vector2DotProduct(Line_Vector, Line_Vector);
      if (segParam >= 0.0 && segParam <= 1.0 && d0*d1 < 0 or d1 == 0) {
          hit = true;   // within endpoints
          DrawText("HIT Detected", 10, 70, 25, RED);
      }
      else {
          hit = false;  // missed — passed above/below
      }

    }

};

int main(){

  Wall wall = Wall({600, 100}, {600, 500});
  Point point = Point({400, 300});

  InitWindow(800, 600, "Signed Distance");

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    if (IsKeyPressed(KEY_SPACE)){
      point.Previous_Position = point.Position;
      point.Position = Vector2Add(point.Position, {60, 0});
    }

    wall.Draw();
    point.Draw();

    DrawText(TextFormat("Distance = %f", point.Distance_to_Line(point.Position, wall.Start, wall.End, wall.Normal)), 10, 10, 25, BLUE);

    point.HitDetection(wall.Start, wall.End, wall.Normal);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
