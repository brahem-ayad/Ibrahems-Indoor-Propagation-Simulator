#pragma once

#include<raylib.h>
#include"../Floor_Plan.h"

static void Delete_Walls() {
  if(IsKeyPressed(KEY_DELETE)){
    for(int i = 0; i < FP::walls.size(); i++){
      if(FP::walls[i].Selected){

      // remove any door on the wall
      for(int j = 0; j < FP::walls[i].doors.size(); j++){
        int id = FP::walls[i].doors[j].ID;

        // remove the doors from the floor plan doors vector
        FP::doors.erase(
            std::remove_if(FP::doors.begin(), FP::doors.end(), [id](const Door& door) {
                return door.ID == id;
            }),
            FP::doors.end()
        );
        // remove them from the CONF doors on walls ids vector
        CONF::doors_on_walls_ids.erase(
            std::remove_if(CONF::doors_on_walls_ids.begin(), CONF::doors_on_walls_ids.end(), [id](const int& ID) {
            return ID == id;
            }),
            CONF::doors_on_walls_ids.end()
        );
      }
      // remove any window on the wall
      for(int j = 0; j < FP::walls[i].windows.size(); j++){
        int id = FP::walls[i].windows[j].ID;

        // remove the doors from the floor plan doors vector
        FP::windows.erase(
            std::remove_if(FP::windows.begin(), FP::windows.end(), [id](const Window& window) {
                return window.ID == id;
            }),
            FP::windows.end()
        );
        // remove them from the CONF doors on walls ids vector
        CONF::windows_on_walls_ids.erase(
            std::remove_if(CONF::windows_on_walls_ids.begin(), CONF::windows_on_walls_ids.end(), [id](const int& ID) {
            return ID == id;
            }),
            CONF::windows_on_walls_ids.end()
        );
      }

        FP::walls.erase(FP::walls.begin() + i);
        i--;
      }
    }
  }
}
