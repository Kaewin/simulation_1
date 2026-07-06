# Valdaryn — vertical slice 0

A running skeleton: fixed-timestep sim loop + raylib rendering

## Build (Linux)

One-time dependency install:

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
