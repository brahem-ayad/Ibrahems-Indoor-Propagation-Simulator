#pragma once

#include<raylib.h>
#include"portable-file-dialogs.h"

static void Load_Floor_Plan_Image(Texture2D &texture, bool &imageLoaded){
  // Open the dialog
  auto selection = pfd::open_file("Select an image to import", ".",
    { "Image Files (.png, .jpg, .bmp)", "*.png *.jpg *.jpeg *.bmp",
      "All Files", "*" },
    pfd::opt::none).result();

  if (selection.empty() == false) {
    // Unload previous texture if one exists
    if (imageLoaded) UnloadTexture(texture);

    // Load the new texture from the selected path
    texture = LoadTexture(selection[0].c_str());

    if (texture.id > 0) {
        imageLoaded = true;
    }
  }
}
