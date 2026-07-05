#include "world.hpp"
#include <cmath>

uint32_t World::next_random() {
    rng_ ^= rng_ << 13;
    rng_ ^= rng_ >> 7;
    rng_ ^= rng_ << 17;
    return static_cast<uint32_t>(rng_ >> 32);
}

World::World(int width, int height, uint64_t seed)
    : w_(width), h_(height), rng_(seed ? seed : 0x9E3779B97F4A7C15ull) {

    tiles_.resize(static_cast<size_t>(w_) * h_);

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

    int cx = w_ / 2;
    int cy = h_ / 2;
    while (solid(cx, cy)) {
        cx += 1;                     
        if (cx >= w_) { cx = 0; cy += 1; }   
    }
    player_.x = static_cast<float>(cx);
    player_.y = static_cast<float>(cy);
    player_.px = player_.x;          
    player_.py = player_.y;          
}

bool World::solid(int x, int y) const {
    if (x < 0 || y < 0 || x >= w_ || y >= h_) return true;   // edges block
    return tileAt(x, y) == Tile::Water;
}

void World::pick_new_target(Agent& a) {
    for (int tries = 0; tries < 16; ++tries) {
        int nx = static_cast<int>(a.x) + (int(next_random() % 7) - 3);
        int ny = static_cast<int>(a.y) + (int(next_random() % 7) - 3);
        if (!solid(nx, ny)) { a.tx = float(nx); a.ty = float(ny); return; }
    }
    a.tx = a.x; a.ty = a.y;   
}

void World::update(float dt, const Input& in) {
    player_.px = player_.x;
    player_.py = player_.y;
    const float speed = 2.5f;  

    for (Agent& a : agents_) {
        a.px = a.x;
        a.py = a.y;

        float dx = a.tx - a.x;
        float dy = a.ty - a.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < 0.05f) {
            pick_new_target(a);        
        } else {
            float step = speed * dt;
            if (step > dist) step = dist;
            a.x += (dx / dist) * step;
            a.y += (dy / dist) * step;
        }
    }
}
