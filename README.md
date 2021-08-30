## About

__WTEngine__ (What the Engine?) is a cross-platform game engine based on [ECS](https://en.wikipedia.org/wiki/Entity_component_system) design that can be used to make a 2D action game.
The engine is written in C++17 and uses the [Allegro Game Library](https://liballeg.org) and [PhysicsFS](https://www.icculus.org/physfs/).

#### API Documentation:
https://www.wtfsystems.net/docs/wtengine/index.html

For an example of the engine in action, see the [demo game code](https://github.com/wtfsystems/wtengine/blob/master/example/src/wte_demo.cpp).

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

## Example Game

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

-----

#  Manual

-----

__NOTE:__ _The following is incomplete, outdated, and is a work in progress._

-----

## Index

-----

## Main Engine Object

Creating a game is done by extending the __wte_main__ object and implementing the members below.


*Required:*
```
virtual void load_menus(void) - Define menus here
virtual void load_systems(void) - Load systems here
virtual void new_game(void) - New game process
virtual void end_game(void) - End game process
```

*Optional:*
```
virtual void on_menu_open(void) - Events when a menu opens
virtual void on_menu_close(void) - Events when a menu closes
virtual void out_of_focus(void) - Events if the game window is clicked out of
virtual void back_in_focus(void) - Events when the game window is brought back into focus
```

In your __main.cpp__ file delcare the object as such:
```
//  Initialize game object.
wte_demo my_game(argc, argv);
//  Run the game.
my_game.do_game();
```

The command line arguments must be passed in for PhysicsFS.

When defining the constructor for your object, pass the window's display title as well:
```
wte_demo::wte_demo(int argc, char **argv) : wte_main(argc, argv, "WTE Demo")
```

See the demo files __main.cpp__ and __wte_demo.cpp__ for an example of an implementation.

-----

## Build Flags

All build flags are defined in __wtengine/_globals/_defines.hpp__

-----

#### WTE_TICKS_PER_SECOND
By default the engine will tick at 60 iterations a second.  You can change the rate using this build flag.  The value is a float.

-----

#### WTE_BUILD_DEBUG
Build debug logging.

-----

#### WTE_MAX_PLAYING_SAMPLES
Sets the maximum number of simultaneous playing samples.  Default is eight.

-----

#### WTE_NO_MAGIC_PINK
By default the engine will use "magic pink" (RGB 255, 0, 255) for transparency.  Define this to disable the feature.

-----

## Entities / Components / Systems

WTEngine stores game objects in the __entity_manager__ class.

#### Entities:

Entities are any and all in-game objects, such as the player, background, enemies, even the text showing the score.  When creating a new entity the engine will assign a unique ID number and unique name.  If the engine is unable to create an entity the error code __WTE_ENTITY_ERROR__ is returned.  You can then change the name of the entity if desired.  When setting the name the engine will verify it does not already exist.

*Creating an entity:*
```
entity_id e_id = world.new_entity();
```

*Setting an entity name:*
```
world.set_name(e_id, "some_unique_name");
```

*Get entity name by ID:*
```
std::string player_name = world.get_name(plr_id);
```

*Get entity ID by name:*
```
entity_id e_id = world.get_id("some_entity");
```

-----

#### Components:

Components are the data objects that make up each entity.  Examples would be the location on the screen, sprite data, hitpoints, etc.  After creating an entity you can assign components to it using its Entity ID.  The engine has seperate functions for reading and setting component data.  You can also create your own components by extending the base __component__ object.

*Adding a component:*
```
mgr::world::add_component<cmp::team>(e_id, 0);
```

*Setting a component's data:*
```
mgr::world::set_component<cmp::overlay>(e_id)->hide();
```

*Reading a component's data:*
```
if(mgr::world::get_component<health>(plr_id)->hp <= 0)
```

-----

#### Systems:

Systems are functions that are processed each iteration when the game is running.  These are designed to obtain all components of a certain type and process them.  For example, the movement system will look for all entities with a velocity component, verify they have a location component, and update their posistion based on the velocity value.  You can also create your own systems by extending the base __system__ object.  You then load all systems into the __system_manager__ in order they will be processed.

*Example:*
```
void wte_demo::load_systems(void) {
    mgr::systems::add(std::make_unique<sys::movement>());
    mgr::systems::add(std::make_unique<sys::colision>());
    mgr::systems::add(std::make_unique<sys::logic>());
    mgr::systems::add(std::make_unique<sys::animate>());
}
```

The __entity_manager__ has members for reading or setting all components of a type, see its documentation for more information.

-----

##  Components with Custom Functions

Certain components within the engine allow to have custom functionality assigned.  Below is the required function signatures for defining these components.  Please see their individual documentation for more information.

-----

####  Background & Overlay
Define custom animation processes for backgrounds & overlays.
```
const entity_id&
```
- __const entity_id&__ - Reference to the Entity ID associated with the calling component.

-----

####  Dispatcher
Define entity message processing.
```
const entity_id&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.

-----

####  Ai
Define entity logic.  Has seperate calls for enabled and disabled entities.
```
const entity_id&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.

-----

OLD

####  Input_directional
Directional input handling.
```
const entity_id&, const float&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __const float&__ - Direction in radians.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.

-----

OLD

####  Input_button
Button input handling.
```
const entity_id&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.

-----

## Menus

OLD WIP

By default WTEngine creates two empty menus, __main_menu__ and __game_menu__.  You can then create additional menus as necessary and add items to them.  See the documentation on the individual menu items for their usage and  functionality.  All menus are stored in the __menu_manager__ object.

*Creating a new menu:*
```
mnu::menu my_menu = mnu::menu("settings", "Settings");
```

*Adding an item using set_menu:*
```
menus.set_menu("main_menu")->add_item(wte_menu_action("Settings", "open_menu", "settings"));
```

*Adding an item directly:*
```
my_menu.add_item(wte_menu_action("Return", "close_menu"));
```

You can also set the background and font color of the menus as such:
```
menus.set_menu_color(ALLEGRO_COLOR font, ALLEGRO_COLOR background);
```
See the documentation on Allegro's color structures for information on the passing values.

A custom menu size can be also be set:
```
menus.set_menu_size(200, 200, 10);
```
The values are width, height and padding in order.  The engine will default to 500x400x32 if not called.

-----

## Messaging and List of Built-In Commands

All in-engine communication is done with the messaging queue.  Messages can be used to change game settings, control the game menus, or have entities interact with each other.  Messages can also be set with a timer value to be processed later.  The __message_manager__ stores messages and keeps track of when they should be processed.

When starting a new game, a file containing messages in binary format can be passed for processing.  The engine also has a command (see list below) that can be used to load additional messages.  These messages have their timer values added to the current engine time when loaded.

A tool named *csv2sdf* is included to convert files in CSV format to be readable by the engine.

-----

#### Message format:

| Timer (optional) | System | To (optional) | From (optional) | Command | Arguments |
| ----- | ----- | ----- | ----- | ----- | ----- |
| Timer value to run at (-1 for instant) | The system to process the message | To entity | From entity | Command to exectute | Arguments for command |

Arguments are split with a semicolon ;

-----

#### Examples:
*Send an instant message to pause music:*
```
mgr::messages::add_message(message("audio", "pause_music", ""));
```
*Delete my_entity at timer value 100:*
```
mgr::messages::add_message(message(100, "spawner", "delete", "my_entity"));
```
*Communication between entities:*
```
mgr::messages::add_message(message("entities", "first_entity", "second_entity", "command", "argument;another"));
```

-----

Needs review

#### Built-In Messages

| Description | System | Commmand | Arguments |
| ----------- | ------ | -------- | --------- |
| Exit engine | system | exit | none |
| Display an alert | system | alert | (1) String of alert to display |
| Start new game | system | new_game | (1 - optional) Filename of game data |
| End current game | system | end_game | none |
| Open menu | system | open_menu | (1) Name of menu to open |
| Close menu | system | close_menu | Close current menu (1 - optional) "all" to close all opened menus |
| Enable a system | system | enable_system | (1) Name of system |
| Disable a system | system | disable_system | (1) Name of system |
| Set engine config variable | system | set_engcfg | (1) "var=val" Variable to set and the value |
| Set game config variable | system | set_gamecfg | (1) "var=val" Variable to set and the value |
| Reconfigure the display | system | reconf_display | none |
| Enable/disable FPS counter | system | fps_counter | (1) "on/off" |
| Load a script from file | system | load_script | (1) Filename of script |
| Control music looping | audio | music_loop | (1) "enable/disable" |
| Play music | audio | play_music | (1) Filename to play |
| Stop music | audio | stop_music | none |
| Pause music | audio | pause_music | none |
| Unpause music | audio | unpause_music | none |
| Load a sample | audio | load_sample | (1) Filename of sample |
| Unload a sample | audio | unload_sample | (1) Sample name or "all" to unload all |
| Play a sample | audio | play_sample | (1) Sample name (2) Mode "once" or "reference name" for looping (3 - optional) Gain 0.0 - 1.0 (4 - optional) Pan -1.0 - 1.0 (5 - optional) Speed 0.00001 - 2.0 ** |
| Stop playing a sample | audio | stop_sample | none |
| Clear sample instances | audio | clear_instances | none |
| Play voice | audio | play_voice | (1) Filename to play |
| Stop voice | audio | stop_voice | none |
| Pause voice | audio | pause_voice | none |
| Unpause voice | audio | unpause_voice | none |
| Control ambiance loop | audio | ambiance_loop | (1) "enable/disable" |
| Play ambiance | audio | play_ambiance | (1) Filename to play |
| Stop ambiance | audio | stop_ambiance | none |
| Pause ambiance | audio | pause_ambiance | none |
| Unpause ambiance | audio | unpause_ambiance | none |
| Set volume levels (from engine cfg variables) | audio | set_volume | none |
| Create a new entity | spawner | new | (1) Name of entity spawn template (2 or more) Additional arguments for spawning entity |
| Delete an entity | spawner | delete | Name of entity to delete |

** NOTE:  When playing a sample to set the gain, pan, or speed you must list all previous arguments in order, even if you are not passing data.

*Example:*
```
some_sample;loop_ref;;0.4
```

This loads a looping sample with a pan value, but no gain or speed.

-----

## Setting up PhysicsFS

To set up [PhysicsFS](https://www.icculus.org/physfs/) and configure the engine to load from a location, call the following in your __main.cpp__ file before calling the game object:

```
wte_main::add_file_location("data.zip");
```

You can make multiple calls to load from more than one location.  That's all there is to it!

-----

Needs review

## Setting up the Renderer

The __render_manager__ requires some parameters to be set up in your __main.cpp__ file before calling the main engine object.

#### Set the arena
Set the arena size in pixels.  This is a bitmap that is used for drawing the visible game area to.  It can later be scaled up or down as necessary with the __scale_factor__ engine variable.
```
mgr::render_manager::set_arena_size(1920, 1080);
```

#### Set the title screen
Load a file to be used as the main title screen.  This will be streched to fit the display size.  If no file is provided, a black title screen will be drawn.
```
mgr::render_manager::set_title_screen("title.bmp");
```

#### Set the background
Load a file to be used as the in-game background.  If the arena size is smaller than the display size, the background will be visible.  This will be streched to fit the display size.  If no file is provided, a black background will be drawn.
```
mgr::render_manager::set_background_screen("title.bmp");
```

-----

Needs review

##  Spawning Entities

Spawning new entities while the engine is running is handled by the __spawn_manager__.  When setting up a new game, you can add functions to the __spawn_manager__ to be called later.

Adding to the spawner is done as such:
```
mrg::spawner::add_spawn(spawn_name, number_of_arguments, function)
```

Note that the number of arguments assigned must match what the calling message contains, but not counting the first argument.  For example, if the message contains a total of 6 arguments, then pass the number 5.  This is because the first argument of the message is used for searching the spawn_name value.  If a number of arguments is being passed that the __spawn_manager__ did not expect, the message will be ignored.

The function being passed is used to create the new entity and needs the following signature:

```
const entity_id&, const msg_arg_list&
```
- __const entity_id&__ - Reference to the Entity ID associated with the calling component.
- __const msg_arg_list&__ - Reference to the message arguments in vector format.

Spawning or deleting entities is then performed by passing a message.  See the section on messaging for more information.

-----

Needs review

## Variable Maps

The variable maps are a method to dynamically create and store variables for use by the game engine.  WTEngine has two variable maps:  __game_cfg__ and __engine_cfg__.  These are stored as strings, but can be type-casted to return as specific data types.  Be careful when using these as they can return unexpected results if misused!

The __engine_cfg__ map should be used for storing configuration variables related to the engine, while __game_cfg__ is used for variables related to your game.  The __game_cfg__ map also has members for performing simple math, see it's documentation for more information.

*Registering a variable:*
```
engine_cfg::reg("display_mode=windowed")
```

*Or:*
```
engine_cfg::reg("display_mode", "windowed")
```

*Setting a registered variable:*
```
engine_cfg::set("draw_fps=1")
```

*Retrieving data as a string:*
```
game_cfg::get("score")
```

*Retrieving data as a casted type:*
```
game_cfg::get<int>("lives")
```

*Performing math:*
```
game_cfg::subtract<int>("lives", 1);
```

The messaging system can also be used to set these variables.  See the documentation page on messaging for more information.

-----

Needs review

#### Saving data:
Both variable maps can save their data, but the functionality is different for each.

The __engine_cfg__ map saves all registered data in __settings.cfg__ and is saved/loaded with the following commands:

*Load engine config data:*
```
engine_cfg::load();
```

*Save engine config data:*
```
engine_cfg::save();
```

The __game_cfg__ map is designed to store only the registered variables you specifically tell it to save.  By default they will be saved to __game.cfg__ but this can also be changed.  When setting a new filename it becomes the active file and subsequent save commands will write to this file.  This allows you to store seperate data into different files.

*Setting a save file:*
```
game_cfg::set_game_cfg_file("my_save.cfg")
```

*Clearing old saved data:*
```
game_cfg::clear_save();
```

*Saving data:*
```
game_cfg::save("hiscore");
```

-----

Needs review

#### Engine Config Variables Created by the Engine:

| Name | Defined In | Description |
| ---- | ---------- | ----------- |
| draw_fps | wte_main.hpp | 0 = don't draw FPS, 1 = draw FPS |
| display_mode | wte_display.hpp | windowed or windowed_full_screen  |
| scale_factor | wte_display.hpp | Scale factor for drawing the arena |
| vsync | wte_display.hpp | 0 (default) = Use gfx driver settings, 1 = on, 2 = off |
| main_vol | audio_manager.hpp | The main volume level (0.0f - 1.0f) |
| mixN_vol | audio_manager.hpp | The volume level of individual mixers (N = 1 - 4) (0.0f - 1.0f) |
| set_a_key_KEY | wte_input_map.hpp | Set A key bind (KEY = key to bind) |
| set_b_key_KEY | wte_input_map.hpp | Set B key bind (KEY = key to bind) |
| joy_ACTION | wte_input_map.hpp | Joystick input binding (ACTION = joystick action to bind) |

-----

End
