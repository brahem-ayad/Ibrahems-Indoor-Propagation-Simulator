#include<raylib.h>
#include"Mouse.h"

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
