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

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"

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

}

namespace wte::handler {

template <handler_scopes S, handler_events IDX>
struct key {
    inline static auto _handle = ([](const int&, ALLEGRO_DISPLAY*){});
};

template <handler_scopes S, handler_events IDX>
struct mouse_axis {
    inline static auto _handle = ([](const int&, const int&, const int&, const int&,
        const int&, const int&, const int&, const int&, const float&, ALLEGRO_DISPLAY*){});
};

template <handler_scopes S, handler_events IDX>
struct mouse_button {
    inline static auto _handle = ([](const int&, const int&,
        const int&, const int&, const unsigned int&, ALLEGRO_DISPLAY*){});
};

template <handler_scopes S, handler_events IDX>
struct mouse_display {
    inline static auto _handle = ([](const int&, const int&,
        const int&, const int&, ALLEGRO_DISPLAY*){});
};

template <handler_scopes S, handler_events IDX>
struct joystick_axis {
    inline static auto _handle = ([](const int&, const int&,
        const float&, ALLEGRO_JOYSTICK*){});
};

template <handler_scopes S, handler_events IDX>
struct joystick_button {
    inline static auto _handle = ([](const int&, ALLEGRO_JOYSTICK*){});
};

template <handler_scopes S, handler_events IDX>
struct touch {
    inline static auto _handle = ([](const int&, const float&, const float&,
        const float&, const float&, const bool&, ALLEGRO_DISPLAY*){});
};

}

namespace wte {

/*!
 * \brief Used to add an input handle.
 * \tparam S Handler scope.
 * \tparam IDX Event index.
 * \tparam T Handler type.
 * \param handle Input handler function expression.
 */
template <handler_scopes S, handler_events IDX, typename T, typename F>
constexpr void add_handler(F&& handle) {
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
    if constexpr (std::is_same_v<T, handler::key>) {
        static_assert(IDX == EVENT_KEY_DOWN || IDX == EVENT_KEY_UP,
            "Event Index must be a Key Up or Down Event");
        handler::key<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::mouse_axis>) {
        static_assert(IDX == EVENT_MOUSE_AXIS || IDX == EVENT_MOUSE_WARPED,
            "Event Index must be a Mouse Axes or Warped Event");
        handler::mouse_axis<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::mouse_button>) {
        static_assert(IDX == EVENT_MOUSE_BUTTON_DOWN || IDX == EVENT_MOUSE_BUTTON_UP,
            "Event Index must be a Mouse Button Up or Down Event");
        handler::mouse_button<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::mouse_display>) {
        static_assert(IDX == EVENT_MOUSE_ENTER_DISPLAY || IDX == EVENT_MOUSE_LEAVE_DISPLAY,
            "Event Index must be a Mouse Enter or Leave Display Event");
        handler::mouse_display<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::joystick_axis>) {
        static_assert(IDX == EVENT_JOYSTICK_AXIS,
            "Event Index must be a Joystick Axes Event");
        handler::joystick_axis<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::joystick_button>) {
        static_assert(IDX == EVENT_JOYSTICK_BUTTON_DOWN || IDX == EVENT_JOYSTICK_BUTTON_UP,
            "Event Index must be a Joystick Button Up or Down Event");
        handler::joystick_button<S, IDX>_handler = handle;
    }
    else if constexpr (std::is_same_v<T, handler::touch>) {
        static_assert(IDX == EVENT_TOUCH_BEGIN || IDX == EVENT_TOUCH_END ||
            IDX == EVENT_TOUCH_MOVE || IDX == EVENT_TOUCH_CANCEL,
            "Event Index must be a Touch Event");
        handler::touch<S, IDX>_handler = handle;
    }
};

}  //  end namespace wte

#endif
