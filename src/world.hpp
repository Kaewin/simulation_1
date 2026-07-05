#pragma once
#include <cstdint>
#include <vector>

// ============================================================================
//  world.hpp  --  THE SEAM.
//
//  This header is the entire contract between the platform layer (main.cpp,
//  render.cpp -- not yours) and the simulation (world.cpp -- yours).
//
//  The platform layer promises only two things:
//    1. It will call World::update(dt) at a fixed rate (see main.cpp).
//    2. It will read your state ONLY through the const accessors below.
//
//  Everything else -- how tiles are stored, how agents are represented, how
//  you partition space, whether "Agent" even stays a struct-of-arrays or
//  becomes an ECS -- is yours. Change the private section freely. As long as
//  the public accessors keep returning sane data, the renderer keeps working
//  and you never touch my code.
// ============================================================================

enum class Tile : uint8_t {
    Grass,
    Sand,
    Stone,
    Water,   // non-walkable in the demo sim
    Count
};

struct Agent {
    // Position in TILE coordinates (fractional -> smooth sub-tile movement).
    float x  = 0.0f, y  = 0.0f;
    // Position at the START of the current tick. The renderer interpolates
    // between (px,py) and (x,y) so motion is smooth even though the sim
    // ticks at a fixed 60 Hz regardless of framerate. This is the only reason
    // these two fields exist -- gut them if you change how you interpolate.
    float px = 0.0f, py = 0.0f;

    // --- demo-only movement state; delete when you write the real thing ---
    float tx = 0.0f, ty = 0.0f;   // current target tile
};

class World {
public:
    World(int width, int height, uint64_t seed);

    // Advance the simulation by exactly `dt` seconds.
    // Called at a FIXED rate by the platform loop. `dt` is always 1/60 s here.
    void update(float dt);

    // ---- read-only views for the renderer (the whole public contract) ----
    int  width()  const { return w_; }
    int  height() const { return h_; }
    Tile tileAt(int x, int y) const { return tiles_[y * w_ + x]; }
    bool solid(int x, int y)  const;                 // e.g. water blocks movement
    const std::vector<Agent>& agents() const { return agents_; }

private:
    // ---- INTERNALS: yours to redesign entirely ----
    int w_, h_;
    std::vector<Tile>  tiles_;    // flat grid, indexed y*w_ + x
    std::vector<Agent> agents_;
    uint64_t rng_;

    uint32_t next_random();       // tiny deterministic RNG (xorshift)
    void pick_new_target(Agent& a);
};
