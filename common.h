#ifndef COMMON_H
#define COMMON_H

#include <raylib.h>
#include <raymath.h>

// This struct stays alive in the Host's memory while the Library swaps
typedef struct {
    Camera3D camera;
    Vector3 Vs[4];
    bool initialized;
} GameContext;

#endif
