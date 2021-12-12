/*!
 * WTEngine | File:  _defines.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_GLOBAL_DEFINES_HPP
#define WTE_GLOBAL_DEFINES_HPP

//  Enable math defines for entire engine.
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//  Define true & false for macro use.
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#ifdef WTE_REQUIRE_OPENGL_LATEST
#define WTE_OPENGL_LATEST TRUE
#else
#define WTE_OPENGL_LATEST FALSE
#endif

/*!
 * Set the timer rate.
 * Number of ticks per second as a float.
 */
#ifndef WTE_TICKS_PER_SECOND
#define WTE_TICKS_PER_SECOND (60.0f)
#endif

/*!
 * Enable debug mode
 */
#ifdef WTE_BUILD_DEBUG
#define WTE_DEBUG_MODE TRUE
#else
#define WTE_DEBUG_MODE FALSE
#endif

/*!
 * Set max number of playing samples.
 */
#ifndef WTE_MAX_PLAYING_SAMPLES
#define WTE_MAX_PLAYING_SAMPLES (8)
#endif

/*!
 * Enable magic pink for transparency if WTE_NO_MAGIC_PINK is not defined.
 */
#ifdef WTE_NO_MAGIC_PINK
#define WTE_USE_MAGIC_PINK FALSE
#else
#define WTE_USE_MAGIC_PINK TRUE
#endif

namespace wte {

/*!
 * \struct WTEBUILDOPTIONS
 * \brief Build options
 */
struct WTEBUILDOPTIONS {
    constexpr static bool wte_opengl_latest = static_cast<bool>(WTE_OPENGL_LATEST);
    constexpr static float ticks_per_sec = static_cast<float>(WTE_TICKS_PER_SECOND);
    constexpr static bool debug_mode = static_cast<bool>(WTE_DEBUG_MODE);
    constexpr static int max_playing_samples = static_cast<int>(WTE_MAX_PLAYING_SAMPLES);
    constexpr static bool use_magic_pink = static_cast<bool>(WTE_USE_MAGIC_PINK);
};
inline constexpr WTEBUILDOPTIONS build_options;

}

#if WTE_USE_MAGIC_PINK
#define WTE_MAGIC_PINK          (al_map_rgb(255,0,255))
#endif

//  Define some colors for use in the engine.
#ifndef WTE_NO_COLORS
#define WTE_COLOR_BLACK         (al_map_rgb(0,0,0))
#define WTE_COLOR_RED           (al_map_rgb(255,0,0))
#define WTE_COLOR_GREEN         (al_map_rgb(0,255,0))
#define WTE_COLOR_YELLOW        (al_map_rgb(255,255,0))
#define WTE_COLOR_BLUE          (al_map_rgb(0,0,255))
#define WTE_COLOR_TEAL          (al_map_rgb(0,128,128))
#define WTE_COLOR_ORANGE        (al_map_rgb(255,165,0))
#define WTE_COLOR_PURPLE        (al_map_rgb(135,31,120))
#define WTE_COLOR_DARKPURPLE    (al_map_rgb(48,25,52))
#define WTE_COLOR_WHITE         (al_map_rgb(255,255,255))
#define WTE_COLOR_TRANSPARENT   (al_map_rgba(0,0,0,0))
#endif

//  Define default keys
#ifndef WTE_KEY_MENU_DEFAULT
#define WTE_KEY_MENU_DEFAULT            ALLEGRO_KEY_ESCAPE
#endif
#ifndef WTE_KEY_TOGGLE_HITBOX
#define WTE_KEY_TOGGLE_HITBOX           ALLEGRO_KEY_F2
#endif

/*
 * Player 1 keyboard inputs
 */
#ifndef WTE_P1_KEY_UP_DEFAULT
#define WTE_P1_KEY_UP_DEFAULT           ALLEGRO_KEY_W
#endif
#ifndef WTE_P1_KEY_DOWN_DEFAULT
#define WTE_P1_KEY_DOWN_DEFAULT         ALLEGRO_KEY_S
#endif
#ifndef WTE_P1_KEY_LEFT_DEFAULT
#define WTE_P1_KEY_LEFT_DEFAULT         ALLEGRO_KEY_A
#endif
#ifndef WTE_P1_KEY_RIGHT_DEFAULT
#define WTE_P1_KEY_RIGHT_DEFAULT        ALLEGRO_KEY_D
#endif
#ifndef WTE_P1_KEY_ACTION1_DEFAULT
#define WTE_P1_KEY_ACTION1_DEFAULT      ALLEGRO_KEY_RCTRL
#endif
#ifndef WTE_P1_KEY_ACTION2_DEFAULT
#define WTE_P1_KEY_ACTION2_DEFAULT      ALLEGRO_KEY_ALTGR
#endif
#ifndef WTE_P1_KEY_ACTION3_DEFAULT
#define WTE_P1_KEY_ACTION3_DEFAULT      (0)
#endif
#ifndef WTE_P1_KEY_ACTION4_DEFAULT
#define WTE_P1_KEY_ACTION4_DEFAULT      (0)
#endif
#ifndef WTE_P1_KEY_ACTION5_DEFAULT
#define WTE_P1_KEY_ACTION5_DEFAULT      (0)
#endif
#ifndef WTE_P1_KEY_ACTION6_DEFAULT
#define WTE_P1_KEY_ACTION6_DEFAULT      (0)
#endif
#ifndef WTE_P1_KEY_ACTION7_DEFAULT
#define WTE_P1_KEY_ACTION7_DEFAULT      (0)
#endif
#ifndef WTE_P1_KEY_ACTION8_DEFAULT
#define WTE_P1_KEY_ACTION8_DEFAULT      (0)
#endif

/*
 * Player 2 keyboard inputs
 */
#ifndef WTE_P2_KEY_UP_DEFAULT
#define WTE_P2_KEY_UP_DEFAULT           ALLEGRO_KEY_UP
#endif
#ifndef WTE_P2_KEY_DOWN_DEFAULT
#define WTE_P2_KEY_DOWN_DEFAULT         ALLEGRO_KEY_DOWN
#endif
#ifndef WTE_P2_KEY_LEFT_DEFAULT
#define WTE_P2_KEY_LEFT_DEFAULT         ALLEGRO_KEY_LEFT
#endif
#ifndef WTE_P2_KEY_RIGHT_DEFAULT
#define WTE_P2_KEY_RIGHT_DEFAULT        ALLEGRO_KEY_RIGHT
#endif
#ifndef WTE_P2_KEY_ACTION1_DEFAULT
#define WTE_P2_KEY_ACTION1_DEFAULT      ALLEGRO_KEY_LCTRL
#endif
#ifndef WTE_P2_KEY_ACTION2_DEFAULT
#define WTE_P2_KEY_ACTION2_DEFAULT      ALLEGRO_KEY_ALT
#endif
#ifndef WTE_P2_KEY_ACTION3_DEFAULT
#define WTE_P2_KEY_ACTION3_DEFAULT      (0)
#endif
#ifndef WTE_P2_KEY_ACTION4_DEFAULT
#define WTE_P2_KEY_ACTION4_DEFAULT      (0)
#endif
#ifndef WTE_P2_KEY_ACTION5_DEFAULT
#define WTE_P2_KEY_ACTION5_DEFAULT      (0)
#endif
#ifndef WTE_P2_KEY_ACTION6_DEFAULT
#define WTE_P2_KEY_ACTION6_DEFAULT      (0)
#endif
#ifndef WTE_P2_KEY_ACTION7_DEFAULT
#define WTE_P2_KEY_ACTION7_DEFAULT      (0)
#endif
#ifndef WTE_P2_KEY_ACTION8_DEFAULT
#define WTE_P2_KEY_ACTION8_DEFAULT      (0)
#endif

#endif
