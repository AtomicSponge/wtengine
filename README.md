#  What the Engine?

__WTEngine__ is a lightweight cross-platform game engine written in C++17 using an [ECS](https://en.wikipedia.org/wiki/Entity_component_system) design.

### Requirements
 - __Build tools__:
    - A working C++ build environment with [CMake](https://cmake.org)
 - __Libraries__:
    - [Allegro Game Library](https://liballeg.org)
    - [PhysicsFS](https://www.icculus.org/physfs/)
    - [OpenGL](https://www.opengl.org) *(2d only - for now)*

WTEngine also uses the library [Dear ImGui](https://github.com/ocornut/imgui) integrated into its codebase.

### Documentation:
 - [Manual](https://github.com/wtfsystems/wtengine/wiki)
 - [API](https://www.wtfsystems.net/docs/wtengine/index.html)
 - [Example game](https://github.com/wtfsystems/wte_demo_01/blob/master/src/wte_demo.cpp)

-----

## Library Installation

Build and installation is handled by [CMake](https://cmake.org/).  To build just the library:
```
git clone https://github.com/wtfsystems/wtengine.git
cd wtengine
cmake .
make
```

Then to install the library run:
```
sudo make install
```

-----

## Troubleshooting

### pkg-config can't find wtengine

Make sure the install location used for pkg-config is in PKG_CONFIG_PATH, example:
```
export PKG_CONFIG_PATH=/usr/local/share/pkgconfig
```

Check __install_manifest.txt__ to see where __wtengine.pc__ was placed.

You can verify pkg-config can locate the engine by:
```
pkg-config --libs --exists wtengine 
```
