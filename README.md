
--------------------------
### INTRODUCTION
--------------------------

WTEngine (What the Engine?) aims to be an object oriented game engine that can be used to make a simple 2D action game.
The engine is written in C++ and uses the [Allegro Game Library](https://liballeg.org) and [PhysFS](https://www.icculus.org/physfs/).

API Documentation:  https://www.wtfsystems.net/docs/wtengine/index.html

For an example of the API in action, see the [demo game code](https://github.com/wtfsystems/wtengine/blob/master/src/wte_demo/wte_demo.cpp).

#### FEATURES:
 - Internal messaging queue
 - Programmable menu interface
 - Memory manager for in-game objects
 - Global variable maps for game/engine settings
 - Music & audio playback via messages
 - Some multi-threading of tasks
 - Buildable debug modes

#### TODO:
 - Create a component to handle sample playing (to prevent message spam)
 - Finish up input interface
 - Create menus for audio & input config
 - Loading in additional scripts
 - Gamepad support
 - Code review / testing / documentation (ongoing)
 - Improve cmake config

#### DEMO:

A compiled demo can be found in the [releases](https://github.com/wtfsystems/releases) repository.

--------------------------
### HOW TO USE
--------------------------

(need to update this section...)

The main loop for the game engine is implemented in wte_main.  You then extend this class to implement your own custom game.

#### MESSAGING:

All in-engine communication is done with the messaging queue.  Messages can be used to change game settings, control the game menus, or have entities interact with each other.  Messages can also be set with a timer value to be processed later.  When starting a new game a scriped file of messages is loaded to process the in-game events.

##### Message format:

| Timer (optional) | System | To (optional) | From (optional) | Command | Arguments |
| ----- | ------ | -- | ---- | ------- | --------- |
| Timer value to run at (-1 for instant) | The system to process the message | To entity | From entity | Command to exectute | Arguments for command |

#### ENTITIES/COMPONENTS:

Entities are in-game objects, and components are the data containers that make them up.  The engine tracks these in an internal database, and user-defined components can be created by extending the base object.  This data can then be fetched in various ways by entity ID or component type.

#### SYSTEMS:

Systems are the logic members that gets processed each step through the game loop.  The engine has a few pre-defined systems for use, but user created ones are  possible as well.  The input system is also extended to implement custom handling.
