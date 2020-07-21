## Build Flags

All build flags are defined in wte_global_defines.hpp

-----

#### WTE_TICKS_PER_SECOND
By default the engine will tick at 60 iterations a second.  You can change this using this build flag.  The value is a float.

-----

#### WTE_DEBUG_MODE
Set level of debugging.
- 0 - No debugging (default)
- 1 - Display timer counter
- 2 - Message logging
- 3 - Draw hitboxes 
- 9 - Turn on all logging

-----

#### WTE_INPUT_MODE
Set the input mode.  Configures how the engine reads sticks on a gamepad.
- 0 - Digital
  - D-pad and sticks record 8-way directions.
  - D-pad and left stick are the same.
  - Analogue triggers are read as button presses.
- 1 - Analogue
  - D-pad only registers button presses, read full data from sticks.
  - Analogue triggers have their data stored.

-----

#### WTE_NO_PRUNE
By default the engine will delete timed messages that were not processed.  Define this to disable the feature.

-----

#### WTE_NO_MAGIC_PINK
By default the engine will use "magic pink" (RGB 255, 0, 255) for transparency.  Define this to disable the feature.
