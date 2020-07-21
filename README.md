
--------------------------
### INTRODUCTION
--------------------------

WTEngine (What the Engine?) is a cross-platform game engine based on [ECS](https://en.wikipedia.org/wiki/Entity_component_system) design that can be used to make a simple 2D action game.
The engine is written in C++17 and uses the [Allegro Game Library](https://liballeg.org) and [PhysicsFS](https://www.icculus.org/physfs/).

API Documentation:  https://www.wtfsystems.net/docs/wtengine/index.html

For an example of the API in action, see the [demo game code](https://github.com/wtfsystems/wtengine/blob/master/src/wte_demo/wte_demo.cpp).

#### FEATURES:
 - Internal messaging queue
 - Programmable menu interface
 - Memory manager for in-game objects
 - Global variable maps for game/engine settings
 - Threaded music & audio playback via messages
 - Buildable debug modes

#### DEMO:

A compiled demo can be found in the [releases](https://github.com/wtfsystems/releases) repository.

--------------------------
### HOW TO USE
--------------------------

#### IMPLEMENTATION:

...

-----

#### MENUS:

...

-----

#### CONFIGURATION VARIABLES:

...

-----

#### MESSAGING:

All in-engine communication is done with the messaging queue.  Messages can be used to change game settings, control the game menus, or have entities interact with each other.  Messages can also be set with a timer value to be processed later.  When starting a new game a scriped file of messages is loaded to process the in-game events.

[List of built-in messages](https://github.com/wtfsystems/wtengine/blob/master/src/wte/MESSAGES.md)

##### Message format:

| Timer (optional) | System | To (optional) | From (optional) | Command | Arguments |
| ----- | ----- | ----- | ----- | ----- | ----- |
| Timer value to run at (-1 for instant) | The system to process the message | To entity | From entity | Command to exectute | Arguments for command |

-----

#### ENTITIES/COMPONENTS:

Entities are in-game objects, and components are the data containers that make them up.  The engine tracks these in an internal database, and user-defined components can be created by extending the base object.  This data can then be fetched in various ways by entity ID or component type.

-----

#### SYSTEMS:

Systems are the logic members that get processed each step through the game loop.  The engine has a few pre-defined systems for use, but user created ones are possible as well.
