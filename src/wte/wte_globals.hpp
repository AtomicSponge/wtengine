/*
  WTEngine
  By:  Matthew Evans
  File:  wte_globals.hpp

  See LICENSE.txt for copyright information

  Global defines for WTE engine
*/

#ifndef WTE_GLOBALS_HPP
#define WTE_GLOBALS_HPP

//!Set level of debugging
/*!
  0 - off
  1 - Display timer counter
  2 - Message logging
  9 - Turn on all logging
*/
#ifndef WTE_DEBUG_MODE
#define WTE_DEBUG_MODE 0
#endif

#define WTE_COLOR_BLACK     al_map_rgb(0,0,0)
#define WTE_COLOR_RED       al_map_rgb(255,0,0)
#define WTE_COLOR_GREEN     al_map_rgb(0,255,0)
#define WTE_COLOR_YELLOW    al_map_rgb(255,255,0)
#define WTE_COLOR_BLUE      al_map_rgb(0,0,255)
#define WTE_COLOR_TEAL      al_map_rgb(0,128,128)
#define WTE_COLOR_ORANGE    al_map_rgb(255, 165, 0)
#define WTE_COLOR_WHITE     al_map_rgb(255,255,255)

namespace wte 
{

//!  Set screen width and height
extern const unsigned int screen_width;
extern const unsigned int screen_height;

//!  Set the timer rate
extern const float TICKS_PER_SECOND;

//!  Used for the game flags
enum GLOBAL_STATE_FLAGS {
    IS_RUNNING, GAME_STARTED, GAME_MENU_OPENED, DRAW_HITBOX, DRAW_FPS, MAX_STATE_FLAGS
};

//!  Global flags for game state
extern volatile bool game_flag[MAX_STATE_FLAGS];

//!  Used for in-game player control
enum PLAYERKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_FIRE_1, KEY_FIRE_2, KEY_FIRE_3, MAX_KEY_FLAGS
};

//!  Global flags for key state
extern volatile bool key[MAX_KEY_FLAGS];

} //  end namespace wte

//  Initialize game flags
inline volatile bool wte::game_flag[wte::MAX_STATE_FLAGS] = { false, false, false, false, false };
inline volatile bool wte::key[wte::MAX_KEY_FLAGS] = { false, false, false, false, false, false, false };

#endif
