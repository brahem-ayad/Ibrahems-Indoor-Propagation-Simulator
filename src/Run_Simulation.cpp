#include"../include/Run_Simulation.hpp"

#include<vector>
#include"../include/Generate_Wall_Collision_Quads.hpp"
#include"../include/Config.hpp"
#include"../include/Floor_Plan.hpp"
#include"../include/Generate_Visibility_Polygon.hpp"

void Run_Simulation() {
  CONF::Wall_Quads_3D.clear();
  CONF::Wall_Quads_3D = Generate_Wall_Collision_Quads();

  Generate_Visibility_Polygon();

  int activeCount = FP::Transmitters.size(); 
  Vector3 shaderPositions[32];
  for (int i = 0; i < activeCount; i++) {
    shaderPositions[i] = FP::Transmitters[i].Position;
  }
  
  CONF::ITU_Sim_Started = true;
  
  SetShaderValue(CONF::ITU_NLOS_Shader_3D, CONF::ITU_NLOS_3D_Shader_Count_ID, &activeCount, SHADER_UNIFORM_INT);
  SetShaderValueV(CONF::ITU_NLOS_Shader_3D, CONF::ITU_NLOS_3D_Shader_Positions_ID, shaderPositions, SHADER_UNIFORM_VEC3, activeCount);
  
  SetShaderValue(CONF::ITU_LOS_Shader_3D, CONF::ITU_LOS_3D_Shader_Count_ID, &activeCount, SHADER_UNIFORM_INT);
  SetShaderValueV(CONF::ITU_LOS_Shader_3D, CONF::ITU_LOS_3D_Shader_Positions_ID, shaderPositions, SHADER_UNIFORM_VEC3, activeCount);
}

