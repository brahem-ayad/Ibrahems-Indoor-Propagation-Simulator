#include "../include/Generate_Wall_Collision_Quads.hpp"

#include <vector>
#include <algorithm>
#include <cmath>
#include "../include/Floor_Plan.hpp"
#include "../include/Draw_Walls.hpp"

std::vector<WallQuad3D> Generate_Wall_Collision_Quads() {
    std::vector<WallQuad3D> collision_quads;

    for (size_t i = 0; i < FP::Walls_Vec.size(); i++) {
        auto& wall = FP::Walls_Vec[i];

        // Capture attributes to assign to the child quads
        WALL_MATERIAL wall_material = wall.Wall_Material; 
        float wall_width            = wall.Wall_Width;

        // 1. Directional Math
        float dx = wall.Wall_End_3D.x - wall.Wall_Start_3D.x;
        float dy = wall.Wall_End_3D.y - wall.Wall_Start_3D.y;
        float wall_len = sqrtf(dx * dx + dy * dy);
        if (wall_len == 0.0f) continue; 

        float nx = -dy / wall_len;
        float ny = dx / wall_len;
        float r = wall_width / 2.0f;
        float offsetX = nx * r;
        float offsetY = ny * r;

        // 2. Segment Splitting Logic (Identical to Rendering Layout)
        std::vector<Wall3DInterval> solid_segments;

        // Doors
        for (const auto& door : wall.Doors_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, door.Position);
            float hw   = (door.Width / 2.0f) * CONF::Grid_Spacing;
            float t_start = Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);
            float t_end   = Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);

            if (door.Height < wall.Wall_Height) {
                solid_segments.push_back({ t_start, t_end, door.Height, wall.Wall_Height });
            }
        }

        // Windows
        for (const auto& win : wall.Windows_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, win.Position);
            float hw   = (win.Width / 2.0f) * CONF::Grid_Spacing;
            float t_start = Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);
            float t_end   = Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);

            if (win.base_height > 0.0f) {
                solid_segments.push_back({ t_start, t_end, 0.0f, win.base_height });
            }
            float win_top = win.base_height + win.window_height;
            if (win_top < wall.Wall_Height) {
                solid_segments.push_back({ t_start, t_end, win_top, wall.Wall_Height });
            }
        }

        // Base/Full Wall Segments
        struct Gap { float start, end; };
        std::vector<Gap> blocked_intervals;

        for (const auto& d : wall.Doors_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, d.Position);
            float hw   = (d.Width / 2.0f) * CONF::Grid_Spacing;
            blocked_intervals.push_back({ Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f), Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f) });
        }
        for (const auto& w : wall.Windows_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, w.Position);
            float hw   = (w.Width / 2.0f) * CONF::Grid_Spacing;
            blocked_intervals.push_back({ Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f), Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f) });
        }

        std::sort(blocked_intervals.begin(), blocked_intervals.end(), [](const Gap& a, const Gap& b) {
            return a.start < b.start;
        });

        float current_t = 0.0f;
        for (const auto& interval : blocked_intervals) {
            if (interval.start > current_t) {
                solid_segments.push_back({ current_t, interval.start, 0.0f, wall.Wall_Height });
            }
            if (interval.end > current_t) current_t = interval.end;
        }
        if (current_t < 1.0f) {
            solid_segments.push_back({ current_t, 1.0f, 0.0f, wall.Wall_Height });
        }

        // =================================================================
        // 3. GENERATE 4-POINT QUAD FACES WITH MATERIAL AND WIDTH DATA
        // =================================================================
        for (const auto& seg : solid_segments) {
            if (seg.end_t - seg.start_t < 0.001f || seg.top_z - seg.bottom_z < 0.001f) continue;

            float start_x = wall.Wall_Start_3D.x + dx * seg.start_t;
            float start_y = wall.Wall_Start_3D.y + dy * seg.start_t;
            float end_x   = wall.Wall_Start_3D.x + dx * seg.end_t;
            float end_y   = wall.Wall_Start_3D.y + dy * seg.end_t;

            // Generate 8 base spatial coordinates
            Vector3 b_start_left  = { start_x + offsetX, start_y + offsetY, seg.bottom_z };
            Vector3 b_start_right = { start_x - offsetX, start_y - offsetY, seg.bottom_z };
            Vector3 b_end_left    = { end_x + offsetX,   end_y + offsetY,   seg.bottom_z };
            Vector3 b_end_right   = { end_x - offsetX,   end_y - offsetY,   seg.bottom_z };

            Vector3 t_start_left  = { b_start_left.x,  b_start_left.y,  seg.top_z };
            Vector3 t_start_right = { b_start_right.x, b_start_right.y, seg.top_z };
            Vector3 t_end_left    = { b_end_left.x,    b_end_left.y,    seg.top_z };
            Vector3 t_end_right   = { b_end_right.x,   b_end_right.y,   seg.top_z };

            // Left Long Face
            collision_quads.push_back({ b_start_left, b_end_left, t_end_left, t_start_left, wall_material, wall_width });

            // Right Long Face
            collision_quads.push_back({ b_end_right, b_start_right, t_start_right, t_end_right, wall_material, wall_width });

            // Top Face (Ceiling)
            collision_quads.push_back({ t_start_left, t_end_left, t_end_right, t_start_right, wall_material, wall_width });

            // Bottom Face (Floor - only if raised above absolute zero)
            if (seg.bottom_z > 0.001f) {
                collision_quads.push_back({ b_start_left, b_start_right, b_end_right, b_end_left, wall_material, wall_width });
            }

            // Window/Door Inner Frame Caps
            if (seg.start_t >= 0.0f) {
                collision_quads.push_back({ b_start_right, b_start_left, t_start_left, t_start_right, wall_material, wall_width });
            }
            if (seg.end_t <= 1.0f) {
                collision_quads.push_back({ b_end_left, b_end_right, t_end_right, t_end_left, wall_material, wall_width });
            }
        }
    }

    return collision_quads;
}
