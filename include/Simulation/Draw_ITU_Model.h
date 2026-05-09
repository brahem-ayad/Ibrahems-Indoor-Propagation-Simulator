#pragma once

#include<raylib.h>
#include<raymath.h>

#include"../Config.h"
#include"../Floor_Planning/Floor_Plan.h"
#include"./Generating_Corner_Rays.h"
#include"./Generate_Polygon_Visibility_Points.h"

static void Draw_ITU_Rays(Shader ITU_Shader, Camera3D camera3, int ITU_Shader_Uniform_LoS_ID) {
  // since the polygon is generated in real time, we need to clear these vectors each frame
  CONF::rays.clear();
  CONF::visibility_polygon.clear();
  CONF::invisibility_polygon.clear();

  // This part is to change the BS Position based on the mouse position
  Vector2 Gimbal_Position;
  if(CONF::tool_state == None) Gimbal_Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
  else Gimbal_Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};
  if(CheckCollisionPointCircle(GetMousePosition(), Gimbal_Position, 70) == false){
  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height}) == false){
  if(CheckCollisionPointRec(GetMousePosition(), CONF::SM_Mode_DDM_Rect) == false){

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
      Ray Camera_Mouse_Ray = GetScreenToWorldRay(GetMousePosition(), camera3);
      float height_difference = Camera_Mouse_Ray.position.z - CONF::BS_POS.z;

      float t = (CONF::BS_POS.z - Camera_Mouse_Ray.position.z)/(Camera_Mouse_Ray.direction.z);
      Vector2 New_BS_Pos;
      New_BS_Pos.x = Camera_Mouse_Ray.position.x + t * Camera_Mouse_Ray.direction.x;
      New_BS_Pos.y = Camera_Mouse_Ray.position.y + t * Camera_Mouse_Ray.direction.y;

      CONF::BS_POS.x = New_BS_Pos.x;
      CONF::BS_POS.y = New_BS_Pos.y;
    }

  }}}


  // Then we generate rays that go from the BS position to the edges of walls, doors, and windows
  std::vector<Vector3> points;

  for(int i = 0; i < FP::walls.size(); i++){

    // generating the rays to the walls edges
    Generate_Corner_Ray({FP::walls[i].Start.x/GRID::spacing, -FP::walls[i].Start.y/GRID::spacing, CONF::BS_POS.z});
    Generate_Corner_Ray({FP::walls[i].End.x/GRID::spacing, -FP::walls[i].End.y/GRID::spacing, CONF::BS_POS.z});

    Vector2 dir = Vector2Normalize(Vector2Subtract(FP::walls[i].End, FP::walls[i].Start));
    // generating the rays to the doors edges
    for(int j = 0; j < FP::walls[i].doors.size(); j++){
      if(FP::walls[i].doors[j].height >= CONF::BS_POS.z){
        Vector2 P1 = Vector2Subtract( { FP::walls[i].doors[j].Position.x/GRID::spacing, -FP::walls[i].doors[j].Position.y/GRID::spacing } , Vector2Scale(dir,  FP::walls[i].doors[j].width/2));
        Vector2 P2 = Vector2Subtract( { FP::walls[i].doors[j].Position.x/GRID::spacing, -FP::walls[i].doors[j].Position.y/GRID::spacing } , Vector2Scale(dir, -FP::walls[i].doors[j].width/2));

        Generate_Corner_Ray({P1.x, P1.y, CONF::BS_POS.z});
        Generate_Corner_Ray({P2.x, P2.y, CONF::BS_POS.z});
      }
    }

    // generating the rays to the windows edges
    for(int j = 0; j < FP::walls[i].windows.size(); j++){
      if(FP::walls[i].windows[j].base_height <= CONF::BS_POS.z and CONF::BS_POS.z <= FP::walls[i].windows[j].base_height + FP::walls[i].windows[j].window_height){
        Vector2 P1 = Vector2Subtract( { FP::walls[i].windows[j].Position.x/GRID::spacing, -FP::walls[i].windows[j].Position.y/GRID::spacing } , Vector2Scale(dir,  FP::walls[i].windows[j].width/2));
        Vector2 P2 = Vector2Subtract( { FP::walls[i].windows[j].Position.x/GRID::spacing, -FP::walls[i].windows[j].Position.y/GRID::spacing } , Vector2Scale(dir, -FP::walls[i].windows[j].width/2));

        Generate_Corner_Ray({P1.x, P1.y, CONF::BS_POS.z});
        Generate_Corner_Ray({P2.x, P2.y, CONF::BS_POS.z});
      }
    }

  }

  // generate the extra left/right rays and put them into a seperate place for now.
  std::vector<Ray> LR_Rays;
  for(int i = 0; i < CONF::rays.size(); i++){
    float LR_angle = 0.01;
    Ray LR_Ray;
    LR_Ray.position = CONF::BS_POS;
    LR_Ray.direction = { CONF::rays[i].direction.x * std::cosf(LR_angle) - CONF::rays[i].direction.y * std::sinf(LR_angle),
                         CONF::rays[i].direction.x * std::sinf(LR_angle) + CONF::rays[i].direction.y * std::cosf(LR_angle),
                         0};
    LR_Rays.push_back(LR_Ray);
    LR_Ray.direction = { CONF::rays[i].direction.x * std::cosf(-LR_angle) - CONF::rays[i].direction.y * std::sinf(-LR_angle),
                         CONF::rays[i].direction.x * std::sinf(-LR_angle) + CONF::rays[i].direction.y * std::cosf(-LR_angle),
                         0};
    LR_Rays.push_back(LR_Ray);
  }

  // sort the rays of the visibility polygon based on angle from BS position to add extra rays between them (interpolation)
  std::sort(CONF::rays.begin(), CONF::rays.end(), [](const Ray& a, const Ray& b) {
      return std::atan2(a.direction.y, a.direction.x) < std::atan2(b.direction.y, b.direction.x);
  });

  // Generate extra rays based on an angle difference
  float step_size = 0.01f;
  std::vector<Ray> extra_rays;
  // if there were no corner rays, then generate the rays from nothing
  if(CONF::rays.size() == 0){
    for(float i = 0; i < 2*PI; i += step_size){
      Ray ER;
      ER.position = CONF::BS_POS;
      ER.direction = {std::cosf(i), std::sinf(i), 0};
      extra_rays.push_back(ER);
    }
  }
  // if there are corner rays. then simple generate extra rays between them
  else{
    for(int i = 0; i < CONF::rays.size(); i++){
      int next_i = (i + 1) % CONF::rays.size();
      float angle_1 = std::atan2(CONF::rays[i].direction.y, CONF::rays[i].direction.x);
      float angle_2 = std::atan2(CONF::rays[next_i].direction.y, CONF::rays[next_i].direction.x);
      float delta_angle = angle_2 - angle_1;
      if(delta_angle < 0){
        delta_angle += 2.0f * PI;
      }
      for(float j = step_size; j < delta_angle - step_size/2; j += step_size){
        float current_angle = angle_1 + j;
        Ray ER;
        ER.position = CONF::BS_POS;
        ER.direction = {std::cosf(current_angle), std::sinf(current_angle), 0};
        extra_rays.push_back(ER);
      }
    }
  }

  Generate_Polygon_Visibility_Points(LR_Rays);
  Generate_Polygon_Visibility_Points(extra_rays);

  if(LR_Rays.size() > 0) CONF::rays.insert(CONF::rays.end(), LR_Rays.begin(), LR_Rays.end());
  if(extra_rays.size() > 0) CONF::rays.insert(CONF::rays.end(), extra_rays.begin(), extra_rays.end());

  // Draw the rays
  //for(int i = 0; i < CONF::rays.size(); i++){
  //  DrawRay(CONF::rays[i], RED);
  //}

  // Draw the points of the visibility_polygon
  //for(int i = 0; i < CONF::visibility_polygon.size(); i++){
  //  DrawSphere(CONF::visibility_polygon[i], 0.05, RED);
  //}

  // sort the points of the visibility polygon based on angle from BS position to prepare them for drawing
  std::sort(CONF::visibility_polygon.begin(), CONF::visibility_polygon.end(), 
    [&](const Vector3& a, const Vector3& b) {
        // Calculate relative coordinates to make the Base Station the origin (0,0)
        float angleA = std::atan2(a.y - CONF::BS_POS.y, a.x - CONF::BS_POS.x);
        float angleB = std::atan2(b.y - CONF::BS_POS.y, b.x - CONF::BS_POS.x);
        return angleA < angleB;
    });

  // the invisibility_polygon
  Generate_Polygon_Invisibility_Points(extra_rays);

  CONF::LOS = 0;
  SetShaderValue(ITU_Shader, ITU_Shader_Uniform_LoS_ID, &CONF::LOS, SHADER_UNIFORM_INT);
  BeginShaderMode(ITU_Shader);

  for(int i = 0; i < CONF::invisibility_polygon.size(); i++){

    if(i < CONF::invisibility_polygon.size() - 1) DrawTriangle3D( {CONF::BS_POS.x, CONF::BS_POS.y, CONF::BS_POS.z - 0.1f} , CONF::invisibility_polygon[i], CONF::invisibility_polygon[i+1], RED);
    else DrawTriangle3D({CONF::BS_POS.x, CONF::BS_POS.y, CONF::BS_POS.z - 0.1f}, CONF::invisibility_polygon[i], CONF::invisibility_polygon[0], RED);
  }

  EndShaderMode();

  // the visibility_polygon
  CONF::LOS = 1;
  SetShaderValue(ITU_Shader, ITU_Shader_Uniform_LoS_ID, &CONF::LOS, SHADER_UNIFORM_INT);

  // Draw the visibility polygon
  BeginShaderMode(ITU_Shader);

  for(int i = 0; i < CONF::visibility_polygon.size(); i++){

    if(i < CONF::visibility_polygon.size() - 1) DrawTriangle3D(CONF::BS_POS, CONF::visibility_polygon[i], CONF::visibility_polygon[i+1], RED);
    else DrawTriangle3D(CONF::BS_POS, CONF::visibility_polygon[i], CONF::visibility_polygon[0], RED);
  }

  EndShaderMode();

}
