##  Spawning Entities

Spawning new entities while the engine is running is handled by the __spawn_manager__.  When setting up a new game, you can add functions to the __spawn_manager__ to be called later.

Adding to the spawner is done as such:
```
spawner.add_spawn(spawn_name, number_of_arguments, function)
```

Note that the number of arguments assigned must match what the calling message contains, but not counting the first argument.  For example, if the message contains a total of 6 arguments, then pass the number 5.  This is because the first argument of the message is used for searching the spawn_name value.  If a number of arguments is being passed that the __spawn_manager__ did not expect, the message will be ignored.

The function being passed is used to create the new entity and needs the following signature:

```
const entity_id&, mgr::entity_manager&, const msg_arg_list&
```
- __const entity_id&__ - Reference to the Entity ID associated with the calling component.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __const msg_arg_list&__ - Reference to the message arguments in vector format.

Spawning or deleting entities is then performed by passing a message.  See the section on messaging for more information.
