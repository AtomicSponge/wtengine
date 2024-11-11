/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_GLOBAL_DEFINES_HPP)
#define SLV_GLOBAL_DEFINES_HPP

#include <string>
#include <vector>
#include <type_traits>

#include <allegro5/allegro.h>

//  Enable math defines for entire engine.
#if !defined(_USE_MATH_DEFINES)
  #define _USE_MATH_DEFINES
#endif

//  Define true & false for macro use.
#if !defined(TRUE)
  #define TRUE (1)
#endif
#if !defined(FALSE)
  #define FALSE (0)
#endif

//  Enable debug mode
#if defined(SLV_BUILD_DEBUG)
  #define SLV_DEBUG_MODE TRUE
#else
  #define SLV_DEBUG_MODE FALSE
#endif

//  Require OpenGL 3.0
#if !defined(SLV_REQUIRE_OPENGL_LATEST)
  #define SLV_OPENGL_LATEST TRUE
#else
  #define SLV_OPENGL_LATEST FALSE
#endif

//  Set the timer rate.
//  Number of ticks per second as a float.
#if !defined(SLV_TICKS_PER_SECOND)
  #define SLV_TICKS_PER_SECOND (60.0f)
#endif

//  Set max number of playing samples.
#if !defined(SLV_MAX_PLAYING_SAMPLES)
  #define SLV_MAX_PLAYING_SAMPLES (12)
#endif

//  Toggle keyboard building
#if !defined(SLV_DISABLE_KEYBOARD)
  #define SLV_USE_KEYBOARD TRUE
#else
  #define SLV_USE_KEYBOARD FALSE
#endif

//  Toggle mouse building
#if !defined(SLV_DISABLE_MOUSE)
  #define SLV_USE_MOUSE TRUE
#else
  #define SLV_USE_MOUSE FALSE
#endif

//  Toggle joystick building
#if !defined(SLV_DISABLE_JOYSTICK)
  #define SLV_USE_JOYSTICK TRUE
#else
  #define SLV_USE_JOYSTICK FALSE
#endif

//  Toggle touch building
#if !defined(SLV_DISABLE_TOUCH)
  #define SLV_USE_TOUCH TRUE
#else
  #define SLV_USE_TOUCH FALSE
#endif

#if !SLV_USE_KEYBOARD && !SLV_USE_MOUSE && !SLV_USE_JOYSTICK && !SLV_USE_TOUCH
  #error Must define at least one input device to be used
#endif

namespace slv {

/*
 * Build flags for configuring the engine.
 */
struct slv_build_options {
  inline constexpr static bool debug_mode = static_cast<bool>(SLV_DEBUG_MODE);
  inline constexpr static bool opengl_latest = static_cast<bool>(SLV_OPENGL_LATEST);
  inline constexpr static float ticks_per_sec = static_cast<float>(SLV_TICKS_PER_SECOND);
  inline constexpr static int max_playing_samples = static_cast<int>(SLV_MAX_PLAYING_SAMPLES);

  //  Input options
  inline constexpr static bool keyboard_enabled = static_cast<bool>(SLV_USE_KEYBOARD);
  inline constexpr static bool mouse_enabled = static_cast<bool>(SLV_USE_MOUSE);
  inline constexpr static bool joystick_enabled = static_cast<bool>(SLV_USE_JOYSTICK);
  inline constexpr static bool touch_enabled = static_cast<bool>(SLV_USE_TOUCH);
};
inline constexpr slv_build_options build_options;

}

#endif
