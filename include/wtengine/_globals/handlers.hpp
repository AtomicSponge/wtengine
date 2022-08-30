/*!
 * wtengine | File:  handlers.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
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

/*
 *
 */
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
    WTE_GLOBAL_HANDLES,     //!<  Global input handles that are always active.
    WTE_GAME_HANDLES,       //!<  In-game input handles.
    WTE_NONGAME_HANDLES,    //!<  Out-of-game input handles.
};

/*!
 * Handler events.
 * These corospond to the various Allegro events.
 * See https://liballeg.org/a5docs/trunk/events.html for more info.
 */
enum handler_events {
    WTE_EVENT_KEY_DOWN,              //!<  Event key down.
    WTE_EVENT_KEY_UP,                //!<  Event key up.

    WTE_EVENT_MOUSE_AXIS,            //!<  Event mouse axis.
    WTE_EVENT_MOUSE_BUTTON_DOWN,     //!<  Event mouse button down.
    WTE_EVENT_MOUSE_BUTTON_UP,       //!<  Event mouse button up.
    WTE_EVENT_MOUSE_WARPED,          //!<  Event mouse warped.
    WTE_EVENT_MOUSE_ENTER_DISPLAY,   //!<  Event mouse enters display.
    WTE_EVENT_MOUSE_LEAVE_DISPLAY,   //!<  Event mouse leaves display.

    WTE_EVENT_JOYSTICK_AXIS,         //!<  Event joystick axis.
    WTE_EVENT_JOYSTICK_BUTTON_DOWN,  //!<  Event joystick button down.
    WTE_EVENT_JOYSTICK_BUTTON_UP,    //!<  Event joystick button up.

    WTE_EVENT_TOUCH_BEGIN,           //!<  Event touch input begin.
    WTE_EVENT_TOUCH_END,             //!<  Event touch input end.
    WTE_EVENT_TOUCH_MOVE,            //!<  Event touch input moves.
    WTE_EVENT_TOUCH_CANCEL,          //!<  Event touch input canceled.

    WTE_HANDLER_EVENT_MAX
};

//  Variant for storing the different handles
using handler_types = std::variant<
    handler::key,
    handler::mouse_axis, handler::mouse_button, handler::mouse_display,
    handler::joystick_axis, handler::joystick_button,
    handler::touch
>;

/*
 * Handlers template class
 * Stores an input handler.
 */
//  Handler is not set
template <handler_scopes S, handler_events IDX, typename Enabled = std::false_type>
class handlers {
    public:
        constexpr inline static bool is_set = false;
};

//  Handler is set - wip
template <handler_scopes S, handler_events IDX>
class handlers<S, IDX, typename std::enable_if_t<std::is_member_function_pointer_v<decltype(&handlers<S, IDX>::add)>>> {
    friend class input;
    friend constexpr void add_handler(const handler_types& handle);

    public:
        handlers() = delete;                       //  Delete constructor.
        ~handlers() = delete;                      //  Delete destructor.
        handlers(const handlers&) = delete;        //  Delete copy constructor.
        void operator=(handlers const&) = delete;  //  Delete assignment operator.

        //  Create a new handler
        //inline static void add(const handler_types& handle) { _handle = handle; };

        constexpr inline static bool is_set = true;

    private:
        inline static handler_types _handle;  //  Store handler
};

/*!
 * \brief Used to add an input handle.
 * \tparam S Handler scope.
 * \tparam IDX Event index.
 * \tparam T Handler type.
 * \param handle Input handler function expression.
 */
template <handler_scopes S, handler_events IDX, class T>
constexpr void add_handler(const handler_types& handle) {
    static_assert(S == WTE_GLOBAL_HANDLES || S == WTE_NONGAME_HANDLES || S == WTE_GAME_HANDLES,
        "Scope must be one of the following: WTE_GLOBAL_HANDLES, WTE_NONGAME_HANDLES, WTE_GAME_HANDLES");
    static_assert(IDX < WTE_HANDLER_EVENT_MAX, "Invalid Handler Event Index");
    static_assert(std::is_same_v<T, handler::key> ||
        std::is_same_v<T, handler::mouse_axis> ||
        std::is_same_v<T, handler::mouse_button> ||
        std::is_same_v<T, handler::mouse_display> ||
        std::is_same_v<T, handler::joystick_axis> ||
        std::is_same_v<T, handler::joystick_button> ||
        std::is_same_v<T, handler::touch>,
        "Type must be a valid handler");
    if constexpr (std::is_same_v<T, handler::key>)
        static_assert(IDX == WTE_EVENT_KEY_DOWN || IDX == WTE_EVENT_KEY_UP,
            "Event Index must be a Key Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::mouse_axis>)
        static_assert(IDX == WTE_EVENT_MOUSE_AXIS || IDX == WTE_EVENT_MOUSE_WARPED,
            "Event Index must be a Mouse Axes or Warped Event");
    else if constexpr (std::is_same_v<T, handler::mouse_button>)
        static_assert(IDX == WTE_EVENT_MOUSE_BUTTON_DOWN || IDX == WTE_EVENT_MOUSE_BUTTON_UP,
            "Event Index must be a Mouse Button Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::mouse_display>)
        static_assert(IDX == WTE_EVENT_MOUSE_ENTER_DISPLAY || IDX == WTE_EVENT_MOUSE_LEAVE_DISPLAY,
            "Event Index must be a Mouse Enter or Leave Display Event");
    else if constexpr (std::is_same_v<T, handler::joystick_axis>)
        static_assert(IDX == WTE_EVENT_JOYSTICK_AXIS,
            "Event Index must be a Joystick Axes Event");
    else if constexpr (std::is_same_v<T, handler::joystick_button>)
        static_assert(IDX == WTE_EVENT_JOYSTICK_BUTTON_DOWN || IDX == WTE_EVENT_JOYSTICK_BUTTON_UP,
            "Event Index must be a Joystick Button Up or Down Event");
    else if constexpr (std::is_same_v<T, handler::touch>)
        static_assert(IDX == WTE_EVENT_TOUCH_BEGIN || IDX == WTE_EVENT_TOUCH_END ||
            IDX == WTE_EVENT_TOUCH_MOVE || IDX == WTE_EVENT_TOUCH_CANCEL,
            "Event Index must be a Touch Event");
    //handlers<S, IDX>::add(handle);
    handlers<S, IDX>::_handle = handle;
};

}  //  end namespace wte

#endif
