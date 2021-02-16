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
