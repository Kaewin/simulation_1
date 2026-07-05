#include "render.hpp"
#include "raylib.h"

// ============================================================================
//  render.cpp  --  platform layer (mine).
//
//  Reads the World ONLY through its public accessors and draws it. Because it
//  never touches World internals, you can rewrite your entire data layout and
//  this file keeps compiling untouched. That decoupling is the point.
// ============================================================================

static Color tile_color(Tile t) {
    switch (t) {
        case Tile::Grass: return Color{ 76, 110,  63, 255 };
        case Tile::Sand:  return Color{198, 176, 122, 255 };
        case Tile::Stone: return Color{112, 112, 120, 255 };
        case Tile::Water: return Color{ 44,  78, 120, 255 };
        default:          return MAGENTA;   // loud on purpose if something's off
    }
}

void render_world(const World& world, float alpha, int tile_px) {
    // --- tiles ---
    for (int y = 0; y < world.height(); ++y) {
        for (int x = 0; x < world.width(); ++x) {
            DrawRectangle(x * tile_px, y * tile_px, tile_px, tile_px,
                          tile_color(world.tileAt(x, y)));
        }
    }

    // --- agents (interpolated between last tick and this one) ---
    for (const Agent& a : world.agents()) {
        float ix = a.px + (a.x - a.px) * alpha;
        float iy = a.py + (a.y - a.py) * alpha;
        float cx = (ix + 0.5f) * tile_px;   // center of the tile
        float cy = (iy + 0.5f) * tile_px;
        DrawCircle(static_cast<int>(cx), static_cast<int>(cy),
                   tile_px * 0.35f, Color{220, 180, 90, 255});
    }
}
