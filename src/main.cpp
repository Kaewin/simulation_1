#include "raylib.h"
#include "world.hpp"
#include "render.hpp"

int main() {
    const int   WORLD_W  = 48;
    const int   WORLD_H  = 32;
    const int   TILE_PX  = 20;
    const uint64_t SEED  = 1234;

    const int   SCREEN_W = WORLD_W * TILE_PX;
    const int   SCREEN_H = WORLD_H * TILE_PX;

    SetConfigFlags(FLAG_VSYNC_HINT);   
    InitWindow(SCREEN_W, SCREEN_H, "Valdaryn -- vertical slice 0");

    World world(WORLD_W, WORLD_H, SEED);

    const float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        float frame = GetFrameTime();
        if (frame > 0.25f) frame = 0.25f;   
        accumulator += frame;

        Input in;
        in.move_x = static_cast<float>(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        in.move_y = static_cast<float>(IsKeyDown(KEY_S) - IsKeyDown(KEY_W));

        while (accumulator >= FIXED_DT) {
            world.update(FIXED_DT, in);         
            accumulator -= FIXED_DT;
        }

        float alpha = accumulator / FIXED_DT;

        BeginDrawing();
        ClearBackground(Color{18, 18, 22, 255});
        render_world(world, alpha, TILE_PX);
        DrawFPS(6, 6);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}