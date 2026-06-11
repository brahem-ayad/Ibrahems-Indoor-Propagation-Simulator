#pragma once

namespace CAMERA {
  inline float speed = 1.0f;
  inline float base_speed = 1.0f;
  inline float max_speed = 10.0f;
  inline float acceleration = 0.05f;
  inline float rotation_speed = 1.0f;
  inline float zoom_speed = 10.0f;
  inline bool is_moving = false;
  inline float Perspective_fovy = 45.0f;
  inline float Orthographic_fovy = 20.0f;
  inline float fps_fovy = 90.0f;
};

void Set_Cameras();

void Update_2D_Camera();

void Rotate_3D_Camera_Around_Target_UP();
void Rotate_3D_Camera_Around_Target_DOWN();
void Rotate_3D_Camera_Around_Target_RIGHT();
void Rotate_3D_Camera_Around_Target_LEFT();

void Zoom_3D_Camera_Out();
void Zoom_3D_Camera_In();

void Move_3D_Camera_UP();
void Move_3D_Camera_DOWN();
void Move_3D_Camera_LEFT();
void Move_3D_Camera_RIGHT();

void Update_3D_Camera();
