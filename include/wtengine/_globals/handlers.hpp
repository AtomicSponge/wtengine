/*!
 * WTEngine | File:  handlers.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_HANDLERS_HPP
#define WTE_HANDLERS_HPP

#include <array>
#include <variant>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"

namespace wte::handler {

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

enum handler_scopes { WTE_GLOBAL_HANDLES, WTE_GAME_HANDLES, WTE_NONGAME_HANDLES };
enum handler_events {
    WTE_EVENT_KEY_DOWN, WTE_EVENT_KEY_UP,
    WTE_EVENT_MOUSE_AXES, WTE_EVENT_MOUSE_BUTTON_DOWN, WTE_EVENT_MOUSE_BUTTON_UP,
    WTE_EVENT_MOUSE_WARPED, WTE_EVENT_MOUSE_ENTER_DISPLAY, WTE_EVENT_MOUSE_LEAVE_DISPLAY,
    WTE_EVENT_JOYSTICK_AXIS, WTE_EVENT_JOYSTICK_BUTTON_DOWN, WTE_EVENT_JOYSTICK_BUTTON_UP,
    WTE_EVENT_TOUCH_BEGIN, WTE_EVENT_TOUCH_END, WTE_EVENT_TOUCH_MOVE, WTE_EVENT_TOUCH_CANCEL,
    WTE_EVENT_MAX
};

using handler_types = std::variant<
    handler::key,
    handler::mouse_axis, handler::mouse_button, handler::mouse_display,
    handler::joystick_axis, handler::joystick_button,
    handler::touch
>;

enum handler_registers { WTE_HANDLER_SET, WTE_HANDLER_NOTSET };

template <size_t S>
struct handler_register {
    handler_registers status;
};

template <size_t S>
using reg_table = std::array<handler_register<S>, WTE_EVENT_MAX>;

template <size_t S>
constexpr reg_table<S> builder() {
    reg_table<S> temp = {};
    return temp;
}

template <size_t S, size_t IDX>
constexpr void register_handler(void) {
    //if constexpr (S == WTE_GLOBAL_HANDLES) global_hreg[IDX] = WTE_HANDLER_SET;
    //if constexpr (S == WTE_GAME_HANDLES) game_hreg[IDX] = WTE_HANDLER_SET;
    //if constexpr (S == WTE_NONGAME_HANDLES) nongame_hreg[IDX] = WTE_HANDLER_SET;
};

/*!
 * \class handlers
 * \tparam S Handler scope.
 * \brief Input handlers.
 */
template <size_t S>
class handlers {
    friend class input;

    public:
        handlers() = delete;                       //!<  Delete constructor.
        ~handlers() = delete;                      //!<  Delete destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an input handler.
         * \tparam T Handler type.
         * \param handle Input handler.
         */
        template <typename T, size_t IDX>
        inline constexpr static void add(const T& handle) {
            check<T, IDX>();
            _handlers[IDX] = handle;
            register_handler<S, IDX>();
        };

    private:
        template <typename T, size_t IDX>
        inline constexpr static void check(void) {
            static_assert(S == WTE_GLOBAL_HANDLES || S == WTE_NONGAME_HANDLES || S == WTE_GAME_HANDLES,
                "Scope must be one of the following: WTE_GLOBAL_HANDLES, WTE_NONGAME_HANDLES, WTE_GAME_HANDLES");
            static_assert(IDX < WTE_EVENT_MAX, "Invalid Handler Event Index");
            if constexpr (std::is_same<T, handler::key>::value)
                static_assert(IDX == WTE_EVENT_KEY_DOWN || IDX == WTE_EVENT_KEY_UP,
                    "Event Index must be a Key Up or Down Event");
            else if constexpr (std::is_same<T, handler::mouse_axis>::value)
                static_assert(IDX == WTE_EVENT_MOUSE_AXES || IDX == WTE_EVENT_MOUSE_WARPED,
                    "Event Index must be a Mouse Axes or Warped Event");
            else if constexpr (std::is_same<T, handler::mouse_button>::value)
                static_assert(IDX == WTE_EVENT_MOUSE_BUTTON_DOWN || IDX == WTE_EVENT_MOUSE_BUTTON_UP,
                    "Event Index must be a Mouse Button Up or Down Event");
            else if constexpr (std::is_same<T, handler::mouse_display>::value)
                static_assert(IDX == WTE_EVENT_MOUSE_ENTER_DISPLAY || IDX == WTE_EVENT_MOUSE_LEAVE_DISPLAY,
                    "Event Index must be a Mouse Enter or Leave Display Event");
            else if constexpr (std::is_same<T, handler::joystick_axis>::value)
                static_assert(IDX == WTE_EVENT_JOYSTICK_AXIS,
                    "Event Index must be a Joystick Axes Event");
            else if constexpr (std::is_same<T, handler::joystick_button>::value)
                static_assert(IDX == WTE_EVENT_JOYSTICK_BUTTON_DOWN || IDX == WTE_EVENT_JOYSTICK_BUTTON_UP,
                    "Event Index must be a Joystick Button Up or Down Event");
            else if constexpr (std::is_same<T, handler::touch>::value)
                static_assert(IDX == WTE_EVENT_TOUCH_BEGIN || IDX == WTE_EVENT_TOUCH_END ||
                    IDX == WTE_EVENT_TOUCH_MOVE || IDX == WTE_EVENT_TOUCH_CANCEL,
                    "Event Index must be a Touch Event");
        };

        template <size_t IDX>
        inline constexpr static bool set() {
            return (_registery[IDX].status == WTE_HANDLER_SET ? true : false);
        };

        inline static std::array<handler_types, WTE_EVENT_MAX> _handlers;
        inline constexpr static reg_table<S> _registery = {};
};

constexpr reg_table<WTE_GLOBAL_HANDLES> global_hreg = builder<WTE_GLOBAL_HANDLES>();
constexpr reg_table<WTE_GAME_HANDLES> nongame_hreg = builder<WTE_GAME_HANDLES>();
constexpr reg_table<WTE_NONGAME_HANDLES> game_hreg = builder<WTE_NONGAME_HANDLES>();

}  //  end namespace wte

#endif
