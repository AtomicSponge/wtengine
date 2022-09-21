/*!
 * wtengine | File:  handlers.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_HANDLERS_HPP
#define WTE_HANDLERS_HPP

#include <variant>
#include <type_traits>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"

namespace wte::handler {

//  Define the handler types
using key = std::function<void(const int&, ALLEGRO_DISPLAY*)>;
using mouse_axis = std::function<void(
    const int&, const int&, const int&, const int&, const int&,
    const int&, const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_button = std::function<void(
    const int&, const int&, const int&, const int&,
    const unsigned int&, ALLEGRO_DISPLAY*)>;
using mouse_display = std::function<void(
    const int&, const int&, const int&, const int&, ALLEGRO_DISPLAY*)>;
using joystick_axis = std::function<void(const int&, const int&, const float&, ALLEGRO_JOYSTICK*)>;
using joystick_button = std::function<void(const int&, ALLEGRO_JOYSTICK*)>;
using touch = std::function<void(
    const int&, const float&, const float&, const float&,
    const float&, const bool&, ALLEGRO_DISPLAY*)>;

}

namespace wte {

/*!
 * Handler scopes
 */
enum handler_scopes {
    GLOBAL_HANDLES,     //!<  Global input handles that are always active.
    GAME_HANDLES,       //!<  In-game input handles.
    NONGAME_HANDLES,    //!<  Out-of-game input handles.
};

/*!
 * Handler events.
 * These corospond to the various Allegro events.
 * See https://liballeg.org/a5docs/trunk/events.html for more info.
 */
enum handler_events {
    EVENT_KEY_DOWN,              //!<  Event key down.
    EVENT_KEY_UP,                //!<  Event key up.

    EVENT_MOUSE_AXIS,            //!<  Event mouse axis.
    EVENT_MOUSE_BUTTON_DOWN,     //!<  Event mouse button down.
    EVENT_MOUSE_BUTTON_UP,       //!<  Event mouse button up.
    EVENT_MOUSE_WARPED,          //!<  Event mouse warped.
    EVENT_MOUSE_ENTER_DISPLAY,   //!<  Event mouse enters display.
    EVENT_MOUSE_LEAVE_DISPLAY,   //!<  Event mouse leaves display.

    EVENT_JOYSTICK_AXIS,         //!<  Event joystick axis.
    EVENT_JOYSTICK_BUTTON_DOWN,  //!<  Event joystick button down.
    EVENT_JOYSTICK_BUTTON_UP,    //!<  Event joystick button up.

    EVENT_TOUCH_BEGIN,           //!<  Event touch input begin.
    EVENT_TOUCH_END,             //!<  Event touch input end.
    EVENT_TOUCH_MOVE,            //!<  Event touch input moves.
    EVENT_TOUCH_CANCEL,          //!<  Event touch input canceled.

    HANDLER_EVENT_MAX
};

/*!
 * Handler types.
 * Used to define which function type to choose for the handler.
 */
using handler_types = std::variant<
    handler::key,              //!<  Keyboard event type.  Signature:  const int& key, ALLEGRO_DISPLAY* display
    handler::mouse_axis,       //!<  Mouse axis event type.
    handler::mouse_button,     //!<  Mouse button event type.
    handler::mouse_display,    //!<  Mouse display event type.
    handler::joystick_axis,    //!<  Mouse axis event type.
    handler::joystick_button,  //!<  Joystick button event type.
    handler::touch             //!<  Touch event type.
>;

//  Template structs to store handlers
template <handler_scopes, handler_events, typename T = std::void_t<>>
struct handlers { inline static handler_types _handle; };

template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::key>>> {
    inline static handler_types _handle = [](const int& key, ALLEGRO_DISPLAY* display) {};
};

template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::mouse_axis>>> {
    inline static handler_types _handle = []() {};
};
template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::mouse_button>>> {
    inline static handler_types _handle = []() {};
};
template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::mouse_display>>> {
    inline static handler_types _handle = []() {};
};

template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::joystick_axis>>> {
    inline static handler_types _handle = []() {};
};
template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::joystick_button>>> {
    inline static handler_types _handle = []() {};
};

template <handler_scopes S, handler_events IDX, typename T>
struct handlers<S, IDX, std::enable_if<std::is_same_v<T, handler::touch>>> {
    inline static handler_types _handle = []() {};
};

/*!
 * \brief Used to add an input handle.
 * \tparam S Handler scope.
 * \tparam IDX Event index.
 * \tparam T Handler type.
 * \param handle Input handler function expression.
 */
template <handler_scopes S, handler_events IDX, typename T>
constexpr void add_handler(const handler_types& handle) {
    static_assert(S == GLOBAL_HANDLES || S == NONGAME_HANDLES || S == GAME_HANDLES,
        "Scope must be one of the following: GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES");
    static_assert(IDX < HANDLER_EVENT_MAX, "Invalid Handler Event Index");
    static_assert(std::is_same_v<T, handler::key> ||
        std::is_same_v<T, handler::mouse_axis> ||
        std::is_same_v<T, handler::mouse_button> ||
        std::is_same_v<T, handler::mouse_display> ||
        std::is_same_v<T, handler::joystick_axis> ||
        std::is_same_v<T, handler::joystick_button> ||
        std::is_same_v<T, handler::touch>,
        "Type must be a valid handler");
    if constexpr (std::is_same_v<T, handler::key>)
        static_assert(IDX == EVENT_KEY_DOWN || IDX == EVENT_KEY_UP,
            "Event Index must be a Key Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::mouse_axis>)
        static_assert(IDX == EVENT_MOUSE_AXIS || IDX == EVENT_MOUSE_WARPED,
            "Event Index must be a Mouse Axes or Warped Event");
    else if constexpr (std::is_same_v<T, handler::mouse_button>)
        static_assert(IDX == EVENT_MOUSE_BUTTON_DOWN || IDX == EVENT_MOUSE_BUTTON_UP,
            "Event Index must be a Mouse Button Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::mouse_display>)
        static_assert(IDX == EVENT_MOUSE_ENTER_DISPLAY || IDX == EVENT_MOUSE_LEAVE_DISPLAY,
            "Event Index must be a Mouse Enter or Leave Display Event");
    else if constexpr (std::is_same_v<T, handler::joystick_axis>)
        static_assert(IDX == EVENT_JOYSTICK_AXIS,
            "Event Index must be a Joystick Axes Event");
    else if constexpr (std::is_same_v<T, handler::joystick_button>)
        static_assert(IDX == EVENT_JOYSTICK_BUTTON_DOWN || IDX == EVENT_JOYSTICK_BUTTON_UP,
            "Event Index must be a Joystick Button Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::touch>)
        static_assert(IDX == EVENT_TOUCH_BEGIN || IDX == EVENT_TOUCH_END ||
            IDX == EVENT_TOUCH_MOVE || IDX == EVENT_TOUCH_CANCEL,
            "Event Index must be a Touch Event");
    handlers<S, IDX, T>::_handle = handle;
};

}  //  end namespace wte

#endif
