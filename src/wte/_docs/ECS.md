## Entities / Components / Systems

#### ENTITIES/COMPONENTS:

Entities are in-game objects, and components are the data containers that make them up.  The engine tracks these in an internal database, and user-defined components can be created by extending the base object.  This data can then be fetched in various ways by entity ID or component type.

-----

#### SYSTEMS:

Systems are the logic members that get processed each step through the game loop.  The engine has a few pre-defined systems for use, but user created ones are possible as well.
