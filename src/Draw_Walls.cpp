#include"../include/Draw_Walls.hpp"

#include<raylib.h>
#include<raymath.h>
#include<rlgl.h>
#include<algorithm>
#include"../include/Floor_Plan.hpp"
#include"../include/Config.hpp"

struct WallInterval {
    float start;
    float end;
};

// Helper structure to pass calculated section segment details between our loops
struct WallSegmentData {
    Vector2 start;
    Vector2 end;
    float width;
    Color color;
};

// --- PASS 1 HELPER: Draws the raw silhouette framework ---
static void Draw_Wall_Outline_Segment(Vector2 start_pos, Vector2 end_pos, float width) {
    float half_width_outer = ((width * CONF::Grid_Spacing) / 2.0f) + 1.0f;
    float total_thickness  = (width * CONF::Grid_Spacing) + 2.0f;

    DrawLineEx(start_pos, end_pos, total_thickness, BLACK);
    DrawCircleV(start_pos, half_width_outer, BLACK);
    DrawCircleV(end_pos, half_width_outer, BLACK);
}

// --- PASS 2 HELPER: Draws the textured material core ---
static void Draw_Wall_Material_Segment(Vector2 start_pos, Vector2 end_pos, float width, Color color) {
    Vector2 direction = Vector2Normalize(Vector2Subtract(end_pos, start_pos));
    Vector2 direction_perp = { direction.y, -direction.x };

    float half_width_inner = (width * CONF::Grid_Spacing) / 2.0f;

    Vector2 V1_W = Vector2Add(end_pos,   Vector2Scale(direction_perp,  half_width_inner));
    Vector2 V2_W = Vector2Add(end_pos,   Vector2Scale(direction_perp, -half_width_inner));
    Vector2 V3_W = Vector2Add(start_pos, Vector2Scale(direction_perp, -half_width_inner));
    Vector2 V4_W = Vector2Add(start_pos, Vector2Scale(direction_perp,  half_width_inner));

    DrawTriangle(V3_W, V2_W, V1_W, color);
    DrawTriangle(V4_W, V3_W, V1_W, color);
    
    DrawCircleV(start_pos, half_width_inner, color);
    DrawCircleV(end_pos, half_width_inner, color);
}

void Draw_Walls_2D() {
    if (FP::Walls_Vec.empty()) return;

    // We'll collect visible segments dynamically to avoid recalculating intervals twice
    std::vector<WallSegmentData> material_segments_to_render;

    // =================================================================
    // PASS 1: DRAW ALL BLACK BACKGROUND OUTLINES & GLOBAL END CAPS FIRST
    // =================================================================
    EndShaderMode(); // Absolutely ensure shader is off for crisp black backing line work

    for (size_t i = 0; i < FP::Walls_Vec.size(); i++) {
        const auto& wall = FP::Walls_Vec[i];
        
        float wall_length = Vector2Distance(wall.Wall_Start_2D, wall.Wall_End_2D);
        if (wall_length <= 0.0f) continue;

        // Draw the absolute outer terminal node caps first
        float absolute_cap_radius = ((wall.Wall_Width * CONF::Grid_Spacing) / 2.0f) + 1.0f;
        DrawCircleV(wall.Wall_Start_2D, absolute_cap_radius, BLACK);
        DrawCircleV(wall.Wall_End_2D, absolute_cap_radius, BLACK);

        // Resolve material color assignment
        Color color = {235, 235, 235, 255}; 
        if      (wall.Wall_Material == Concrete_Wall) color = {191, 191, 191, 255};
        else if (wall.Wall_Material == Brick_Wall)    color = {178, 102, 76, 255};
        else if (wall.Wall_Material == Wooden_Wall)   color = {217, 186, 140, 255};
        else if (wall.Wall_Material == Glass_Wall)    color = {166, 217, 242, 255};
        else if (wall.Wall_Material == Metal_Wall)    color = {102, 128, 153, 255};

        // Gather window/door cutoff intervals
        std::vector<WallInterval> openings;
        for (const auto& door : wall.Doors_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, door.Position);
            float hw   = (door.Width / 2.0f) * CONF::Grid_Spacing;
            openings.push_back({ Clamp((dist - hw) / wall_length, 0.0f, 1.0f), Clamp((dist + hw) / wall_length, 0.0f, 1.0f) });
        }
        for (const auto& win : wall.Windows_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, win.Position);
            float hw   = (win.Width / 2.0f) * CONF::Grid_Spacing;
            openings.push_back({ Clamp((dist - hw) / wall_length, 0.0f, 1.0f), Clamp((dist + hw) / wall_length, 0.0f, 1.0f) });
        }

        std::sort(openings.begin(), openings.end(), [](const WallInterval& a, const WallInterval& b) {
            return a.start < b.start;
        });

        // Compute gaps and draw outline intervals
        float current_t = 0.0f;
        for (const auto& opening : openings) {
            if (opening.start > current_t) {
                Vector2 seg_start = Vector2Lerp(wall.Wall_Start_2D, wall.Wall_End_2D, current_t);
                Vector2 seg_end   = Vector2Lerp(wall.Wall_Start_2D, wall.Wall_End_2D, opening.start);
                
                // Draw outline segment immediately
                Draw_Wall_Outline_Segment(seg_start, seg_end, wall.Wall_Width);
                // Cache info for the material fill phase
                material_segments_to_render.push_back({ seg_start, seg_end, wall.Wall_Width, color });
            }
            if (opening.end > current_t) current_t = opening.end;
        }

        if (current_t < 1.0f) {
            Vector2 seg_start = Vector2Lerp(wall.Wall_Start_2D, wall.Wall_End_2D, current_t);
            
            Draw_Wall_Outline_Segment(seg_start, wall.Wall_End_2D, wall.Wall_Width);
            material_segments_to_render.push_back({ seg_start, wall.Wall_End_2D, wall.Wall_Width, color });
        }
    }

    // =================================================================
    // PASS 2: BATCH RENDER THE INNER FILL CONTENT INSIDE THE SHADER
    // =================================================================
    BeginShaderMode(CONF::Wall_Shader_2D);
    
    for (const auto& seg : material_segments_to_render) {
        Draw_Wall_Material_Segment(seg.start, seg.end, seg.width, seg.color);
    }
    
    EndShaderMode();
}

void Draw_Walls_3D() {
    rlDisableBackfaceCulling();
    BeginShaderMode(CONF::Wall_Shader_3D);

    for (size_t i = 0; i < FP::Walls_Vec.size(); i++) {
        auto& wall = FP::Walls_Vec[i];

        // 1. Resolve Material Colors
        Color color = {235, 235, 235, 255}; // Default Drywall
        if      (wall.Wall_Material == Concrete_Wall) color = {191, 191, 191, 255};
        else if (wall.Wall_Material == Brick_Wall)    color = {178, 102, 76, 255};
        else if (wall.Wall_Material == Wooden_Wall)   color = {217, 186, 140, 255};
        else if (wall.Wall_Material == Glass_Wall)    color = {166, 217, 242, 255};
        else if (wall.Wall_Material == Metal_Wall)    color = {102, 128, 153, 255};

        // 2. Structural Direction Math
        float dx = wall.Wall_End_3D.x - wall.Wall_Start_3D.x;
        float dy = wall.Wall_End_3D.y - wall.Wall_Start_3D.y;
        float wall_len = sqrtf(dx * dx + dy * dy);
        if (wall_len == 0.0f) continue; 

        float nx = -dy / wall_len;
        float ny = dx / wall_len;
        float r = wall.Wall_Width / 2.0f;
        float offsetX = nx * r;
        float offsetY = ny * r;

        // 3. Absolute Terminal Corner Cylinder Caps
        DrawCylinderEx({wall.Wall_Start_3D.x, wall.Wall_Start_3D.y, 0},
                       {wall.Wall_Start_3D.x, wall.Wall_Start_3D.y, wall.Wall_Height},
                       r, r, 16, color);

        DrawCylinderEx({wall.Wall_End_3D.x, wall.Wall_End_3D.y, 0},
                       {wall.Wall_End_3D.x, wall.Wall_End_3D.y, wall.Wall_Height},
                       r, r, 16, color);

        // 4. Collect and Segment Sub-component Openings (0.0 to 1.0)
        std::vector<Wall3DInterval> solid_segments;

        // Map Door Headers
        for (const auto& door : wall.Doors_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, door.Position);
            float hw   = (door.Width / 2.0f) * CONF::Grid_Spacing;
            float t_start = Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);
            float t_end   = Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);

            // Wall slice floating directly above the door frame up to ceiling
            if (door.Height < wall.Wall_Height) {
                solid_segments.push_back({ t_start, t_end, door.Height, wall.Wall_Height });
            }
        }

        // Map Window Sills and Headers
        for (const auto& win : wall.Windows_Vec) {
            float dist = Vector2Distance(wall.Wall_Start_2D, win.Position);
            float hw   = (win.Width / 2.0f) * CONF::Grid_Spacing;
            float t_start = Clamp((dist - hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);
            float t_end   = Clamp((dist + hw) / (wall_len * CONF::Grid_Spacing), 0.0f, 1.0f);

            // Wall slice below the window frame (the sill)
            if (win.base_height > 0.0f) {
                solid_segments.push_back({ t_start, t_end, 0.0f, win.base_height });
            }
            // Wall slice above the window frame (the header)
            float win_top = win.base_height + win.window_height;
            if (win_top < wall.Wall_Height) {
                solid_segments.push_back({ t_start, t_end, win_top, wall.Wall_Height });
            }
        }

        // Map Full-Height Wall Gaps (Where there are no doors or windows)
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
        // 5. RENDER GEOMETRY FOR ALL PACKED VOLUMES
        // =================================================================
        for (const auto& seg : solid_segments) {
            // Find the 3D center positions for this specific sub-segment slice
            float start_x = wall.Wall_Start_3D.x + dx * seg.start_t;
            float start_y = wall.Wall_Start_3D.y + dy * seg.start_t;
            float end_x   = wall.Wall_Start_3D.x + dx * seg.end_t;
            float end_y   = wall.Wall_Start_3D.y + dy * seg.end_t;

            // Generate the 8 unique spatial vertex coordinates
            Vector3 b_start_left  = { start_x + offsetX, start_y + offsetY, seg.bottom_z };
            Vector3 b_start_right = { start_x - offsetX, start_y - offsetY, seg.bottom_z };
            Vector3 b_end_left    = { end_x + offsetX,   end_y + offsetY,   seg.bottom_z };
            Vector3 b_end_right   = { end_x - offsetX,   end_y - offsetY,   seg.bottom_z };

            Vector3 t_start_left  = { b_start_left.x,  b_start_left.y,  seg.top_z };
            Vector3 t_start_right = { b_start_right.x, b_start_right.y, seg.top_z };
            Vector3 t_end_left    = { b_end_left.x,    b_end_left.y,    seg.top_z };
            Vector3 t_end_right   = { b_end_right.x,   b_end_right.y,   seg.top_z };

            // --- Draw Faces ---
            // Left long panel
            DrawTriangle3D(b_start_left, b_end_left, t_end_left, color);
            DrawTriangle3D(b_start_left, t_end_left, t_start_left, color);

            // Right long panel
            DrawTriangle3D(b_end_right, b_start_right, t_start_right, color);
            DrawTriangle3D(b_end_right, t_start_right, t_end_right, color);

            // Top Face (Ceiling Cap)
            DrawTriangle3D(t_start_left, t_end_left, t_end_right, color);
            DrawTriangle3D(t_start_left, t_end_right, t_start_right, color);

            // Bottom Face (Floor Cap - skip if flat against absolute ground)
            if (seg.bottom_z > 0.0f) {
                DrawTriangle3D(b_start_left, b_start_right, b_end_right, color);
                DrawTriangle3D(b_start_left, b_end_right, b_end_left, color);
            }
            
            // Internal Opening Framing Caps (Left & Right cross-sections)
            // Draws clean connecting quads on the open side profiles where walls break
            if (seg.start_t > 0.0f) { // Left internal framing wall profile
                DrawTriangle3D(b_start_right, b_start_left, t_start_left, color);
                DrawTriangle3D(b_start_right, t_start_left, t_start_right, color);
            }
            if (seg.end_t < 1.0f) { // Right internal framing wall profile
                DrawTriangle3D(b_end_left, b_end_right, t_end_right, color);
                DrawTriangle3D(b_end_left, t_end_right, t_end_left, color);
            }
        }
    }

    EndShaderMode();
    rlEnableBackfaceCulling();
}
