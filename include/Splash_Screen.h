#pragma once

#include<raylib.h>
#include"Config.h"
#include"Utilities.h"

static void Draw_Splash_Screen(Texture2D Splash_Screen_Image_Light, Texture2D Splash_Screen_Image_Dark, bool &Unload_Splash_Screen_Image, State &state, Font font_32, Texture2D Sun_Icon, Texture2D Moon_Icon) {

  // Drawing the Splash Screen Texture
  if(CONF::Theme == Light_Theme) DrawTexture(Splash_Screen_Image_Light, GetScreenWidth()/2 - Splash_Screen_Image_Light.width/2, GetScreenHeight()/2 - Splash_Screen_Image_Light.height/2, WHITE);
  else DrawTexture(Splash_Screen_Image_Dark, GetScreenWidth()/2 - Splash_Screen_Image_Dark.width/2, GetScreenHeight()/2 - Splash_Screen_Image_Dark.height/2, WHITE);

  // Checking if one of the buttons is pressed

  float offset = 50;
  float vertical_center = (float)GetScreenHeight()/2 + offset;

  float padding = 10;

  char* text1 = (char*)"Create a New Empty File";
  char* text2 = (char*)"Open an Existing File";
  char* text3 = (char*)"Load an Example";

  Vector2 text1_size = MeasureTextEx(font_32, text1, 32, 1);
  Vector2 text1_pos = { (float)GetScreenWidth()/2 - text1_size.x/2, vertical_center};

  Vector2 text2_size = MeasureTextEx(font_32, text2, 32, 1);
  Vector2 text2_pos = { (float)GetScreenWidth()/2 - text2_size.x/2, vertical_center + 65};

  Vector2 text3_size = MeasureTextEx(font_32, text3, 32, 1);
  Vector2 text3_pos = { (float)GetScreenWidth()/2 - text3_size.x/2, vertical_center + 65 * 2};

  Rectangle Text_Rec_1 = { text1_pos.x - padding, text1_pos.y - padding/2, text1_size.x + padding*2, text1_size.y + padding };
  Rectangle Text_Rec_2 = { text1_pos.x - padding, text2_pos.y - padding/2, text1_size.x + padding*2, text2_size.y + padding };
  Rectangle Text_Rec_3 = { text1_pos.x - padding, text3_pos.y - padding/2, text1_size.x + padding*2, text3_size.y + padding };

  Color Border_Color;
  Color Background_Color;
  Color Text_Color;
  if(CONF::Theme == Light_Theme){
    Border_Color = {45, 45, 45, 255};
    Background_Color = {219, 219, 219, 255};
    Text_Color = Border_Color;
  }
  else{
    Border_Color = {27, 27, 27, 255};
    Background_Color = {12, 12, 12, 255};
    Text_Color = {185, 185, 185, 255};
  }

  DrawRectangleRounded(Text_Rec_1, 0.5, 10, Background_Color);
  DrawRectangleRoundedLinesEx(Text_Rec_1, 0.5, 10, 3, Border_Color);
  DrawTextEx(font_32, text1, text1_pos, 32, 1, Text_Color);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_1)){
    DrawRectangleRounded(Text_Rec_1, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_1, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      state = Floor_Planning_State;
      Unload_Splash_Screen_Image = true;
    }
  }

  DrawRectangleRounded(Text_Rec_2, 0.5, 10, Background_Color);
  DrawRectangleRoundedLinesEx(Text_Rec_2, 0.5, 10, 3, Border_Color);
  DrawTextEx(font_32, text2, text2_pos, 32, 1, Text_Color);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_2)){
    DrawRectangleRounded(Text_Rec_2, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_2, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    }
  }

  DrawRectangleRounded(Text_Rec_3, 0.5, 10, Background_Color);
  DrawRectangleRoundedLinesEx(Text_Rec_3, 0.5, 10, 3, Border_Color);
  DrawTextEx(font_32, text3, text3_pos, 32, 1, Text_Color);
  if(CheckCollisionPointRec(GetMousePosition(), Text_Rec_3)){
    DrawRectangleRounded(Text_Rec_3, 0.5, 10, Fade(WHITE, 0.2));
    DrawRectangleRoundedLinesEx(Text_Rec_3, 0.5, 10, 3, Fade(WHITE, 0.2));
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
    }
  }

  // Theme Swtiching Button
  Vector2 Theme_Switching_Button_Pos = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2 - (float)Splash_Screen_Image_Light.height/2 + 195};
  float Theme_Switching_Button_Radius = 20;

  if(CheckCollisionPointCircle(GetMousePosition(), Theme_Switching_Button_Pos, Theme_Switching_Button_Radius)){
    if(CONF::Theme == Dark_Theme){
      DrawTexture(Sun_Icon, Theme_Switching_Button_Pos.x - (float)Sun_Icon.width/2, Theme_Switching_Button_Pos.y - (float)Sun_Icon.height/2, WHITE);
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Theme = Light_Theme;
    }
    else if(CONF::Theme == Light_Theme){
      DrawTexture(Moon_Icon, Theme_Switching_Button_Pos.x - (float)Moon_Icon.width/2, Theme_Switching_Button_Pos.y - (float)Moon_Icon.height/2, WHITE);
      if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) CONF::Theme = Dark_Theme;
    }
  }

  // Checking if the outside of the splash screen has been pressed
  Rectangle REC = {
    (float)GetScreenWidth()/2 - (float)Splash_Screen_Image_Light.width/2, (float)GetScreenHeight()/2 - (float)Splash_Screen_Image_Light.height/2,
    (float)Splash_Screen_Image_Light.width, (float)Splash_Screen_Image_Light.height
  };

  if(CheckCollisionPointRec(GetMousePosition(), REC) == false){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      state = Floor_Planning_State;
      Unload_Splash_Screen_Image = true;
    }
  }

}
