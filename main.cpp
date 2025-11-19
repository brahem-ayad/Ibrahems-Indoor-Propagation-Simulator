#include <iostream>
#include "raylib.h"
#include <vector>
#include <math.h>
#include <algorithm>

#include "Wall.h"
#include "Ray.h"
#include "Cell.h"

Vector2 GetSnappedMousePosition(Vector2 Line_Start_Temp) {
  if(GetMousePosition().x > Line_Start_Temp.x and GetMousePosition().y < Line_Start_Temp.y){ // Mosue is Top Right of the Point
    if(GetMousePosition().x - Line_Start_Temp.x < Line_Start_Temp.y - GetMousePosition().y){
      return {Line_Start_Temp.x, GetMousePosition().y};
    }
    else if(GetMousePosition().x - Line_Start_Temp.x > Line_Start_Temp.y - GetMousePosition().y){
      return {GetMousePosition().x, Line_Start_Temp.y};
    }
  }
  else if(GetMousePosition().x < Line_Start_Temp.x and GetMousePosition().y < Line_Start_Temp.y){ // Mosue is Top Left of the Point
    if(Line_Start_Temp.x - GetMousePosition().x < Line_Start_Temp.y - GetMousePosition().y){
      return {Line_Start_Temp.x, GetMousePosition().y};
    }
    else if(Line_Start_Temp.x - GetMousePosition().x > Line_Start_Temp.y - GetMousePosition().y){
      return {GetMousePosition().x, Line_Start_Temp.y};
    }
  }
  else if(GetMousePosition().x < Line_Start_Temp.x and GetMousePosition().y > Line_Start_Temp.y){ // Mosue is Bottom Left of the Point
    if(Line_Start_Temp.x - GetMousePosition().x < GetMousePosition().y - Line_Start_Temp.y) {
      return {Line_Start_Temp.x, GetMousePosition().y};
    }
    else if(Line_Start_Temp.x - GetMousePosition().x > GetMousePosition().y - Line_Start_Temp.y){
      return {GetMousePosition().x, Line_Start_Temp.y};
    }
  }
  else if(GetMousePosition().x > Line_Start_Temp.x and GetMousePosition().y > Line_Start_Temp.y){ // Mosue is Bottom Right of the Point
    if(GetMousePosition().x - Line_Start_Temp.x < GetMousePosition().y - Line_Start_Temp.y){
      return {Line_Start_Temp.x, GetMousePosition().y};
    }
    else if(GetMousePosition().x - Line_Start_Temp.x > GetMousePosition().y - Line_Start_Temp.y){
      return {GetMousePosition().x, Line_Start_Temp.y};
    }
  }
  return GetMousePosition();
}

int main() {

  std::vector<BRay> rays;

  float speed = 4;
  Vector2 Start_Pos = {400, 150};

  bool Draw_Lines = true;

  int number_of_rays = 360;
  //for(int i = 0; i < number_of_rays; i++){
  //  rays.push_back( BRay(Start_Pos, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, speed, 2.4e9, 100e-3/number_of_rays) );
  //}

  std::vector<Wall> walls;
  //walls.push_back( Wall({100, 100}, {700, 100}) );
  //walls.push_back( Wall({700, 100}, {700, 500}) );
  //walls.push_back( Wall({700, 500}, {250, 500}) );
  //walls.push_back( Wall({250, 500}, {250, 400}) );
  //walls.push_back( Wall({250, 400}, {600, 400}) );
  //walls.push_back( Wall({600, 400}, {600, 200}) );
  //walls.push_back( Wall({600, 200}, {200, 200}) );
  //walls.push_back( Wall({200, 200}, {200, 500}) );
  //walls.push_back( Wall({200, 500}, {100, 500}) );
  //walls.push_back( Wall({100, 500}, {100, 100}) );


  InitWindow(800, 600, "Indoor Propagation Simulator");
  SetTargetFPS(60);

  bool Drawing_Tool_Selected = false;
  bool Line_Start_Available = false;

  bool BS_Tool_Selected = false;

  bool IS_BS_Position_Selected = false;

  Vector2 Line_Start_Temp = {-50, -50};
  Vector2 Line_End_Temp = {-50, -50};

  Vector2 Temp_Next_Pos;

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRoundedLinesEx({10, 10, 100, 50}, 0.8, 10, 3, WHITE);
    DrawLineEx({30, 40}, {90, 30}, 5, WHITE);
    if(Drawing_Tool_Selected){
      DrawRectangleRounded({10, 10, 100, 50}, 0.8, 10, {0, 100, 255, 100});
      DrawRectangleRoundedLinesEx({10, 10, 100, 50}, 0.8, 10, 3, SKYBLUE);
      DrawLineEx({30, 40}, {90, 30}, 5, SKYBLUE);
    }

    DrawRectangleRoundedLinesEx({130, 10, 85, 50}, 0.8, 10, 3, WHITE);
    DrawCircleV({150, 35}, 5, WHITE);
    DrawText("BS", 165, 25, 25, WHITE);
    if(BS_Tool_Selected){
      DrawRectangleRounded({130, 10, 85, 50}, 0.8, 10, {255, 0, 0, 100});
      DrawRectangleRoundedLinesEx({130, 10, 85, 50}, 0.8, 10, 3, RED);
      DrawCircleV({150, 35}, 5, RED);
      DrawText("BS", 165, 25, 25, RED);
    }

    if( CheckCollisionPointRec(GetMousePosition(), {10, 10, 100, 50}) ){
      if(IsMouseButtonPressed(0) and Drawing_Tool_Selected == false and BS_Tool_Selected == false){
        Drawing_Tool_Selected = true;
      }
      else if (IsMouseButtonPressed(0) and Drawing_Tool_Selected == true) {
        Drawing_Tool_Selected = false;
      }
    }

    if( CheckCollisionPointRec(GetMousePosition(), {130, 10, 85, 50}) ){
      if(IsMouseButtonPressed(0) and BS_Tool_Selected == false and Drawing_Tool_Selected == false){
        BS_Tool_Selected = true;
      }
      else if (IsMouseButtonPressed(0) and BS_Tool_Selected == true) {
        BS_Tool_Selected = false;
      }
    }


    if(Drawing_Tool_Selected) {

      if(CheckCollisionPointRec(GetMousePosition(), {10, 10, 100, 50}) == false and CheckCollisionPointRec(GetMousePosition(), {130, 10, 85, 50}) == false and IsMouseButtonPressed(0) and Line_Start_Available == false){
        Line_Start_Temp = GetMousePosition();
        Line_Start_Available = true;
      }
      if(CheckCollisionPointRec(GetMousePosition(), {10, 10, 100, 50}) == false and IsMouseButtonPressed(0) and Line_Start_Available == true){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
          Line_End_Temp = GetSnappedMousePosition(Line_Start_Temp);
        }
        else{
          Line_End_Temp = GetMousePosition();
        }
        walls.push_back( ( Wall(Line_Start_Temp, Line_End_Temp) ) );
        Line_Start_Temp = Line_End_Temp;
      }

      DrawCircleV(Line_Start_Temp, 5, WHITE);
      DrawCircleV(Line_End_Temp, 5, WHITE);

      if(IsKeyDown(KEY_LEFT_SHIFT)){
        Temp_Next_Pos = GetSnappedMousePosition(Line_Start_Temp);
        DrawCircleV(GetSnappedMousePosition(Line_Start_Temp), 5, WHITE);
      }
      else{
        Temp_Next_Pos = GetMousePosition();
        DrawCircleV(GetMousePosition(), 5, WHITE);
      }

      if(Line_Start_Temp.x != -50 and Line_Start_Available == true){
        DrawLineV(Line_Start_Temp, Temp_Next_Pos, WHITE);
      }

      if(IsKeyPressed(KEY_ENTER)){
        Line_Start_Available = false;
      }

    }

    if(IsKeyPressed(KEY_D) and Draw_Lines == true){
      Draw_Lines = false;
    }
    else if(IsKeyPressed(KEY_D) and Draw_Lines == false){
      Draw_Lines = true;
    }


    if(BS_Tool_Selected and !CheckCollisionPointRec(GetMousePosition(), {130, 10, 85, 50}) and !CheckCollisionPointRec(GetMousePosition(), {10, 10, 100, 50}) ){
      DrawCircleV(GetMousePosition(), 3, RED);
      if(IsMouseButtonPressed(0)){
        Start_Pos = GetMousePosition();
        IS_BS_Position_Selected = true;
      }
      if(IS_BS_Position_Selected){
        DrawCircleV(Start_Pos, 5, RED);
      }
    }



    float dt = GetFrameTime() * 60;

    for (int i = 0; i < rays.size(); ++i) {
      rays[i].ResolveCollisions(walls, dt);
    }

    for (int i = 0; i < rays.size(); i++){
      rays[i].draw(Draw_Lines);
    }
    //for(int i = 0; i < cells.size(); i++){
    //  for(int j = 0; j < rays.size(); j++){
    //    cells[i].update(rays[j].Position, 2, rays[j].Power);
    //  }
    //  cells[i].draw();
    //}
    for (int i = 0; i < walls.size(); i++){
      walls[i].draw();
    }

    rays.erase(
    std::remove_if(rays.begin(), rays.end(),
        [](const BRay& ra){ return ra.color.r < 5; }),
    rays.end()
    );

    if(IsKeyPressed(KEY_SPACE)){
      for(int i = 0; i < number_of_rays; i ++){
        rays.push_back( BRay(Start_Pos, {sinf(i*6.28/number_of_rays), cosf(i*6.28/number_of_rays)}, speed, 2.4e9, 100e-3/number_of_rays) );
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
