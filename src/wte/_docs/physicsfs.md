## Setting up PhysicsFS

To set up [PhysicsFS](https://www.icculus.org/physfs/) and configure the engine to load from a location, call the following in your __main.cpp__ file before calling the game object:

```
wte_main::add_file_location("data.zip");
```

You can make multiple calls to load from more than one location.  That's all there is to it!
