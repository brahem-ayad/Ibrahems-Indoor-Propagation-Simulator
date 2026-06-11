#include "../include/Draw_Door_Placing.hpp"

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "../include/Config.hpp"
#include "../include/Utilities.hpp"
#include "../include/Floor_Plan.hpp"

// Independent helper to handle the structural validation of undo operations safely
static void UndoLastDoor() {
    if (FP::Doors.empty() || FP::doors_on_walls_ids.empty()) return;

    int target_wall_idx = FP::doors_on_walls_ids.back();
 
    // Safety check: ensure the indices match reality before popping vectors
    if (target_wall_idx >= 0 && target_wall_idx < (int)FP::Walls_Vec.size()) {
        if (!FP::Walls_Vec[target_wall_idx].Doors_Vec.empty()) {
            FP::Walls_Vec[target_wall_idx].Doors_Vec.pop_back();
        }
    }

    FP::Doors.pop_back();
    FP::doors_on_walls_ids.pop_back();
}

void Draw_Door_Placing() {
    // 1. Position Gather Pass
    Vector2 pos_w = CONF::Enable_Snapping 
                    ? Get_Snapped_World_Mouse_Position() 
                    : GetScreenToWorld2D(GetMousePosition(), CONF::camera2);
    Vector2 pos_s = GetWorldToScreen2D(pos_w, CONF::camera2);

    float door_radius_world = (CONF::Door_Width / 2.0f) * CONF::Grid_Spacing;

    bool snapped_to_wall = false;
    bool placement_allowed = false;

    Vector2 projected_pos_w = pos_w;
    Vector2 projected_pos_s = pos_s;
    int active_wall_idx = -1;
    Vector2 wall_dir = { 0.0f, 0.0f };

    // 2. Scan scene geometry for intersections
    for (size_t i = 0; i < FP::Walls_Vec.size(); i++) {
        const auto& wall = FP::Walls_Vec[i];

        // Core collision check: Is the cursor hovering near this wall segment?
        if (!CheckCollisionCircleLine(pos_w, door_radius_world / 5.0f, wall.Wall_Start_2D, wall.Wall_End_2D)) {
            continue; 
        }

        snapped_to_wall = true;
        projected_pos_w = Get_Closest_Point_on_Segment(pos_w, wall.Wall_Start_2D, wall.Wall_End_2D);
        projected_pos_s = GetWorldToScreen2D(projected_pos_w, CONF::camera2);

        // Clearance Check: Ensure the door fits inside the start/end bounds of the wall
        float d1 = Vector2Distance(projected_pos_w, wall.Wall_Start_2D) / CONF::Grid_Spacing;
        float d2 = Vector2Distance(projected_pos_w, wall.Wall_End_2D) / CONF::Grid_Spacing;
        if (d1 < CONF::Door_Width / 2.0f || d2 < CONF::Door_Width / 2.0f) {
            continue; // Too close to corners
        }

        // Overlap Checks: Scan existing windows on this wall
        bool overlapping_object = false;
        for (const auto& win : wall.Windows_Vec) {
            float win_radius_world = (win.Width / 2.0f) * CONF::Grid_Spacing;
            if (CheckCollisionCircles(pos_w, door_radius_world, win.Position, win_radius_world)) {
                overlapping_object = true;
                break;
            }
        }
        if (overlapping_object) continue;

        // Overlap Checks: Scan existing doors on this wall
        for (const auto& door : wall.Doors_Vec) {
            float other_door_radius_world = (door.Width / 2.0f) * CONF::Grid_Spacing;
            if (CheckCollisionCircles(pos_w, door_radius_world, door.Position, other_door_radius_world)) {
                overlapping_object = true;
                break;
            }
        }
        if (overlapping_object) continue;

        // If we survived all loops/guards, we found a valid wall slice!
        placement_allowed = true;
        active_wall_idx = (int)i;
        wall_dir = Vector2Normalize(Vector2Subtract(wall.Wall_Start_2D, wall.Wall_End_2D));
        break; // Found our target wall segment, cease searching
    }

    // 3. Render Context Previews & Handle Mouse Placements
    if (placement_allowed && active_wall_idx != -1) {
        // Valid State Rendering
        DrawCircleV(projected_pos_s, 5 * CONF::camera2.zoom, BLUE);
        DrawCircleV(projected_pos_s, door_radius_world * CONF::camera2.zoom, Fade(BLUE, 0.2f));
        Draw_Rectangle_Around_Lines(projected_pos_w, wall_dir, CONF::Door_Width, 2, BLUE);

        // Execute Placement Input Action
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DOOR D;
            D.Position = projected_pos_w;
            D.Height   = CONF::Door_Height;
            D.Width    = CONF::Door_Width;
            D.Material = CONF::Door_Material;

            RectanglePoints RP = Get_Rectangle_Points_Around_Lines(projected_pos_w, wall_dir, CONF::Door_Width, 2);
            D.P1 = RP.P1; D.P2 = RP.P2; D.P3 = RP.P3; D.P4 = RP.P4;
            D.ID = active_wall_idx;

            FP::Doors.push_back(D);
            FP::Walls_Vec[active_wall_idx].Doors_Vec.push_back(D);
            FP::doors_on_walls_ids.push_back(active_wall_idx);
        }
    } 
    else {
        // Invalid/Idle State Rendering
        if (snapped_to_wall) {
            // Snapped onto a wall, but clearance or intersection rules failed
            DrawCircleV(projected_pos_s, 5 * CONF::camera2.zoom, RED);
            DrawCircleV(projected_pos_s, door_radius_world * CONF::camera2.zoom, Fade(RED, 0.2f));
        } else {
            // Completely floating out in free empty space
            DrawCircleV(pos_s, 5 * CONF::camera2.zoom, RED);
            DrawCircleV(pos_s, door_radius_world * CONF::camera2.zoom, Fade(RED, 0.2f));
        }
    }

    // 4. Global Action / Mode Controls
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
        UndoLastDoor();
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        CONF::Selected_Tool = None_Tool;
    }
}
