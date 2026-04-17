#pragma once

#include<raylib.h>
#include<raymath.h>
#include<cmath>

#include"Config.h"

#define SQRT2 1.41421356237
const float PHI_EPSILON = 0.01f;

namespace CAMERA {
  static float speed = 1.0f;
  static float base_speed = 1.0f;
  static float max_speed = 10.0f;
  static float acceleration = 0.05f;
  static float rotation_speed = 1.0f;
  static float zoom_speed = 10.0f;
  static bool is_moving = false;
  static float Perspective_fovy = 45.0f;
  static float Orthographic_fovy = 20.0f;
};

static void Set_Cameras(Camera2D &camera2, Camera3D &camera3){
  camera2.target = {0, 0};
  camera2.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
  camera2.rotation = 0;
  camera2.zoom = 1.0f;

  camera3.target = {0, 0, 0};
  camera3.up = {0, 0, 1};
  camera3.position = {0, -10, 10};
  camera3.fovy = 45.0f;
  camera3.projection = CAMERA_PERSPECTIVE;
}

static void Update_2D_Camera(Camera2D &camera){
if(CONF::state == Floor_Planning_State){

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

  if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) and IsKeyDown(KEY_SPACE)){
    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
    CONF::Moving_2d = true;
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/camera.zoom);
    camera.target = Vector2Add(camera.target, delta);
  }
  else{
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    CONF::Moving_2d = false;
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


static void Rotate_3D_Camera_Around_Target_UP(Camera3D &camera3) {
  float dx = camera3.position.x - camera3.target.x;
  float dy = camera3.position.y - camera3.target.y;
  float dz = camera3.position.z - camera3.target.z;
 
  float radius = std::sqrt( dx*dx + dy*dy + dz*dz );
  float phi = std::acos(dz / radius) - CAMERA::rotation_speed * GetFrameTime();
  float theta = std::atan2(dy, dx);
 
  if (phi < PHI_EPSILON) phi = PHI_EPSILON;
 
  camera3.position.x = camera3.target.x + radius * std::sin(phi) * std::cos(theta);
  camera3.position.y = camera3.target.y + radius * std::sin(phi) * std::sin(theta);
  camera3.position.z = camera3.target.z + radius * std::cos(phi);
}

static void Rotate_3D_Camera_Around_Target_DOWN(Camera3D &camera3){
  float dx = camera3.position.x - camera3.target.x;
  float dy = camera3.position.y - camera3.target.y;
  float dz = camera3.position.z - camera3.target.z;

  float radius = std::sqrt( dx*dx + dy*dy + dz*dz );
  float phi = std::acos(dz / radius) + CAMERA::rotation_speed * GetFrameTime();
  float theta = std::atan2(dy, dx);

  if (phi > PI - PHI_EPSILON) phi = PI - PHI_EPSILON;

  camera3.position.x = camera3.target.x + radius * std::sin(phi) * std::cos(theta);
  camera3.position.y = camera3.target.y + radius * std::sin(phi) * std::sin(theta);
  camera3.position.z = camera3.target.z + radius * std::cos(phi);
}

static void Rotate_3D_Camera_Around_Target_RIGHT(Camera3D &camera3) {
  float dy = camera3.position.y - camera3.target.y;
  float dx = camera3.position.x - camera3.target.x;
  float angle = std::atan2(dy, dx) + CAMERA::rotation_speed * GetFrameTime();
  float distance = std::sqrt(std::pow(camera3.position.x - camera3.target.x, 2) + std::pow(camera3.position.y - camera3.target.y, 2));
  camera3.position.x = camera3.target.x + distance * std::cos(angle);
  camera3.position.y = camera3.target.y + distance * std::sin(angle);
}

static void Rotate_3D_Camera_Around_Target_LEFT(Camera3D &camera3) {
  float dy = camera3.position.y - camera3.target.y;
  float dx = camera3.position.x - camera3.target.x;
  float angle = std::atan2(dy, dx) - CAMERA::rotation_speed * GetFrameTime();
  float distance = std::sqrt(std::pow(camera3.position.x - camera3.target.x, 2) + std::pow(camera3.position.y - camera3.target.y, 2));
  camera3.position.x = camera3.target.x + distance * std::cos(angle);
  camera3.position.y = camera3.target.y + distance * std::sin(angle);
}

static void Zoom_3D_Camera_Out(Camera3D &camera3){
  float radius = Vector3Distance(camera3.position, camera3.target);
  if(radius < 150){
    Vector3 direction = Vector3Normalize(Vector3Subtract(camera3.position, camera3.target));
    camera3.position = Vector3Add(camera3.position, Vector3Scale(direction, CAMERA::zoom_speed * GetFrameTime()));
  }
}

static void Zoom_3D_Camera_In(Camera3D &camera3){
  float radius = Vector3Distance(camera3.position, camera3.target);
  if(radius > 1){
    Vector3 direction = Vector3Normalize(Vector3Subtract(camera3.target, camera3.position));
    camera3.position = Vector3Add(camera3.position, Vector3Scale(direction, CAMERA::zoom_speed * GetFrameTime()));
  }
}

static void Move_3D_Camera_UP(Camera3D &camera3){
  Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
  Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime());
 
  camera3.target = Vector3Subtract(camera3.target, direction_vector);
  camera3.position = Vector3Subtract(camera3.position, direction_vector);
}

static void Move_3D_Camera_DOWN(Camera3D &camera3) {
  Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
  Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime());
 
  camera3.target = Vector3Add(camera3.target, direction_vector);
  camera3.position = Vector3Add(camera3.position, direction_vector);
}

static void Move_3D_Camera_LEFT(Camera3D &camera3) {
  Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
  Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector2 perp = {camera_to_target.y, -camera_to_target.x};
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime());
 
  camera3.target = Vector3Add(camera3.target, direction_vector);
  camera3.position = Vector3Add(camera3.position, direction_vector);
}

static void Move_3D_Camera_RIGHT(Camera3D &camera3) {
  Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
  Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector2 perp = {camera_to_target.y, -camera_to_target.x};
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime());
 
  camera3.target = Vector3Subtract(camera3.target, direction_vector);
  camera3.position = Vector3Subtract(camera3.position, direction_vector);
}

static void Update_3D_Camera(Camera3D &camera3){

  if(IsKeyDown(KEY_PAGE_UP)) CAMERA::Perspective_fovy += 0.1;
  if(IsKeyDown(KEY_PAGE_DOWN)) CAMERA::Perspective_fovy -= 0.1;

  if(CONF::Camera_3D_Projection == Perspective){
    camera3.projection = CAMERA_PERSPECTIVE;
    camera3.fovy = CAMERA::Perspective_fovy;
  }
  else{
    camera3.projection = CAMERA_ORTHOGRAPHIC;
    camera3.fovy = CAMERA::Orthographic_fovy;
  }

  // lateral movement
  if(IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN)) Move_3D_Camera_LEFT(camera3);
  if(IsKeyDown(KEY_RIGHT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN)) Move_3D_Camera_RIGHT(camera3);
  if(IsKeyDown(KEY_DOWN) and !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT)) Move_3D_Camera_DOWN(camera3);
  if(IsKeyDown(KEY_UP) and !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT)) Move_3D_Camera_UP(camera3);

  // diagonal movement
  if(IsKeyDown(KEY_LEFT) and IsKeyDown(KEY_UP)){
    Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
    Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    camera3.target = Vector3Add(camera3.target, direction_vector);
    camera3.position = Vector3Add(camera3.position, direction_vector);

    camera3.target = Vector3Subtract(camera3.target, direction_vector_2);
    camera3.position = Vector3Subtract(camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_LEFT) and IsKeyDown(KEY_DOWN)){
    Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
    Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    camera3.target = Vector3Add(camera3.target, direction_vector);
    camera3.position = Vector3Add(camera3.position, direction_vector);

    camera3.target = Vector3Add(camera3.target, direction_vector_2);
    camera3.position = Vector3Add(camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_UP)){
    Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
    Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    camera3.target = Vector3Subtract(camera3.target, direction_vector);
    camera3.position = Vector3Subtract(camera3.position, direction_vector);

    camera3.target = Vector3Subtract(camera3.target, direction_vector_2);
    camera3.position = Vector3Subtract(camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_DOWN)){
    Vector2 camera_pos_2d = {camera3.position.x, camera3.position.y};
    Vector2 camera_targ_2d = {camera3.target.x, camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    camera3.target = Vector3Subtract(camera3.target, direction_vector);
    camera3.position = Vector3Subtract(camera3.position, direction_vector);

    camera3.target = Vector3Add(camera3.target, direction_vector_2);
    camera3.position = Vector3Add(camera3.position, direction_vector_2);
  }

  // speed control
  if( IsKeyDown(KEY_LEFT) or IsKeyDown(KEY_RIGHT) or IsKeyDown(KEY_UP) or IsKeyDown(KEY_DOWN) ) CAMERA::is_moving = true;
  if( CAMERA::is_moving ){ CAMERA::speed += CAMERA::acceleration; }
  else{ CAMERA::speed = CAMERA::base_speed; }
  if(CAMERA::speed > CAMERA::max_speed){ CAMERA::speed = CAMERA::max_speed; }
  if( !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN) ) CAMERA::is_moving = false;

  // Rotation
  if(IsKeyDown(KEY_D)) Rotate_3D_Camera_Around_Target_RIGHT(camera3);
  if(IsKeyDown(KEY_A)) Rotate_3D_Camera_Around_Target_LEFT(camera3);
  if(IsKeyDown(KEY_W)) Rotate_3D_Camera_Around_Target_UP(camera3);
  if(IsKeyDown(KEY_S)) Rotate_3D_Camera_Around_Target_DOWN(camera3);

  // Zoom
  if(IsKeyDown(KEY_Q)) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_Out(camera3);
    else CAMERA::Orthographic_fovy += CAMERA::zoom_speed * GetFrameTime();
  }
  if(IsKeyDown(KEY_E)) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_In(camera3);
    else CAMERA::Orthographic_fovy -= CAMERA::zoom_speed * GetFrameTime();
  }


  // Going Up and Down
  if(IsKeyDown(KEY_SPACE)){
    camera3.target.z += 1.0f * GetFrameTime();
    camera3.position.z += 1.0f * GetFrameTime();
  }
  if(IsKeyDown(KEY_LEFT_CONTROL)){
    camera3.target.z -= 1.0f * GetFrameTime();
    camera3.position.z -= 1.0f * GetFrameTime();
  }

}
