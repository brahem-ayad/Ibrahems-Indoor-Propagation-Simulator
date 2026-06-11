#include"../include/Draw_Transmitters.hpp"

#include<raylib.h>
#include<raylib.h>
#include"../include/Floor_Plan.hpp"
#include"../include/Config.hpp"

void Draw_Transmitters_2D(){
  for(int i = 0; i < FP::Transmitters.size(); i++){
    Vector2 T_Pos = {FP::Transmitters[i].Position.x*CONF::Grid_Spacing, -FP::Transmitters[i].Position.y*CONF::Grid_Spacing};
    Vector2 Image_Pos = {T_Pos.x - CONF::Transmitter_Image.width/2.f, T_Pos.y - CONF::Transmitter_Image.height/2.f};
    DrawTextureV(CONF::Transmitter_Image, Image_Pos, WHITE);
  }
}

void Draw_Transmitters_3D(){
  for(int i = 0; i < FP::Transmitters.size(); i++){
    DrawSphere(FP::Transmitters[i].Position, 0.2f, BLACK);
  }
}
