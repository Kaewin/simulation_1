#pragma once
#include "world.hpp"

// Draw the world. `alpha` in [0,1] is the interpolation factor between the
// previous sim tick and the current one, so motion looks smooth regardless of
// framerate. Platform layer -- you shouldn't need to touch this, but it reads
// only through the World accessors, so it can't break when you rewrite the sim.
void render_world(const World& world, float alpha, int tile_px);
