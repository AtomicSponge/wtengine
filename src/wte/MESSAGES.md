## List of built-in messages
Arguments are split with ";"

| Description | System | Commmand | Arguments |
| ----------- | ------ | -------- | --------- |
| Exit engine | system | exit | none |
| Display an alert | system | alert | (1) String of alert to display |
| Start new game | system | new_game | (1 - optional) Filename of game data |
| End current game | system | end_game | none |
| Open menu | system | open_menu | (1) Name of menu to open |
| Close menu | system | close_menu | (1 - optional) "all" to close all opened menus |
| Enable a system | system | enable_system | (1) Name of system |
| Disable a system | system | disable_system | (1) Name of system |
| Set Engine Config Variable | system | set_engcfg | (1) "var=val" Variable to set and the value |
| Set Game Config Variable | system | set_gamecfg | (1) "var=val" Variable to set and the value |
| Reconfigure the display | system | reconf_display | none |
| Enable/disable FPS Counter | system | fps_counter | (1) "on/off" |
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

** NOTE:  When playing a sample to set the gain, pan, or speed you must include all arguments in order, even if you are not passing data.

Example:  some_sample;loop_ref;;0.4;

This loads a looping sample with a pan value, but no gain or speed.
