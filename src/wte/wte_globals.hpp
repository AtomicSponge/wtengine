/*
  WTEngine
  By:  Matthew Evans
  File:  wte_globals.hpp

  See LICENSE.txt for copyright information

  Global defines for WTE engine
*/

#ifndef WTE_GLOBALS_HPP
#define WTE_GLOBALS_HPP

#define WTE_DEBUG_MODE //  Comment out to turn off debug mode

#define WTE_ARENA_WIDTH     768
#define WTE_ARENA_HEIGHT    1024

#define WTE_COLOR_BLACK     al_map_rgb(0,0,0)
#define WTE_COLOR_RED       al_map_rgb(255,0,0)
#define WTE_COLOR_GREEN     al_map_rgb(0,255,0)
#define WTE_COLOR_YELLOW    al_map_rgb(255,255,0)
#define WTE_COLOR_BLUE      al_map_rgb(0,0,255)
#define WTE_COLOR_TEAL      al_map_rgb(0,128,128)
#define WTE_COLOR_ORANGE    al_map_rgb(255, 165, 0)
#define WTE_COLOR_WHITE     al_map_rgb(255,255,255)

#define WTE_NUM_GAME_FLAGS 5

namespace wte 
{

//  Set the timer rate
const float TICKS_PER_SECOND = 60.0;

//  Used for the game flags
enum GLOBAL_STATE_FLAGS {
    IS_RUNNING, GAME_STARTED, GAME_MENU_OPENED, DRAW_HITBOX, DRAW_FPS
};

//  Global flags for game state
extern volatile bool game_flag[WTE_NUM_GAME_FLAGS];

} //  end namespace wte

#endif
