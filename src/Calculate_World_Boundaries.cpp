#include"../include/Calculate_World_Boundaries.hpp"

#include<raylib.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Config.hpp"

void Calculate_World_Boundaries() {
  // 1. Initialize bounds with extreme opposites
  float minX =  999999.0f;
  float maxX = -999999.0f;
  float minY =  999999.0f;
  float maxY = -999999.0f;

  // Handle an empty environment edge-case safely (No walls AND no transmitters)
  if (FP::Walls_Vec.empty() && FP::Transmitters.empty()) {
    CONF::World_Center = (Vector3){ 0.0f, 0.0f, 0.0f };
    CONF::World_Width = 50.0f;   // Fallback defaults
    CONF::World_Height = 25.0f;
    return;
  }

  // 2. Loop through every wall and check both its start and end coordinates
  for (const auto& wall : FP::Walls_Vec) {
    // --- Check Wall Start Point ---
    if (wall.Wall_Start_3D.x < minX) minX = wall.Wall_Start_3D.x;
    if (wall.Wall_Start_3D.x > maxX) maxX = wall.Wall_Start_3D.x;
    if (wall.Wall_Start_3D.y < minY) minY = wall.Wall_Start_3D.y;
    if (wall.Wall_Start_3D.y > maxY) maxY = wall.Wall_Start_3D.y;

    // --- Check Wall End Point ---
    if (wall.Wall_End_3D.x < minX) minX = wall.Wall_End_3D.x;
    if (wall.Wall_End_3D.x > maxX) maxX = wall.Wall_End_3D.x;
    if (wall.Wall_End_3D.y < minY) minY = wall.Wall_End_3D.y;
    if (wall.Wall_End_3D.y > maxY) maxY = wall.Wall_End_3D.y;
  }

  // 3. Loop through every transmitter position
  for (const auto& tx : FP::Transmitters) {
    if (tx.Position.x < minX) minX = tx.Position.x;
    if (tx.Position.x > maxX) maxX = tx.Position.x;
    if (tx.Position.y < minY) minY = tx.Position.y;
    if (tx.Position.y > maxY) maxY = tx.Position.y;
  }

  // 4. Calculate total width and height of the world
  CONF::World_Width = maxX - minX;
  CONF::World_Height = maxY - minY;

  // 5. Add a tiny padding margin (e.g., 5%) so your outer elements 
  // aren't hugging the absolute edge of the baked texture pixels.
  float padding = CONF::ITU_Simulation_Area_Padding;
  CONF::World_Width += padding * 2.0f;
  CONF::World_Height += padding * 2.0f;

  // Recalculate minimums with padding taken into account
  minX -= padding;
  minY -= padding;

  // 6. Calculate the exact central point
  CONF::World_Center.x = minX + (CONF::World_Width / 2.0f);
  CONF::World_Center.y = minY + (CONF::World_Height / 2.0f);
  CONF::World_Center.z = 0.0f; // Kept at zero since we map on a flat plane
}
