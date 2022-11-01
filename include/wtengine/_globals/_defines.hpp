/*!
 * wtengine | File:  _defines.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_GLOBAL_DEFINES_HPP)
#define WTE_GLOBAL_DEFINES_HPP

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

/*!
 * Enable debug mode
 */
#if defined(WTE_BUILD_DEBUG)
    #define WTE_DEBUG_MODE TRUE
#else
    #define WTE_DEBUG_MODE FALSE
#endif

#if !defined(WTE_REQUIRE_OPENGL_LATEST)
    #define WTE_OPENGL_LATEST TRUE
#else
    #define WTE_OPENGL_LATEST FALSE
#endif

#if !defined(WTE_DISABLE_IMGUI)
    #define WTE_USE_IMGUI TRUE
#else
    #define WTE_USE_IMGUI FALSE
#endif

#if !defined(WTE_IMGUI_LIGHTMODE)
    #define WTE_IMGUI_DARKMODE TRUE
#else
    #define WTE_IMGUI_DARKMODE FALSE
#endif

/*!
 * Set the timer rate.
 * Number of ticks per second as a float.
 */
#if !defined(WTE_TICKS_PER_SECOND)
    #define WTE_TICKS_PER_SECOND (60.0f)
#endif

/*!
 * Set max number of playing samples.
 */
#if !defined(WTE_MAX_PLAYING_SAMPLES)
    #define WTE_MAX_PLAYING_SAMPLES (8)
#endif

/*!
 * Toggle keyboard building
 */
#if !defined(WTE_DISABLE_KEYBOARD)
    #define WTE_USE_KEYBOARD TRUE
#else
    #define WTE_USE_KEYBOARD FALSE
#endif

/*!
 * Toggle mouse building
 */
#if !defined(WTE_DISABLE_MOUSE)
    #define WTE_USE_MOUSE TRUE
#else
    #define WTE_USE_MOUSE FALSE
#endif

/*!
 * Toggle joystick building
 */
#if !defined(WTE_DISABLE_JOYSTICK)
    #define WTE_USE_JOYSTICK TRUE
#else
    #define WTE_USE_JOYSTICK FALSE
#endif

/*!
 * Toggle touch building
 */
#if !defined(WTE_DISABLE_TOUCH)
    #define WTE_USE_TOUCH TRUE
#else
    #define WTE_USE_TOUCH FALSE
#endif

namespace wte {

/*!
 * \struct build_options
 * \brief Build options
 */
struct wte_build_options {
    inline constexpr static bool debug_mode = static_cast<bool>(WTE_DEBUG_MODE);
    inline constexpr static bool opengl_latest = static_cast<bool>(WTE_OPENGL_LATEST);
    inline constexpr static bool use_imgui = static_cast<bool>(WTE_USE_IMGUI);
    inline constexpr static bool imgui_darkmode = static_cast<bool>(WTE_IMGUI_DARKMODE);
    inline constexpr static float ticks_per_sec = static_cast<float>(WTE_TICKS_PER_SECOND);
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
#if !defined(WTE_KEY_MENU_DEFAULT)
    #define WTE_KEY_MENU_DEFAULT            ALLEGRO_KEY_ESCAPE
#endif
#if !defined(WTE_KEY_TOGGLE_HITBOX)
    #define WTE_KEY_TOGGLE_HITBOX           ALLEGRO_KEY_F2
#endif

/*
 * Error checking
 */
#if !WTE_USE_KEYBOARD && !WTE_USE_MOUSE && !WTE_USE_JOYSTICK && !WTE_USE_TOUCH
    #error Must define at least one input device to be used
#endif

/*
 * Define keyboard keys for use in the engine.
 * These just map to the Allegro defines.
 */
#if WTE_USE_KEYBOARD  //  Only define these if keyboard is enabled.
    #if !defined(WTE_KEY_A)
        #define WTE_KEY_A ALLEGRO_KEY_A
    #endif
    #if !defined(WTE_KEY_B)
        #define WTE_KEY_B ALLEGRO_KEY_B
    #endif
    #if !defined(WTE_KEY_C)
        #define WTE_KEY_C ALLEGRO_KEY_C
    #endif
    #if !defined(WTE_KEY_D)
        #define WTE_KEY_D ALLEGRO_KEY_D
    #endif
    #if !defined(WTE_KEY_E)
        #define WTE_KEY_E ALLEGRO_KEY_E
    #endif
    #if !defined(WTE_KEY_F)
        #define WTE_KEY_F ALLEGRO_KEY_F
    #endif
    #if !defined(WTE_KEY_G)
        #define WTE_KEY_G ALLEGRO_KEY_G
    #endif
    #if !defined(WTE_KEY_H)
        #define WTE_KEY_H ALLEGRO_KEY_H
    #endif
    #if !defined(WTE_KEY_I)
        #define WTE_KEY_I ALLEGRO_KEY_I
    #endif
    #if !defined(WTE_KEY_J)
        #define WTE_KEY_J ALLEGRO_KEY_J
    #endif
    #if !defined(WTE_KEY_K)
        #define WTE_KEY_K ALLEGRO_KEY_K
    #endif
    #if !defined(WTE_KEY_L)
        #define WTE_KEY_L ALLEGRO_KEY_L
    #endif
    #if !defined(WTE_KEY_M)
        #define WTE_KEY_M ALLEGRO_KEY_M
    #endif
    #if !defined(WTE_KEY_N)
        #define WTE_KEY_N ALLEGRO_KEY_N
    #endif
    #if !defined(WTE_KEY_O)
        #define WTE_KEY_O ALLEGRO_KEY_O
    #endif
    #if !defined(WTE_KEY_P)
        #define WTE_KEY_P ALLEGRO_KEY_P
    #endif
    #if !defined(WTE_KEY_Q)
        #define WTE_KEY_Q ALLEGRO_KEY_Q
    #endif
    #if !defined(WTE_KEY_R)
        #define WTE_KEY_R ALLEGRO_KEY_R
    #endif
    #if !defined(WTE_KEY_S)
        #define WTE_KEY_S ALLEGRO_KEY_S
    #endif
    #if !defined(WTE_KEY_T)
        #define WTE_KEY_T ALLEGRO_KEY_T
    #endif
    #if !defined(WTE_KEY_U)
        #define WTE_KEY_U ALLEGRO_KEY_U
    #endif
    #if !defined(WTE_KEY_V)
        #define WTE_KEY_V ALLEGRO_KEY_V
    #endif
    #if !defined(WTE_KEY_W)
        #define WTE_KEY_W ALLEGRO_KEY_W
    #endif
    #if !defined(WTE_KEY_X)
        #define WTE_KEY_X ALLEGRO_KEY_X
    #endif
    #if !defined(WTE_KEY_Y)
        #define WTE_KEY_Y ALLEGRO_KEY_Y
    #endif
    #if !defined(WTE_KEY_Z)
        #define WTE_KEY_Z ALLEGRO_KEY_Z
    #endif
    #if !defined(WTE_KEY_0)
        #define WTE_KEY_0 ALLEGRO_KEY_0
    #endif
    #if !defined(WTE_KEY_1)
        #define WTE_KEY_1 ALLEGRO_KEY_1
    #endif
    #if !defined(WTE_KEY_2)
        #define WTE_KEY_2 ALLEGRO_KEY_2
    #endif
    #if !defined(WTE_KEY_3)
        #define WTE_KEY_3 ALLEGRO_KEY_3
    #endif
    #if !defined(WTE_KEY_4)
        #define WTE_KEY_4 ALLEGRO_KEY_4
    #endif
    #if !defined(WTE_KEY_5)
        #define WTE_KEY_5 ALLEGRO_KEY_5
    #endif
    #if !defined(WTE_KEY_6)
        #define WTE_KEY_6 ALLEGRO_KEY_6
    #endif
    #if !defined(WTE_KEY_7)
        #define WTE_KEY_7 ALLEGRO_KEY_7
    #endif
    #if !defined(WTE_KEY_8)
        #define WTE_KEY_8 ALLEGRO_KEY_8
    #endif
    #if !defined(WTE_KEY_9)
        #define WTE_KEY_9 ALLEGRO_KEY_9
    #endif
    #if !defined(WTE_KEY_PAD_0)
        #define WTE_KEY_PAD_0 ALLEGRO_KEY_PAD_0
    #endif
    #if !defined(WTE_KEY_PAD_1)
        #define WTE_KEY_PAD_1 ALLEGRO_KEY_PAD_1
    #endif
    #if !defined(WTE_KEY_PAD_2)
        #define WTE_KEY_PAD_2 ALLEGRO_KEY_PAD_2
    #endif
    #if !defined(WTE_KEY_PAD_3)
        #define WTE_KEY_PAD_3 ALLEGRO_KEY_PAD_3
    #endif
    #if !defined(WTE_KEY_PAD_4)
        #define WTE_KEY_PAD_4 ALLEGRO_KEY_PAD_4
    #endif
    #if !defined(WTE_KEY_PAD_5)
        #define WTE_KEY_PAD_5 ALLEGRO_KEY_PAD_5
    #endif
    #if !defined(WTE_KEY_PAD_6)
        #define WTE_KEY_PAD_6 ALLEGRO_KEY_PAD_6
    #endif
    #if !defined(WTE_KEY_PAD_7)
        #define WTE_KEY_PAD_7 ALLEGRO_KEY_PAD_7
    #endif
    #if !defined(WTE_KEY_PAD_8)
        #define WTE_KEY_PAD_8 ALLEGRO_KEY_PAD_8
    #endif
    #if !defined(WTE_KEY_PAD_9)
        #define WTE_KEY_PAD_9 ALLEGRO_KEY_PAD_9
    #endif
    #if !defined(WTE_KEY_F1)
        #define WTE_KEY_F1 ALLEGRO_KEY_F1
    #endif
    #if !defined(WTE_KEY_F2)
        #define WTE_KEY_F2 ALLEGRO_KEY_F2
    #endif
    #if !defined(WTE_KEY_F3)
        #define WTE_KEY_F3 ALLEGRO_KEY_F3
    #endif
    #if !defined(WTE_KEY_F4)
        #define WTE_KEY_F4 ALLEGRO_KEY_F4
    #endif
    #if !defined(WTE_KEY_F5)
        #define WTE_KEY_F5 ALLEGRO_KEY_F5 
    #endif
    #if !defined(WTE_KEY_F6)
        #define WTE_KEY_F6 ALLEGRO_KEY_F6
    #endif
    #if !defined(WTE_KEY_F7)
        #define WTE_KEY_F7 ALLEGRO_KEY_F7
    #endif
    #if !defined(WTE_KEY_F8)
        #define WTE_KEY_F8 ALLEGRO_KEY_F8
    #endif
    #if !defined(WTE_KEY_F9)
        #define WTE_KEY_F9 ALLEGRO_KEY_F9
    #endif
    #if !defined(WTE_KEY_F10)
        #define WTE_KEY_F10 ALLEGRO_KEY_F10
    #endif
    #if !defined(WTE_KEY_F11)
        #define WTE_KEY_F11 ALLEGRO_KEY_F11
    #endif
    #if !defined(WTE_KEY_F12)
        #define WTE_KEY_F12 ALLEGRO_KEY_F12
    #endif
    #if !defined(WTE_KEY_ESCAPE)
        #define WTE_KEY_ESCAPE ALLEGRO_KEY_ESCAPE
    #endif
    #if !defined(WTE_KEY_TILDE)
        #define WTE_KEY_TILDE ALLEGRO_KEY_TILDE
    #endif
    #if !defined(WTE_KEY_MINUS)
        #define WTE_KEY_MINUS ALLEGRO_KEY_MINUS
    #endif
    #if !defined(WTE_KEY_EQUALS)
        #define WTE_KEY_EQUALS ALLEGRO_KEY_EQUALS
    #endif
    #if !defined(WTE_KEY_BACKSPACE)
        #define WTE_KEY_BACKSPACE ALLEGRO_KEY_BACKSPACE
    #endif
    #if !defined(WTE_KEY_TAB)
        #define WTE_KEY_TAB ALLEGRO_KEY_TAB
    #endif
    #if !defined(WTE_KEY_OPENBRACE)
        #define WTE_KEY_OPENBRACE ALLEGRO_KEY_OPENBRACE
    #endif
    #if !defined(WTE_KEY_CLOSEBRACE)
        #define WTE_KEY_CLOSEBRACE ALLEGRO_KEY_CLOSEBRACE
    #endif
    #if !defined(WTE_KEY_ENTER)
        #define WTE_KEY_ENTER ALLEGRO_KEY_ENTER
    #endif
    #if !defined(WTE_KEY_SEMICOLON)
        #define WTE_KEY_SEMICOLON ALLEGRO_KEY_SEMICOLON
    #endif
    #if !defined(WTE_KEY_QUOTE)
        #define WTE_KEY_QUOTE ALLEGRO_KEY_QUOTE
    #endif
    #if !defined(WTE_KEY_BACKSLASH)
        #define WTE_KEY_BACKSLASH ALLEGRO_KEY_BACKSLASH
    #endif
    #if !defined(WTE_KEY_BACKSLASH2)
        #define WTE_KEY_BACKSLASH2 ALLEGRO_KEY_BACKSLASH2
    #endif
    #if !defined(WTE_KEY_COMMA)
        #define WTE_KEY_COMMA ALLEGRO_KEY_COMMA
    #endif
    #if !defined(WTE_KEY_FULLSTOP)
        #define WTE_KEY_FULLSTOP ALLEGRO_KEY_FULLSTOP
    #endif
    #if !defined(WTE_KEY_SLASH)
        #define WTE_KEY_SLASH ALLEGRO_KEY_SLASH
    #endif
    #if !defined(WTE_KEY_SPACE)
        #define WTE_KEY_SPACE ALLEGRO_KEY_SPACE
    #endif
    #if !defined(WTE_KEY_INSERT)
        #define WTE_KEY_INSERT ALLEGRO_KEY_INSERT
    #endif
    #if !defined(WTE_KEY_DELETE)
        #define WTE_KEY_DELETE ALLEGRO_KEY_DELETE
    #endif
    #if !defined(WTE_KEY_HOME)
        #define WTE_KEY_HOME ALLEGRO_KEY_HOME
    #endif
    #if !defined(WTE_KEY_END)
        #define WTE_KEY_END ALLEGRO_KEY_END
    #endif
    #if !defined(WTE_KEY_PGUP)
        #define WTE_KEY_PGUP ALLEGRO_KEY_PGUP
    #endif
    #if !defined(WTE_KEY_PGDN)
        #define WTE_KEY_PGDN ALLEGRO_KEY_PGDN
    #endif
    #if !defined(WTE_KEY_LEFT)
        #define WTE_KEY_LEFT ALLEGRO_KEY_LEFT
    #endif
    #if !defined(WTE_KEY_RIGHT)
        #define WTE_KEY_RIGHT ALLEGRO_KEY_RIGHT
    #endif
    #if !defined(WTE_KEY_UP)
        #define WTE_KEY_UP ALLEGRO_KEY_UP
    #endif
    #if !defined(WTE_KEY_DOWN)
        #define WTE_KEY_DOWN ALLEGRO_KEY_DOWN
    #endif
    #if !defined(WTE_KEY_PAD_SLASH)
        #define WTE_KEY_PAD_SLASH ALLEGRO_KEY_PAD_SLASH
    #endif
    #if !defined(WTE_KEY_PAD_ASTERISK)
        #define WTE_KEY_PAD_ASTERISK ALLEGRO_KEY_PAD_ASTERISK
    #endif
    #if !defined(WTE_KEY_PAD_MINUS)
        #define WTE_KEY_PAD_MINUS ALLEGRO_KEY_PAD_MINUS
    #endif
    #if !defined(WTE_KEY_PAD_PLUS)
        #define WTE_KEY_PAD_PLUS ALLEGRO_KEY_PAD_PLUS
    #endif
    #if !defined(WTE_KEY_PAD_DELETE)
        #define WTE_KEY_PAD_DELETE ALLEGRO_KEY_PAD_DELETE
    #endif
    #if !defined(WTE_KEY_PAD_ENTER)
        #define WTE_KEY_PAD_ENTER ALLEGRO_KEY_PAD_ENTER
    #endif
    #if !defined(WTE_KEY_PRINTSCREEN)
        #define WTE_KEY_PRINTSCREEN ALLEGRO_KEY_PRINTSCREEN
    #endif
    #if !defined(WTE_KEY_PAUSE)
        #define WTE_KEY_PAUSE ALLEGRO_KEY_PAUSE
    #endif
    #if !defined(WTE_KEY_ABNT_C1)
        #define WTE_KEY_ABNT_C1 ALLEGRO_KEY_ABNT_C1
    #endif
    #if !defined(WTE_KEY_YEN)
        #define WTE_KEY_YEN ALLEGRO_KEY_YEN
    #endif
    #if !defined(WTE_KEY_KANA)
        #define WTE_KEY_KANA ALLEGRO_KEY_KANA
    #endif
    #if !defined(WTE_KEY_CONVERT)
        #define WTE_KEY_CONVERT ALLEGRO_KEY_CONVERT
    #endif
    #if !defined(WTE_KEY_NOCONVERT)
        #define WTE_KEY_NOCONVERT ALLEGRO_KEY_NOCONVERT
    #endif
    #if !defined(WTE_KEY_AT)
        #define WTE_KEY_AT ALLEGRO_KEY_AT
    #endif
    #if !defined(WTE_KEY_CIRCUMFLEX)
        #define WTE_KEY_CIRCUMFLEX ALLEGRO_KEY_CIRCUMFLEX
    #endif
    #if !defined(WTE_KEY_COLON2)
        #define WTE_KEY_COLON2 ALLEGRO_KEY_COLON2
    #endif
    #if !defined(WTE_KEY_KANJI)
        #define WTE_KEY_KANJI ALLEGRO_KEY_KANJI
    #endif
    #if !defined(WTE_KEY_LSHIFT)
        #define WTE_KEY_LSHIFT ALLEGRO_KEY_LSHIFT
    #endif
    #if !defined(WTE_KEY_RSHIFT)
        #define WTE_KEY_RSHIFT ALLEGRO_KEY_RSHIFT
    #endif
    #if !defined(WTE_KEY_LCTRL)
        #define WTE_KEY_LCTRL ALLEGRO_KEY_LCTRL
    #endif
    #if !defined(WTE_KEY_RCTRL)
        #define WTE_KEY_RCTRL ALLEGRO_KEY_RCTRL
    #endif
    #if !defined(WTE_KEY_ALT)
        #define WTE_KEY_ALT ALLEGRO_KEY_ALT
    #endif
    #if !defined(WTE_KEY_ALTGR)
        #define WTE_KEY_ALTGR ALLEGRO_KEY_ALTGR
    #endif
    #if !defined(WTE_KEY_LWIN)
        #define WTE_KEY_LWIN ALLEGRO_KEY_LWIN
    #endif
    #if !defined(WTE_KEY_RWIN)
        #define WTE_KEY_RWIN ALLEGRO_KEY_RWIN
    #endif
    #if !defined(WTE_KEY_MENU)
        #define WTE_KEY_MENU ALLEGRO_KEY_MENU
    #endif
    #if !defined(WTE_KEY_SCROLLLOCK)
        #define WTE_KEY_SCROLLLOCK ALLEGRO_KEY_SCROLLLOCK
    #endif
    #if !defined(WTE_KEY_NUMLOCK)
        #define WTE_KEY_NUMLOCK ALLEGRO_KEY_NUMLOCK
    #endif
    #if !defined(WTE_KEY_CAPSLOCK)
        #define WTE_KEY_CAPSLOCK ALLEGRO_KEY_CAPSLOCK
    #endif
    #if !defined(WTE_KEY_PAD_EQUALS)
        #define WTE_KEY_PAD_EQUALS ALLEGRO_KEY_PAD_EQUALS
    #endif
    #if !defined(WTE_KEY_BACKQUOTE)
        #define WTE_KEY_BACKQUOTE ALLEGRO_KEY_BACKQUOTE
    #endif
    #if !defined(WTE_KEY_SEMICOLON2)
        #define WTE_KEY_SEMICOLON2 ALLEGRO_KEY_SEMICOLON2
    #endif
    #if !defined(WTE_KEY_COMMAND)
        #define WTE_KEY_COMMAND ALLEGRO_KEY_COMMAND
    #endif
    #if !defined(WTE_KEYMOD_SHIFT)
        #define WTE_KEYMOD_SHIFT ALLEGRO_KEYMOD_SHIFT
    #endif
    #if !defined(WTE_KEYMOD_CTRL)
        #define WTE_KEYMOD_CTRL ALLEGRO_KEYMOD_CTRL
    #endif
    #if !defined(WTE_KEYMOD_ALT)
        #define WTE_KEYMOD_ALT ALLEGRO_KEYMOD_ALT
    #endif
    #if !defined(WTE_KEYMOD_LWIN)
        #define WTE_KEYMOD_LWIN ALLEGRO_KEYMOD_LWIN
    #endif
    #if !defined(WTE_KEYMOD_RWIN)
        #define WTE_KEYMOD_RWIN ALLEGRO_KEYMOD_RWIN
    #endif
    #if !defined(WTE_KEYMOD_MENU)
        #define WTE_KEYMOD_MENU ALLEGRO_KEYMOD_MENU
    #endif
    #if !defined(WTE_KEYMOD_ALTGR)
        #define WTE_KEYMOD_ALTGR ALLEGRO_KEYMOD_ALTGR
    #endif
    #if !defined(WTE_KEYMOD_COMMAND)
        #define WTE_KEYMOD_COMMAND ALLEGRO_KEYMOD_COMMAND
    #endif
    #if !defined(WTE_KEYMOD_SCROLLLOCK)
        #define WTE_KEYMOD_SCROLLLOCK ALLEGRO_KEYMOD_SCROLLLOCK
    #endif
    #if !defined(WTE_KEYMOD_NUMLOCK)
        #define WTE_KEYMOD_NUMLOCK ALLEGRO_KEYMOD_NUMLOCK
    #endif
    #if !defined(WTE_KEYMOD_CAPSLOCK)
        #define WTE_KEYMOD_CAPSLOCK ALLEGRO_KEYMOD_CAPSLOCK
    #endif
    #if !defined(WTE_KEYMOD_INALTSEQ)
        #define WTE_KEYMOD_INALTSEQ ALLEGRO_KEYMOD_INALTSEQ
    #endif
    #if !defined(WTE_KEYMOD_ACCENT1)
        #define WTE_KEYMOD_ACCENT1 ALLEGRO_KEYMOD_ACCENT1
    #endif
    #if !defined(WTE_KEYMOD_ACCENT2)
        #define WTE_KEYMOD_ACCENT2 ALLEGRO_KEYMOD_ACCENT2
    #endif
    #if !defined(WTE_KEYMOD_ACCENT3)
        #define WTE_KEYMOD_ACCENT3 ALLEGRO_KEYMOD_ACCENT3
    #endif
    #if !defined(WTE_KEYMOD_ACCENT4)
        #define WTE_KEYMOD_ACCENT4 ALLEGRO_KEYMOD_ACCENT4
    #endif
#endif  //  #if WTE_USE_KEYBOARD

#endif
