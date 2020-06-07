
--------------------------
### INTRODUCTION
--------------------------

WTEngine (What the Engine?) aims to be an object oriented game engine that can be used to make a simple 2D action game.
The engine is written in C++ and uses the [Allegro Game Library](https://liballeg.org) and [PhysFS](https://www.icculus.org/physfs/).

Built and tested with [MSYS2](https://www.msys2.org/)

API Documentation:  https://www.wtfsystems.net/docs/wtengine/index.html

#### FEATURES:
 - Internal messaging queue 
 - Programmable menu interface
 - Memory manager for in-game objects
 - Music & audio playback via messages
 - Some multi-threading of tasks
 - Buildable debug modes

#### TODO:
 - Resolution & window settings
 - SFX & input debugging
 - Code review / documentation
 - Improve cmake config

#### DEMO:

A compiled demo can be found in the [releases](https://github.com/wtfsystems/releases) repository.

--------------------------
### HOW TO USE
--------------------------

The main loop for the game engine is implemented in wte_main.  The per-new-game functions for this class are completed in a seperate source file.  Certain systems are then extended to implement your game.

#### MESSAGING:

All in-engine communication is done with the messaging queue.  Messages can be used to change game settings, control the game menus, or have entities interact with each other.  Messages can also be set with a timer value to be processed later.

##### Message format:

| Timer (optional) | System | To (optional) | From (optional) | Command | Arguments |
| ----- | ------ | -- | ---- | ------- | --------- |
| Timer value to run at (-1 for instant) | The system to process the message | To entity | From entity | Command to exectute | Arguments for command |

#### ENTITIES/COMPONENTS:

Entities are in-game objects, and components are the data containers that make them up.  The engine tracks these in an internal database, and user-defined components can be created by extending the base object.  This data can then be fetched in various ways by entity ID or component type.

#### SYSTEMS:

Systems are the logic members that gets processed each step through the game loop.  You can extend these classes to implement your game code.
