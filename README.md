# RaylibIMGUITemplate

A template/example for using Raylib with Dear IMGUI to make awesome GUI applications (and/or games). This template app builds out-of-the box with a very simple example code. The built example shows basic usage of Raylib and IMGUI. 

This is part of The Kea Campus: http://keasigmadelta.com/kea-campus/

## Getting started - How to build the code

You'll need to have installed the following:
- A compiler (e.g., Visual Studio Code, Visual Studio on Windows, XCode on MacOS X, or GCC)
- CMake - download from https://cmake.org/download/

If you're using Visual Studio Code, then install the following extensions:
- C/C++ (make sure it's the one written by Microsoft)
- CMake Tools

Open a shell window, and type the following:
mkdir build
cd build
cmake ..
cmake --build .

That's it. You'll find the executable hello_world(.exe) either in the build directory, or in the 
Debug/ subdirectory (e.g., on Windows with Visual Studio).

Alternatively, use CMake-GUI if you prefer a user-interface instead of the command line.

## Building your own GUI application

Feel free to use this template as a starting point for your own apps. 

Dear IMGUI currently lacks comprehensive documentation. So, how do you figure out how to build the GUI that you want?

First, build this template, and experiment with it (see the "How to..." section below). You can see what IMGUI's built-in widgets and features can do. After this, you can sketch our a GUI you want to create, and then start digging into the demo code and examples to see how to implement the various bits. 

Here are some useful resources:
- The rlImGui examples: https://github.com/raylib-extras/rlImGui/tree/main/examples
- Interactive manual for demo. You can hover over parts of the demo's GUI, and it'll show you the code that builds it: https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
- The raw IMGUI demo source code: https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp
- Third-party extensions (e.g., text editor widgets, graphing, and more): https://github.com/ocornut/imgui/wiki/Useful-Extensions

### Building monitor DPI adaptive GUIs

Modern Operating Systems (OSes) provide a scale factor for high DPI screens. Dear IMGUI doesn't handle this scaling itself, which means that it's up to you, the GUI programmer. 

You can get the monitor's DPI using Raylib's  `GetWindowScaleDPI()` function (it's a 2D vector). After that, call `ImGui::GetStyle().ScaleAllSizes((float)dpiScale);`, and load fonts scaled appropriately (always round down fonts size to the nearest integer).

Here's some basic example code:
```
Vector2 monitorDPI = GetWindowScaleDPI();
int dpiScale = std::max((int)monitorDPI.x, (int)monitorDPI.y);
ImGui::GetStyle().ScaleAllSizes((float)dpiScale);
ImFont* font = io.Fonts->AddFontFromFileTTF("fonts/roboto.ttf", dpiScale * 18.0f);
```

NOTE: Monitor DPI scales can change if the user switches the screen resolution, and/or changes monitors.

See the IMGUI FAQ on this topic for more: https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application

## Authors and acknowledgment
Written by:
Hans de Ruiter

Thanks to:
- Jeffery Myers - the creator of rlImGui (the glue between Raylib and Dear IMGUI)
- Omar Cornut - the creator of Dear IMGUI
- Ramon Santamaria - for creating Raylib
- All contributors to Raylib, rlImGui, & Dear IMGUI

## License
See License.md

## Want more templates, guides, etc?

Join the Kea Campus: http://keasigmadelta.com/kea-campus/