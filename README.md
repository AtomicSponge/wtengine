## About

__WTEngine__ (What the Engine?) is a cross-platform game engine based on [ECS](https://en.wikipedia.org/wiki/Entity_component_system) design that can be used to make a 2D action game.
The engine is written in C++17 and uses the [Allegro Game Library](https://liballeg.org) and [PhysicsFS](https://www.icculus.org/physfs/).

#### Documentation:
https://www.wtfsystems.net/docs/wtengine/index.html

For an example of the engine in action, see the [demo game code](https://github.com/wtfsystems/wtengine/blob/master/example/src/wte_demo.cpp).

#### Features:
 - Internal messaging queue
 - Programmable menu interface
 - Memory manager for in-game objects
 - Global variable maps for game/engine settings
 - Threaded music & audio playback via messages
 - Buildable debug modes

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

## Example

Requires the library to be installed to build.

```
cd example
cmake .
make
```

No install process, just run the created *wte_demo* executable.

A compiled demo for Windows can be found in the [releases](https://github.com/wtfsystems/releases) repository.  Binary data to play the demo is included.

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