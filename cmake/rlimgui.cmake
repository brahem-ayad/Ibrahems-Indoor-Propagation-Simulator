FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG        main  # <-- This tells CMake to pull the latest commit on 'main'
)
FetchContent_MakeAvailable(rlimgui)

if(NOT TARGET rlimgui)
    add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
    target_link_libraries(rlimgui PUBLIC raylib imgui)
    target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
endif()
