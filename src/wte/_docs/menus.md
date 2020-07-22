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
