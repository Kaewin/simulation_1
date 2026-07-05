#pragma once
#include <cstdint>
#include <vector>

enum class Tile : uint8_t {
    Grass,
    Sand,
    Stone,
    Water,   
    Count
};

struct Agent {
    float x  = 0.0f, y  = 0.0f;
    float px = 0.0f, py = 0.0f;

    float tx = 0.0f, ty = 0.0f;
};

struct Input {
    float move_x = 0.0f;   
    float move_y = 0.0f;   
};

struct Player {
    float x = 0.0f, y = 0.0f;    
    float px = 0.0f, py = 0.0f;  
};

void update(float dt, const Input& in); 

class World {
public:
    World(int width, int height, uint64_t seed);

    void update(float dt);

    int  width()  const { return w_; }
    int  height() const { return h_; }
    Tile tileAt(int x, int y) const { return tiles_[y * w_ + x]; }
    bool solid(int x, int y)  const;                 
    const std::vector<Agent>& agents() const { return agents_; }

private:
    int w_, h_;
    std::vector<Tile>  tiles_;    
    std::vector<Agent> agents_;
    uint64_t rng_;

    uint32_t next_random();       
    void pick_new_target(Agent& a);
};
