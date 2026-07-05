#include "render.hpp"
#include "raylib.h"

static Color tile_color(Tile t) {
    switch (t) {
        case Tile::Grass: return Color{ 76, 110,  63, 255 };
        case Tile::Sand:  return Color{198, 176, 122, 255 };
        case Tile::Stone: return Color{112, 112, 120, 255 };
        case Tile::Water: return Color{ 44,  78, 120, 255 };
        default:          return MAGENTA;   
}

void render_world(const World& world, float alpha, int tile_px) {
    for (int y = 0; y < world.height(); ++y) {
        for (int x = 0; x < world.width(); ++x) {
            DrawRectangle(x * tile_px, y * tile_px, tile_px, tile_px,
                          tile_color(world.tileAt(x, y)));
        }
    }

    for (const Agent& a : world.agents()) {
        float ix = a.px + (a.x - a.px) * alpha;
        float iy = a.py + (a.y - a.py) * alpha;
        float cx = (ix + 0.5f) * tile_px;   
        float cy = (iy + 0.5f) * tile_px;
        DrawCircle(static_cast<int>(cx), static_cast<int>(cy),
                   tile_px * 0.35f, Color{220, 180, 90, 255});
    }
}