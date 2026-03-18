#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include "common.h"

typedef void (*game_update_func)(GameContext*);

// Helper to check when the file was last modified
long get_lib_time(const char* path) {
    struct stat attr;
    if (stat(path, &attr) == 0) return attr.st_mtime;
    return 0;
}

int main() {
    InitWindow(500, 500, "Raylib Hot Reloading");
    SetTargetFPS(60);

    GameContext ctx = {0};
    void* lib_handle = NULL;
    game_update_func game_update = NULL;
    long last_mod_time = 0;

    while (!WindowShouldClose()) {
        long current_mod_time = get_lib_time("./library.so");

        // Only reload if the file has changed
        if (current_mod_time > last_mod_time) {
            if (lib_handle) dlclose(lib_handle);

            lib_handle = dlopen("./library.so", RTLD_NOW);
            if (lib_handle) {
                game_update = (game_update_func)dlsym(lib_handle, "game_update");
                last_mod_time = current_mod_time;
                printf("Library reloaded successfully!\n");
            } else {
                fprintf(stderr, "Failed to load lib: %s\n", dlerror());
            }
        }

        // Run the game logic if the library is loaded
        if (game_update) {
            game_update(&ctx);
        }
    }

    if (lib_handle) dlclose(lib_handle);
    CloseWindow();
    return 0;
}

