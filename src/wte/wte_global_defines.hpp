/*
  WTEngine
  By:  Matthew Evans
  File:  wte_global_defines.hpp

  See LICENSE.txt for copyright information

  Global defines for WTEngine
*/

#ifndef WTE_GLOBAL_DEFINES_HPP
#define WTE_GLOBAL_DEFINES_HPP

//  Set level of debugging
/*
  0 - off
  1 - Display timer counter
  2 - Message logging
  9 - Turn on all logging
*/
#ifndef WTE_DEBUG_MODE
#define WTE_DEBUG_MODE 0
#endif

//  Enabled audio if WTE_NO_AUDIO is not defined
#ifndef WTE_NO_AUDIO
#define WTE_AUDIO_ENABLED
#endif

//  Endable message pruning if WTE_NO_PRUNE is not defined
#ifndef WTE_NO_PRUNE
#define WTE_PRUNE_ENABLED
#endif

#define WTE_COLOR_BLACK         al_map_rgb(0,0,0)
#define WTE_COLOR_RED           al_map_rgb(255,0,0)
#define WTE_COLOR_GREEN         al_map_rgb(0,255,0)
#define WTE_COLOR_YELLOW        al_map_rgb(255,255,0)
#define WTE_COLOR_BLUE          al_map_rgb(0,0,255)
#define WTE_COLOR_TEAL          al_map_rgb(0,128,128)
#define WTE_COLOR_ORANGE        al_map_rgb(255, 165, 0)
#define WTE_COLOR_PURPLE        al_map_rgb(135, 31, 120)
#define WTE_COLOR_DARKPURPLE    al_map_rgb(48, 25, 52)
#define WTE_COLOR_WHITE         al_map_rgb(255,255,255)

#endif
