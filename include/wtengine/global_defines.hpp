/*!
 * WTEngine | File:  wte_global_defines.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
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
 * Set the input mode.  Default is digital.
 * Configures how the engine reads sticks on a gamepad.
 * 
 * 0 - Digital
 * 1 - Analogue
 */
#ifndef WTE_INPUT_MODE
#define WTE_INPUT_MODE (0)
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

#if WTE_USE_MAGIC_PINK
#define WTE_MAGIC_PINK          (al_map_rgb(255, 0, 255))
#endif

//  Define some colors for use in the engine.
#define WTE_COLOR_BLACK         (al_map_rgb(0,0,0))
#define WTE_COLOR_RED           (al_map_rgb(255,0,0))
#define WTE_COLOR_GREEN         (al_map_rgb(0,255,0))
#define WTE_COLOR_YELLOW        (al_map_rgb(255,255,0))
#define WTE_COLOR_BLUE          (al_map_rgb(0,0,255))
#define WTE_COLOR_TEAL          (al_map_rgb(0,128,128))
#define WTE_COLOR_ORANGE        (al_map_rgb(255, 165, 0))
#define WTE_COLOR_PURPLE        (al_map_rgb(135, 31, 120))
#define WTE_COLOR_DARKPURPLE    (al_map_rgb(48, 25, 52))
#define WTE_COLOR_WHITE         (al_map_rgb(255,255,255))
#define WTE_COLOR_TRANSPARENT   (al_map_rgba(0, 0, 0, 0))

#endif
