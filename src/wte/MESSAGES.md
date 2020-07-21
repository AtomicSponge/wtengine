## List of built-in messages

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
| Control music looping | audio | music_loop | ... |
| Play music | audio | play_music | ... |
| Stop music | audio | stop_music | ... |
| Pause music | audio | pause_music | ... |
| Unpause music | audio | unpause_music | ... |
| Load a sample | audio | load_sample | ... |
| Unload a sample | audio | unload_sample | ... |
| Play a sample | audio | play_sample | ... |
| Stop playing a sample | audio | stop_sample | ... |
| Clear sample instances | audio | clear_instances | ... |
| Play voice | audio | play_voice | ... |
| Stop voice | audio | stop_voice | ... |
| Pause voice | audio | pause_voice | ... |
| Unpause voice | audio | unpause_voice | ... |
| Control ambiance loop | audio | ambiance_loop | ... |
| Play ambiance | audio | play_ambiance | ... |
| Stop ambiance | audio | stop_ambiance | ... |
| Pause ambiance | audio | pause_ambiance | ... |
| Unpause ambiance | audio | unpause_ambiance | ... |
| Set volume levels | audio | set_volume | ... |
| Create a new entity | spawner | new | ... |
| Delete an entity | spawner | delete | ... |
