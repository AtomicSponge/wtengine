/*!
 * wtengine | File:  _defines.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_GLOBAL_DEFINES_HPP
#define WTE_GLOBAL_DEFINES_HPP

#include <type_traits>

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
 * Toggle keyboard building
 */
#ifdef WTE_DISABLE_KEYBOARD
#define WTE_USE_KEYBOARD FALSE
#else
#define WTE_USE_KEYBOARD TRUE
#endif

/*!
 * Toggle mouse building
 */
#ifdef WTE_DISABLE_MOUSE
#define WTE_USE_MOUSE FALSE
#else
#define WTE_USE_MOUSE TRUE
#endif

/*!
 * Toggle joystick building
 */
#ifdef WTE_DISABLE_JOYSTICK
#define WTE_USE_JOYSTICK FALSE
#else
#define WTE_USE_JOYSTICK TRUE
#endif

/*!
 * Toggle touch building
 */
#ifdef WTE_DISABLE_TOUCH
#define WTE_USE_TOUCH FALSE
#else
#define WTE_USE_TOUCH TRUE
#endif

namespace wte {

/*!
 * \struct build_options
 * \brief Build options
 */
struct wte_build_options {
    inline constexpr static bool opengl_latest = static_cast<bool>(WTE_OPENGL_LATEST);
    inline constexpr static float ticks_per_sec = static_cast<float>(WTE_TICKS_PER_SECOND);
    inline constexpr static bool debug_mode = static_cast<bool>(WTE_DEBUG_MODE);
    inline constexpr static int max_playing_samples = static_cast<int>(WTE_MAX_PLAYING_SAMPLES);

    //  Input options
    inline constexpr static bool keyboard_enabled = static_cast<bool>(WTE_USE_KEYBOARD);
    inline constexpr static bool mouse_enabled = static_cast<bool>(WTE_USE_MOUSE);
    inline constexpr static bool joystick_enabled = static_cast<bool>(WTE_USE_JOYSTICK);
    inline constexpr static bool touch_enabled = static_cast<bool>(WTE_USE_TOUCH);
};
inline constexpr wte_build_options build_options;

}

//  Define some colors for use in the engine.
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
