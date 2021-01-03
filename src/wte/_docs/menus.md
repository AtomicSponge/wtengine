## Menus

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
