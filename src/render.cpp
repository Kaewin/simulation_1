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
}

void render_world(const World& world, float alpha, int tile_px) {
    // Camera centered on the interpolated player position.
    const Player& p = world.player();
    float pix = p.px + (p.x - p.px) * alpha;
    float piy = p.py + (p.y - p.py) * alpha;

    Camera2D cam = {};
    cam.target = { (pix + 0.5f) * tile_px, (piy + 0.5f) * tile_px };
    cam.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    cam.zoom   = 1.0f;

    BeginMode2D(cam);

    // Cull: only iterate the tiles inside the visible window (+1 tile margin).
    float half_w = GetScreenWidth()  / 2.0f;
    float half_h = GetScreenHeight() / 2.0f;
    int min_x = static_cast<int>((cam.target.x - half_w) / tile_px) - 1;
    int max_x = static_cast<int>((cam.target.x + half_w) / tile_px) + 1;
    int min_y = static_cast<int>((cam.target.y - half_h) / tile_px) - 1;
    int max_y = static_cast<int>((cam.target.y + half_h) / tile_px) + 1;
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= world.width())  max_x = world.width()  - 1;
    if (max_y >= world.height()) max_y = world.height() - 1;

    for (int y = min_y; y <= max_y; ++y)
        for (int x = min_x; x <= max_x; ++x)
            DrawRectangle(x * tile_px, y * tile_px, tile_px, tile_px,
                          tile_color(world.tileAt(x, y)));

    for (const Agent& a : world.agents()) {
        float ix = a.px + (a.x - a.px) * alpha;
        float iy = a.py + (a.y - a.py) * alpha;
        DrawCircle(static_cast<int>((ix + 0.5f) * tile_px),
                   static_cast<int>((iy + 0.5f) * tile_px),
                   tile_px * 0.35f, Color{220, 180, 90, 255});
    }

    DrawCircle(static_cast<int>((pix + 0.5f) * tile_px),
               static_cast<int>((piy + 0.5f) * tile_px),
               tile_px * 0.42f, Color{235, 235, 240, 255});

    EndMode2D();
}