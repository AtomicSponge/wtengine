/*!
 * \brief WTEngine | File:  wte_global_defines.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Global defines for WTEngine.
 */

#ifndef WTE_GLOBAL_DEFINES_HPP
#define WTE_GLOBAL_DEFINES_HPP

//  Define true & false for macro use.
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

/*!
 * \def WTE_TICKS_PER_SECOND
 * \brief Set the timer rate.
 * Number of tics per second as a float.
 */
#ifndef WTE_TICKS_PER_SECOND
#define WTE_TICKS_PER_SECOND (60.0f)
#endif

/*!
 * \def WTE_DEBUG_MODE
 * \brief Set level of debugging.
 * 
 * 0 - No debugging
 * 1 - Display timer counter
 * 2 - Message logging
 * 3 - Draw hitboxes
 * 
 * 9 - Turn on all logging
 */
#ifndef WTE_DEBUG_MODE
#define WTE_DEBUG_MODE (0)
#endif

/*!
 * \def WTE_NO_PRUNE
 * Enable message pruning if WTE_NO_PRUNE is not defined.
 */
#ifdef WTE_NO_PRUNE
#define WTE_PRUNE_ENABLED FALSE
#else
#define WTE_PRUNE_ENABLED TRUE
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

/*!
 * \def WTE_NO_MAGIC_PINK
 * Enable magic pink for transparency if WTE_NO_MAGIC_PINK is not defined.
 */
#ifndef WTE_NO_MAGIC_PINK
#define WTE_MAGIC_PINK          (al_map_rgb(255, 0, 255))
#endif

#endif
