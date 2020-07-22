## Entities / Components / Systems

WTEngine stores game objects in the __entity_manager__ class.

#### Entities:

Entities are any and all in-game objects, such as the player, background, enemies, even the text showing the score.  When creating a new entity, then engine will assign a unique ID number and unique name.  You can then change the name of the entity if desired as long as it remains unique.

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

-----

#### Components:

Components are the data objects that make up each entity.  Examples would be the location on the screen, sprite data, hitpoints, etc.  After creating an entity you can assign components to it using its Entity ID.  The engine has seperate functions for reading and setting component data.  You can also create your own components by extending the base __component__ object.

*Adding a component:*
```
wte_new_component(e_id, cmp::visible, false);
```

*Setting a component's data:*
```
wte_set_component(e_id, cmp::sprite)->load_sprite("my_sprite.bmp");
```

*Reading a component's data:*
```
wte_get_component(e_id, cmp::location)->get_x()
```

-----

#### Systems:

Systems are functions that run each iteration when the game is running.  These are designed to obtain all components of a certain type and process them.  For example, the movement system will look for all entities with a velocity component, verify they have a location component, and update their posistion.  You can also create your own systems by extending the base __system__ object.  You then load all systems in order they will be processed.

The entity manager has members for retreving all components of a type, see it's documentation for more information.
