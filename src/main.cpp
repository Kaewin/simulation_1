#include "raylib.h"
#include "world.hpp"
#include "render.hpp"

// ============================================================================
//  main.cpp  --  platform layer (mine).
//
//  Owns the window and the game loop. The one architectural decision baked in
//  here that's worth understanding (because it's yours-adjacent and annoying to
//  retrofit later) is the FIXED-TIMESTEP loop with render interpolation:
//
//    - The simulation always advances in fixed 1/60 s steps. Determinism,
//      reproducible-from-seed behavior, and stable physics all depend on this.
//    - Rendering happens as fast as the display allows, and interpolates
//      between the last two sim states using `alpha` so motion stays smooth.
//    - The `frame > 0.25` clamp prevents the "spiral of death" where a slow
//      frame demands more sim steps, which slows the frame further, forever.
//
//  Reference if you want the theory: Glenn Fiedler, "Fix Your Timestep!"
//  This is standard, load-bearing, and boring on purpose -- exactly the kind
//  of thing you hand off.
// ============================================================================

int main() {
    // --- world config (tweak defaults; move to a config layer whenever) ---
    const int   WORLD_W  = 48;
    const int   WORLD_H  = 32;
    const int   TILE_PX  = 20;
    const uint64_t SEED  = 1234;

    const int   SCREEN_W = WORLD_W * TILE_PX;
    const int   SCREEN_H = WORLD_H * TILE_PX;

    SetConfigFlags(FLAG_VSYNC_HINT);   // cap render rate to the display
    InitWindow(SCREEN_W, SCREEN_H, "Valdaryn -- vertical slice 0");

    World world(WORLD_W, WORLD_H, SEED);

    // --- fixed-timestep accumulator ---
    const float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        float frame = GetFrameTime();
        if (frame > 0.25f) frame = 0.25f;   // anti spiral-of-death clamp
        accumulator += frame;

        while (accumulator >= FIXED_DT) {
            world.update(FIXED_DT);          // <-- the only call into YOUR code
            accumulator -= FIXED_DT;
        }

        float alpha = accumulator / FIXED_DT;   // 0..1 between sim ticks

        BeginDrawing();
        ClearBackground(Color{18, 18, 22, 255});
        render_world(world, alpha, TILE_PX);
        DrawFPS(6, 6);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
