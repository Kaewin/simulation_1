# Valdaryn — vertical slice 0

A running skeleton: fixed-timestep sim loop + raylib rendering, with a clean
seam so the simulation is yours and the platform layer is disposable.

## Build (Linux)

One-time dependency install (raylib builds from source and needs these headers):

```sh
sudo apt install build-essential cmake \
  libasound2-dev libx11-dev libxrandr-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
  libxinerama-dev libwayland-dev libxkbcommon-dev
```

Then:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/valdaryn
```

First configure downloads and builds raylib 5.5 (~30s). After that, rebuilds
are fast — only your files recompile.

## The map

```
CMakeLists.txt     build + raylib dependency        (platform — ignore it)
src/main.cpp       window + fixed-timestep loop      (platform — ignore it)
src/render.cpp     draws a World with raylib         (platform — ignore it)
src/render.hpp     renderer interface                (platform — ignore it)
src/world.hpp      THE SEAM — the sim/platform contract
src/world.cpp      the simulation                    ← YOURS. Start here.
```

## What's running

A generated tile grid with a dozen agents wandering across it, avoiding water.
The agents move at a fixed 60 Hz and the renderer interpolates between ticks, so
motion is smooth at any framerate. All of it is placeholder — `world.cpp` exists
only so you have something alive on screen to replace.

## Where you take it

`world.cpp` is the file to gut. The contract in `world.hpp` (`update(dt)` plus a
few const accessors) is all the platform layer knows about your simulation, so
you can redesign the internals — data layout, agent representation, spatial
structure — without ever touching my code.
