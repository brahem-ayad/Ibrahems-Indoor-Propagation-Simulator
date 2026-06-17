#include"../include/Cameras.hpp"

#include"../include/Config.hpp"

#include<raylib.h>
#include<raymath.h>
#include<cmath>

#define SQRT2 1.41421356237
#define PHI_EPSILON 0.01f

void Set_Cameras(){
  CONF::camera2.target = {0, 0};
  CONF::camera2.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
  CONF::camera2.rotation = 0;
  CONF::camera2.zoom = 1.0f;

  CONF::camera3.target = {0, 0, 0};
  CONF::camera3.up = {0, 0, 1};
  CONF::camera3.position = {0, -10, 10};
  CONF::camera3.fovy = 45.0f;
  CONF::camera3.projection = CAMERA_PERSPECTIVE;

  CONF::camerafps.target = {0, 0, 0};
  CONF::camerafps.up = {0, 0, 1};
  CONF::camerafps.position = {0, -10, 10};
  CONF::camerafps.fovy = 45.0f;
  CONF::camerafps.projection = CAMERA_PERSPECTIVE;

  CONF::Visibility_Polygon_Camera.projection = CAMERA_ORTHOGRAPHIC;
  CONF::Visibility_Polygon_Camera.target = {0, 0, 0};
  CONF::Visibility_Polygon_Camera.position = {0, 0, 10};
  CONF::Visibility_Polygon_Camera.up = {0, 1, 0};
  CONF::Visibility_Polygon_Camera.fovy = 10;
}

void Update_2D_Camera(){
  // if the window is resized, we will only see a part of the viewport not all of it. And I don't want to see the left or right part
  // I want to see the middle/center part of the old viewport so I wrote this:
  if(IsWindowResized()){
    CONF::camera2.offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
  }

  float speed = 3;

  float max_zoom = 0.1f;
  float min_zoom = 7.0f;

  if(IsKeyDown(KEY_Q)){
    CONF::camera2.zoom /= 1.01f;
  }
  if(IsKeyDown(KEY_E)){
    CONF::camera2.zoom *= 1.01f;
  }

  // Zoom based on mouse wheel
  float wheel = GetMouseWheelMove();
  if (wheel != 0 and (GetMousePosition().x > 138 or !CONF::Show_Tools_Bar) )
  {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), CONF::camera2);

      // Set the offset to where the mouse is
      CONF::camera2.offset = GetMousePosition();

      // Set the target to match, so that the CONF::camera2 maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      CONF::camera2.target = mouseWorldPos;

      // Zoom increment
      // Uses log scaling to provide consistent zoom speed
      float scale = 0.2f*wheel;
      CONF::camera2.zoom = Clamp(expf(logf(CONF::camera2.zoom)+scale), 0.125f, 64.0f);
  }
  if(CONF::camera2.zoom < max_zoom){
    CONF::camera2.zoom = max_zoom;
  }
  if(CONF::camera2.zoom > min_zoom){
    CONF::camera2.zoom = min_zoom;
  }

  if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) and IsKeyDown(KEY_SPACE)){
    SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
    CONF::Moving_2d = true;
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f/CONF::camera2.zoom);
    CONF::camera2.target = Vector2Add(CONF::camera2.target, delta);
  }
  else{
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    CONF::Moving_2d = false;
  }

  if(IsKeyDown(KEY_LEFT)){
    CONF::camera2.target.x -= speed;
  }
  if(IsKeyDown(KEY_RIGHT)){
    CONF::camera2.target.x += speed;
  }
  if(IsKeyDown(KEY_UP)){
    CONF::camera2.target.y -= speed;
  }
  if(IsKeyDown(KEY_DOWN)){
    CONF::camera2.target.y += speed;
  }

}

void Rotate_3D_Camera_Around_Target_UP() {
  float dx = CONF::camera3.position.x - CONF::camera3.target.x;
  float dy = CONF::camera3.position.y - CONF::camera3.target.y;
  float dz = CONF::camera3.position.z - CONF::camera3.target.z;
 
  float radius = std::sqrt( dx*dx + dy*dy + dz*dz );
  float phi = std::acos(dz / radius) - CAMERA::rotation_speed * GetFrameTime();
  float theta = std::atan2(dy, dx);
 
  if (phi < PHI_EPSILON) phi = PHI_EPSILON;
 
  CONF::camera3.position.x = CONF::camera3.target.x + radius * std::sin(phi) * std::cos(theta);
  CONF::camera3.position.y = CONF::camera3.target.y + radius * std::sin(phi) * std::sin(theta);
  CONF::camera3.position.z = CONF::camera3.target.z + radius * std::cos(phi);
}

void Rotate_3D_Camera_Around_Target_DOWN(){
  float dx = CONF::camera3.position.x - CONF::camera3.target.x;
  float dy = CONF::camera3.position.y - CONF::camera3.target.y;
  float dz = CONF::camera3.position.z - CONF::camera3.target.z;

  float radius = std::sqrt( dx*dx + dy*dy + dz*dz );
  float phi = std::acos(dz / radius) + CAMERA::rotation_speed * GetFrameTime();
  float theta = std::atan2(dy, dx);

  if (phi > PI - PHI_EPSILON) phi = PI - PHI_EPSILON;

  CONF::camera3.position.x = CONF::camera3.target.x + radius * std::sin(phi) * std::cos(theta);
  CONF::camera3.position.y = CONF::camera3.target.y + radius * std::sin(phi) * std::sin(theta);
  CONF::camera3.position.z = CONF::camera3.target.z + radius * std::cos(phi);
}

void Rotate_3D_Camera_Around_Target_RIGHT() {
  float dy = CONF::camera3.position.y - CONF::camera3.target.y;
  float dx = CONF::camera3.position.x - CONF::camera3.target.x;
  float angle = std::atan2(dy, dx) + CAMERA::rotation_speed * GetFrameTime();
  float distance = std::sqrt(std::pow(CONF::camera3.position.x - CONF::camera3.target.x, 2) + std::pow(CONF::camera3.position.y - CONF::camera3.target.y, 2));
  CONF::camera3.position.x = CONF::camera3.target.x + distance * std::cos(angle);
  CONF::camera3.position.y = CONF::camera3.target.y + distance * std::sin(angle);
}

void Rotate_3D_Camera_Around_Target_LEFT() {
  float dy = CONF::camera3.position.y - CONF::camera3.target.y;
  float dx = CONF::camera3.position.x - CONF::camera3.target.x;
  float angle = std::atan2(dy, dx) - CAMERA::rotation_speed * GetFrameTime();
  float distance = std::sqrt(std::pow(CONF::camera3.position.x - CONF::camera3.target.x, 2) + std::pow(CONF::camera3.position.y - CONF::camera3.target.y, 2));
  CONF::camera3.position.x = CONF::camera3.target.x + distance * std::cos(angle);
  CONF::camera3.position.y = CONF::camera3.target.y + distance * std::sin(angle);
}

void Zoom_3D_Camera_Out(){
  float radius = Vector3Distance(CONF::camera3.position, CONF::camera3.target);
  if(radius < 150){
    Vector3 direction = Vector3Normalize(Vector3Subtract(CONF::camera3.position, CONF::camera3.target));
    CONF::camera3.position = Vector3Add(CONF::camera3.position, Vector3Scale(direction, CAMERA::zoom_speed * GetFrameTime()));
  }
}

void Zoom_3D_Camera_In(){
  float radius = Vector3Distance(CONF::camera3.position, CONF::camera3.target);
  if(radius > 1){
    Vector3 direction = Vector3Normalize(Vector3Subtract(CONF::camera3.target, CONF::camera3.position));
    CONF::camera3.position = Vector3Add(CONF::camera3.position, Vector3Scale(direction, CAMERA::zoom_speed * GetFrameTime()));
  }
}

void Move_3D_Camera_UP(){
  Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
  Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime());
 
  CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector);
  CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector);
}

void Move_3D_Camera_DOWN() {
  Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
  Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime());
 
  CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector);
  CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector);
}

void Move_3D_Camera_LEFT() {
  Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
  Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector2 perp = {camera_to_target.y, -camera_to_target.x};
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime());
 
  CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector);
  CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector);
}

void Move_3D_Camera_RIGHT() {
  Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
  Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
  Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
  Vector2 perp = {camera_to_target.y, -camera_to_target.x};
  Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime());
 
  CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector);
  CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector);
}

void Update_3D_Camera(){

  if(IsKeyDown(KEY_PAGE_UP)) CAMERA::Perspective_fovy += 0.5;
  if(IsKeyDown(KEY_PAGE_DOWN)) CAMERA::Perspective_fovy -= 0.5;

  if(CONF::Camera_3D_Projection == Perspective){
    CONF::camera3.projection = CAMERA_PERSPECTIVE;
    CONF::camera3.fovy = CAMERA::Perspective_fovy;
  }
  else{
    CONF::camera3.projection = CAMERA_ORTHOGRAPHIC;
    CONF::camera3.fovy = CAMERA::Orthographic_fovy;
  }

  // lateral movement
  if(IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN)) Move_3D_Camera_LEFT();
  if(IsKeyDown(KEY_RIGHT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN)) Move_3D_Camera_RIGHT();
  if(IsKeyDown(KEY_DOWN) and !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT)) Move_3D_Camera_DOWN();
  if(IsKeyDown(KEY_UP) and !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT)) Move_3D_Camera_UP();

  // diagonal movement
  if(IsKeyDown(KEY_LEFT) and IsKeyDown(KEY_UP)){
    Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
    Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector);
    CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector);

    CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector_2);
    CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_LEFT) and IsKeyDown(KEY_DOWN)){
    Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
    Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector);
    CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector);

    CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector_2);
    CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_UP)){
    Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
    Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector);
    CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector);

    CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector_2);
    CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector_2);
  }
  if(IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_DOWN)){
    Vector2 camera_pos_2d = {CONF::camera3.position.x, CONF::camera3.position.y};
    Vector2 camera_targ_2d = {CONF::camera3.target.x, CONF::camera3.target.y};
    Vector2 camera_to_target = Vector2Subtract(camera_pos_2d, camera_targ_2d);
    Vector2 perp = {camera_to_target.y, -camera_to_target.x};
    Vector3 direction_vector = Vector3Scale(Vector3Normalize({perp.x, perp.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);
    Vector3 direction_vector_2 = Vector3Scale(Vector3Normalize({camera_to_target.x, camera_to_target.y, 0}), CAMERA::speed * GetFrameTime() / SQRT2);

    CONF::camera3.target = Vector3Subtract(CONF::camera3.target, direction_vector);
    CONF::camera3.position = Vector3Subtract(CONF::camera3.position, direction_vector);

    CONF::camera3.target = Vector3Add(CONF::camera3.target, direction_vector_2);
    CONF::camera3.position = Vector3Add(CONF::camera3.position, direction_vector_2);
  }

  // speed control
  if( IsKeyDown(KEY_LEFT) or IsKeyDown(KEY_RIGHT) or IsKeyDown(KEY_UP) or IsKeyDown(KEY_DOWN) ) CAMERA::is_moving = true;
  if( CAMERA::is_moving ){ CAMERA::speed += CAMERA::acceleration; }
  else{ CAMERA::speed = CAMERA::base_speed; }
  if(CAMERA::speed > CAMERA::max_speed){ CAMERA::speed = CAMERA::max_speed; }
  if( !IsKeyDown(KEY_LEFT) and !IsKeyDown(KEY_RIGHT) and !IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN) ) CAMERA::is_moving = false;

  // Rotation
  if(IsKeyDown(KEY_D)) Rotate_3D_Camera_Around_Target_RIGHT();
  if(IsKeyDown(KEY_A)) Rotate_3D_Camera_Around_Target_LEFT();
  if(IsKeyDown(KEY_W)) Rotate_3D_Camera_Around_Target_UP();
  if(IsKeyDown(KEY_S)) Rotate_3D_Camera_Around_Target_DOWN();

  // Zoom
  if(IsKeyDown(KEY_Q)) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_Out();
    else CAMERA::Orthographic_fovy += CAMERA::zoom_speed * GetFrameTime();
  }
  if(IsKeyDown(KEY_E)) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_In();
    else CAMERA::Orthographic_fovy -= CAMERA::zoom_speed * GetFrameTime();
  }
  if(GetMouseWheelMove() < 0) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_Out();
    else CAMERA::Orthographic_fovy += CAMERA::zoom_speed * GetFrameTime();
  }
  if(GetMouseWheelMove() > 0) {
    if(CONF::Camera_3D_Projection == Perspective) Zoom_3D_Camera_In();
    else CAMERA::Orthographic_fovy -= CAMERA::zoom_speed * GetFrameTime();
  }


  // Going Up and Down
  if(IsKeyDown(KEY_SPACE)){
    CONF::camera3.target.z += 1.0f * GetFrameTime();
    CONF::camera3.position.z += 1.0f * GetFrameTime();
  }
  if(IsKeyDown(KEY_LEFT_CONTROL)){
    CONF::camera3.target.z -= 1.0f * GetFrameTime();
    CONF::camera3.position.z -= 1.0f * GetFrameTime();
  }

}



//#define GRAVITY         32.0f
//#define MAX_SPEED        5.0f
//#define CROUCH_SPEED     3.0f
//#define JUMP_FORCE      10.0f
//#define MAX_ACCEL      100.0f
//#define FRICTION        0.86f
//#define AIR_DRAG        0.98f
//#define CONTROL         15.0f
//#define CROUCH_HEIGHT   0.85f
//#define STAND_HEIGHT     1.4f
//#define BOTTOM_HEIGHT    0.0f
//
//#define NORMALIZE_INPUT  1
//
//static Vector2 sensitivity = { 0.003f, 0.003f };
//
//typedef struct {
//    Vector3 position;
//    Vector3 velocity;
//    Vector3 dir;
//    bool isGrounded;
//} Body;
//
//static Body player = { 0 };
//static Vector2 lookRotation = { 0 };
//static float headTimer = 0.0f;
//static float walkLerp = 0.0f;
//static float headLerp = STAND_HEIGHT;
//static Vector2 lean = { 0 };
//
//static void UpdateBody(Body *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold)
//{
//    Vector2 input = (Vector2){ (float)side, (float)forward }; // Forward is positive Y now
//
//#if defined(NORMALIZE_INPUT)
//    if ((side != 0) && (forward != 0)) input = Vector2Normalize(input);
//#endif
//
//    float delta = GetFrameTime();
//
//    // Gravity acts on Z
//    if (!body->isGrounded) body->velocity.z -= GRAVITY * delta;
//
//    if (body->isGrounded && jumpPressed)
//    {
//        body->velocity.z = JUMP_FORCE;
//        body->isGrounded = false;
//    }
//
//    // Direction vectors for Z-up (Rotation around Z axis)
//    // Front is Y+, Right is X+
//    Vector3 front = (Vector3){ -sin(rot), cos(rot), 0.f };
//    Vector3 right = (Vector3){ cos(rot), sin(rot), 0.f };
//
//    Vector3 desiredDir = (Vector3){ 
//        input.x * right.x + input.y * front.x, 
//        input.x * right.y + input.y * front.y, 
//        0.0f 
//    };
// 
//    body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * delta);
//
//    float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
//    // Horizontal velocity is X and Y
//    Vector3 hvel = (Vector3){ body->velocity.x * decel, body->velocity.y * decel, 0.0f };
//
//    float hvelLength = Vector3Length(hvel);
//    if (hvelLength < (MAX_SPEED * 0.01f)) hvel = (Vector3){ 0 };
//
//    float speed = Vector3DotProduct(hvel, body->dir);
//    float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
//    float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);
// 
//    hvel.x += body->dir.x * accel;
//    hvel.y += body->dir.y * accel;
//
//    body->velocity.x = hvel.x;
//    body->velocity.y = hvel.y;
//
//    // Apply velocities
//    body->position.x += body->velocity.x * delta;
//    body->position.y += body->velocity.y * delta;
//    body->position.z += body->velocity.z * delta;
//
//    // Floor Collision
//    if (body->position.z <= 0.0f)
//    {
//        body->position.z = 0.0f;
//        body->velocity.z = 0.0f;
//        body->isGrounded = true;
//    }
//}
//
//static void Update_FPS_Camera(Camera3D &camerafps) {
//
//    // check that the mouse position is not at the top bar.
//  if(CheckCollisionPointRec(GetMousePosition(), {0, 0, (float)GetScreenWidth(), CONF::MMB_height + CONF::Tool_Bar_height}) == false){
//  Vector2 Position;
//  if(CONF::tool_state == None) Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + 15 + 50};
//  else Position = {(float)GetScreenWidth() - 70, CONF::MMB_height + CONF::Tool_Bar_height + CONF::Tool_Options_Bar_height + 15 + 50};
//  if(CheckCollisionPointCircle(GetMousePosition(), Position, 70) == false){ // and not at the gimbal
//
//
//  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
//    if(CONF::FPS_View_Started == false){
//      DisableCursor();
//      CONF::FPS_View_Started = true;
//    }
//  }
//
//  if(CONF::FPS_View_Started){
//    Vector2 mouseDelta = GetMouseDelta();
//    lookRotation.x -= mouseDelta.x * sensitivity.x;
//    lookRotation.y += mouseDelta.y * sensitivity.y;
//
//    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
//    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
//    bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
// 
//    UpdateBody(&player, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching);
//
//    float delta = GetFrameTime();
//    headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f * delta);
// 
//    // Position: Height is Z
//    camerafps.position = (Vector3){
//        player.position.x,
//        player.position.y,
//        player.position.z + (BOTTOM_HEIGHT + headLerp),
//    };
//
//  if(IsKeyDown(KEY_PAGE_UP)) CAMERA::fps_fovy += 0.5;
//  if(IsKeyDown(KEY_PAGE_DOWN)) CAMERA::fps_fovy -= 0.5;
//
//    if (player.isGrounded && ((forward != 0) || (sideway != 0))) {
//        headTimer += delta * 3.0f;
//        walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
//        camerafps.fovy = Lerp(camerafps.fovy, CAMERA::fps_fovy + 5.0f, 5.0f * delta);
//    } else {
//        walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
//        camerafps.fovy = Lerp(camerafps.fovy, CAMERA::fps_fovy, 5.0f * delta);
//    }
//
//    lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
//    lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);
//
//    // Z-UP World Setup
//    const Vector3 up = (Vector3){ 0.0f, 0.0f, 1.0f };
//    const Vector3 targetOffset = (Vector3){ 0.0f, 1.0f, 0.0f }; // Forward is Y+
//
//    // Yaw (Around Z)
//    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);
//
//    // Pitch axis (Right vector)
//    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));
//
//    // Pitch rotation (Up and Down)
//    float pitchAngle = -lookRotation.y - lean.y;
//    pitchAngle = Clamp(pitchAngle, -PI/2 + 0.01f, PI/2 - 0.01f);
//    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);
//
//    //// Update Camera Up (for camera tilting/rolling effects)
//    //float headSin = sin(headTimer * PI);
//    //float headCos = cos(headTimer * PI);
//    //const float stepRotation = 0.01f;
//    //camerafps.up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);
//
//    //// Camera Bobbing logic for Z-up
//    //const float bobSide = 0.0f;
//    //const float bobUp = 0.0f;
// 
//    //Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
//    //// Vertical bobbing added to Z
//    //bobbing.z = fabsf(headCos * bobUp);
//
//    //camerafps.position = Vector3Add(camerafps.position, Vector3Scale(bobbing, walkLerp));
//
//    camerafps.up = (Vector3){ 0.0f, 0.0f, 1.0f };
//
//    camerafps.position = (Vector3){
//        player.position.x,
//        player.position.y,
//        player.position.z + (BOTTOM_HEIGHT + (crouching ? CROUCH_HEIGHT : STAND_HEIGHT)),
//    };
//
//    camerafps.target = Vector3Add(camerafps.position, pitch);
//
//  }
//
//  if(IsKeyPressed(KEY_ESCAPE)){
//    CONF::FPS_View_Started = false;
//    EnableCursor();
//  }
//
//  }
//  }
//
//}
