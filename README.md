
--------------------------
### INTRODUCTION:
--------------------------

WTEngine (What the Engine?) aims to be an object oriented game engine that can be used to make a simple 2D action game.
The engine is written in C++ and uses the Allegro Game Library.

Allegro can be found at:  https://liballeg.org

**Features:**
 - Internal messaging queue 
 - Memory manager for in-game objects
 - Some multi-threading of tasks

**TODO:**
 - Finish basic menu system implementation
 - Implement audio
 - Implement sprites

--------------------------
### DEMO:
--------------------------

A compiled demo can be found in the [releases](https://github.com/wtfsystems/releases) repository.

--------------------------
### HOW TO USE:
--------------------------

The main loop for the game engine is implemented in wte_main.  The per-new-game functions for this class are completed in a seperate source file.  Certain systems are then extended to implement your game.  For a list of all members and their function, build the source documentation with [Doxygen](http://doxygen.nl/)

--------------------------
### MESSAGING:
--------------------------

All in-engine communication is done with the messaging queue.  Messages can be used to change game settings, control the game menus, or have entities interact with each other.  Messages can also be set with a timer value to be processed later.

--------------------------
### ENTITIES/COMPONENTS:
--------------------------

Entities are in-game objects, and components are the data containers that make them up.  The engine tracks these in an internal database, and user-defined components can be created by extending the base object.  This data can then be fetched in various ways by entity ID or component type.

--------------------------
### SYSTEMS:
--------------------------

Systems are the logic members that gets processed each step through the game loop.  
