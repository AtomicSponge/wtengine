## Rendering

The __render_manager__ requires some parameters to be set up in your __main.cpp__ file before calling the main engine object.

#### Set the arena
Set the arena size in pixels.  This can later be scaled up or down as necessary.
```
mgr::render_manager::set_arena_size(1920, 1080);
```

#### Set the title screen
Load a file to be used as the main title screen.  This will be streched to fit the display size.
```
mgr::render_manager::set_title_screen("title.bmp");
```

#### Set the background
Load a file to be used as the in-game background.  If the arena size is smaller than the display size, the background will be visible.  This will be streched to fit the display size.
```
mgr::render_manager::set_background_screen("title.bmp");
```
