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
virtual void handle_custom_sys_msg(const message&) - Define custom message handling
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
