__*NOTE:*__  Working on refactoring to build proper library

Seems the main build issue is with the entity mgr get/set templates.
Working on a test app to get the correct instantiation.

The old version (*everything in headers*) can be found [here](https://github.com/wtfsystems/the_dump/blob/master/wtengine_pre_lib.zip)

----

WTEngine (What the Engine?) is a cross-platform game engine based on [ECS](https://en.wikipedia.org/wiki/Entity_component_system) design that can be used to make a 2D action game.
The engine is written in C++17 and uses the [Allegro Game Library](https://liballeg.org) and [PhysicsFS](https://www.icculus.org/physfs/).

Documentation:  https://www.wtfsystems.net/docs/wtengine/index.html

For an example of the engine in action, see the [demo game code](https://github.com/wtfsystems/wtengine/blob/master/example/src/wte_demo.cpp).

#### FEATURES:
 - Internal messaging queue
 - Programmable menu interface
 - Memory manager for in-game objects
 - Global variable maps for game/engine settings
 - Threaded music & audio playback via messages
 - Buildable debug modes

#### DEMO:

A compiled demo can be found in the [releases](https://github.com/wtfsystems/releases) repository.
