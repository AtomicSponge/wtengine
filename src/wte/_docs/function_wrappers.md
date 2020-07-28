##  Components with Custom Functions

Certain components within the engine allow to have custom functionality assigned.  Below is the required function signatures for defining these components.  Please see their individual documentation for more information.

-----

####  Background & Overlay
Define custom animation processes for backgrounds & overlays.
```
const entity_id&, mgr::entity_manager&, const int64_t&
```
- __const entity_id&__ - Reference to the Entity ID associated with the calling component.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __const int64_t&__ - Reference to the current engine time value.

-----

####  Dispatcher
Define entity message processing.
```
const entity_id&, const message&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __const message&__ - Message to be processed.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.

-----

####  Ai
Define entity logic.  Has seperate calls for enabled and disabled entities.
```
const entity_id&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.

-----

####  Input_directional
Directional input handling.
```
const entity_id&, const float&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __const float&__ - Direction in radians.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.

-----

####  Input_button
Button input handling.
```
const entity_id&, mgr::entity_manager&, mgr::message_manager&, const int64_t&
```
- __entity_id&__ - Reference to the Entity ID associated with the calling component.
- __mgr::entity_manager&__ - Reference to the entity manager.
- __mgr::message_manager&__ - Reference to the message manager.
- __int64_t&__ - Reference to the current engine time value.
