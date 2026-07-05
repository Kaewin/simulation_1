#include "world.hpp"
#include <cmath>

// ============================================================================
//  world.cpp  --  YOURS.
//
//  Everything here is a PLACEHOLDER whose only job is to make the pipeline
//  visibly alive: a generated tile grid and a few agents wandering across it.
//  It is deliberately dumb. It uses no clever data layout, no spatial
//  structure, no real AI. It exists so that when you delete it and write the
//  first real system, you already have something on screen to compare against.
//
//  The moment you replace this file, the project becomes yours.
// ============================================================================

// --- tiny deterministic RNG so worldgen is reproducible from a seed ---
uint32_t World::next_random() {
    // xorshift64 -> take the high bits. Good enough for placeholder worldgen.
    rng_ ^= rng_ << 13;
    rng_ ^= rng_ >> 7;
    rng_ ^= rng_ << 17;
    return static_cast<uint32_t>(rng_ >> 32);
}

World::World(int width, int height, uint64_t seed)
    : w_(width), h_(height), rng_(seed ? seed : 0x9E3779B97F4A7C15ull) {

    tiles_.resize(static_cast<size_t>(w_) * h_);

    // Placeholder worldgen: mostly grass, a diagonal-ish band of water,
    // some scattered stone. Nothing principled -- just something to look at.
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            Tile t = Tile::Grass;
            int band = x + y;
            if (band % 17 == 0 || band % 17 == 1) t = Tile::Water;
            else if (next_random() % 100 < 6)      t = Tile::Stone;
            else if (next_random() % 100 < 8)      t = Tile::Sand;
            tiles_[static_cast<size_t>(y) * w_ + x] = t;
        }
    }

    // Drop a handful of agents on walkable tiles.
    const int kAgents = 12;
    for (int i = 0; i < kAgents; ++i) {
        Agent a;
        do {
            a.x = static_cast<float>(next_random() % w_);
            a.y = static_cast<float>(next_random() % h_);
        } while (solid(static_cast<int>(a.x), static_cast<int>(a.y)));
        a.px = a.x; a.py = a.y;
        pick_new_target(a);
        agents_.push_back(a);
    }
}

bool World::solid(int x, int y) const {
    if (x < 0 || y < 0 || x >= w_ || y >= h_) return true;   // edges block
    return tileAt(x, y) == Tile::Water;
}

void World::pick_new_target(Agent& a) {
    // Wander: pick a nearby walkable tile as the next target.
    for (int tries = 0; tries < 16; ++tries) {
        int nx = static_cast<int>(a.x) + (int(next_random() % 7) - 3);
        int ny = static_cast<int>(a.y) + (int(next_random() % 7) - 3);
        if (!solid(nx, ny)) { a.tx = float(nx); a.ty = float(ny); return; }
    }
    a.tx = a.x; a.ty = a.y;   // give up this tick, stay put
}

void World::update(float dt) {
    const float speed = 2.5f;   // tiles per second

    for (Agent& a : agents_) {
        // Record where we were at the start of the tick (for interpolation).
        a.px = a.x;
        a.py = a.y;

        // Move toward the target tile.
        float dx = a.tx - a.x;
        float dy = a.ty - a.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < 0.05f) {
            pick_new_target(a);        // arrived -> new destination
        } else {
            float step = speed * dt;
            if (step > dist) step = dist;
            a.x += (dx / dist) * step;
            a.y += (dy / dist) * step;
        }
    }
}
