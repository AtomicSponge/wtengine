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
#include <algorithm>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"

namespace wte::handler {

using key = std::function<void(const int&, ALLEGRO_DISPLAY*)>;
using mouse_axes = std::function<void(
    const int&, const int&, const int&, const int&, const int&,
    const int&, const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_button = std::function<void(
    const int&, const int&, const int&, const int&,
    const unsigned int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_display = std::function<void(
    const int&, const int&, const int&, const int&, ALLEGRO_DISPLAY*)>;
using joy_axis = std::function<void(const int&, const int&, const float&, ALLEGRO_JOYSTICK*)>;
using joy_button = std::function<void(const int&, ALLEGRO_JOYSTICK*)>;
using touch = std::function<void(
    const int&, const float&, const float&, const float&,
    const float&, const bool&, ALLEGRO_DISPLAY*)>;

}

namespace wte {

enum handler_scopes { GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES };
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
    handler::mouse_axes, handler::mouse_button, handler::mouse_display,
    handler::joy_axis, handler::joy_button,
    handler::touch
>;

using b_table = std::array<bool, WTE_EVENT_MAX>;
constexpr b_table register_handler(const b_table t) { return t; };

b_table _global_hreg = { false };
b_table _game_hreg = { false };
b_table _nongame_hreg = { false };

/*!
 * \class handlers
 * \tparam S Handler scope.
 * \brief Input handlers.
 */
template <size_t S>
class handlers {
    friend class input;

    using h_table = std::array<handler_types, WTE_EVENT_MAX>;

    public:
        handlers() = delete;                       //!<  Delete constructor.
        ~handlers() = delete;                      //!<  Delete destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an input handler.
         * \tparam IDX Handler event index.
         * \param handle Input handler.
         */
        template <size_t IDX>
        inline constexpr static void add(const handler_types& handle) {
            check<IDX>(handle);
            _handlers[IDX] = handle;
            if constexpr (S == GLOBAL_HANDLES) _global_hreg[IDX] = true;
            if constexpr (S == NONGAME_HANDLES) _game_hreg[IDX] = true;
            if constexpr (S == GAME_HANDLES) _nongame_hreg[IDX] = true;
        };

    private:
        template <size_t IDX>
        inline constexpr static void check(const handler_types& handle) {
            static_assert(S == GLOBAL_HANDLES || S == NONGAME_HANDLES || S == GAME_HANDLES,
                "Scope must be one of the following: GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES");
            static_assert(IDX < WTE_EVENT_MAX, "Invalid Handler Event Index");
            if(std::holds_alternative<handler::key>(handle))
                static_assert(IDX == WTE_EVENT_KEY_DOWN || IDX == WTE_EVENT_KEY_UP,
                    "Event Index must be a Key Up or Down Event");
            /*else if(std::holds_alternative<handler::mouse_axes>(handle))
                static_assert(IDX == WTE_EVENT_MOUSE_AXES || IDX == WTE_EVENT_MOUSE_WARPED,
                    "Event Index must be a Mouse Axes or Warped Event");
            else if(std::holds_alternative<handler::mouse_button>(handle))
                static_assert(IDX == WTE_EVENT_MOUSE_BUTTON_DOWN || IDX == WTE_EVENT_MOUSE_BUTTON_UP,
                    "Event Index must be a Mouse Button Up or Down Event");
            else if(std::holds_alternative<handler::mouse_display>(handle))
                static_assert(IDX == WTE_EVENT_MOUSE_ENTER_DISPLAY || IDX == WTE_EVENT_MOUSE_LEAVE_DISPLAY,
                    "Event Index must be a Mouse Enter or Leave Display Event");
            else if(std::holds_alternative<handler::joy_axis>(handle))
                static_assert(IDX == WTE_EVENT_JOYSTICK_AXIS,
                    "Event Index must be a Joystick Axes Event");
            else if(std::holds_alternative<handler::joy_button>(handle))
                static_assert(IDX == WTE_EVENT_JOYSTICK_BUTTON_DOWN || IDX == WTE_EVENT_JOYSTICK_BUTTON_UP,
                    "Event Index must be a Joystick Button Up or Down Event");
            else if(std::holds_alternative<handler::touch>(handle))
                static_assert(IDX == WTE_EVENT_TOUCH_BEGIN || IDX == WTE_EVENT_TOUCH_END ||
                    IDX == WTE_EVENT_TOUCH_MOVE || IDX == WTE_EVENT_TOUCH_CANCEL,
                    "Event Index must be a Touch Event");*/
        };

        inline static h_table _handlers;
};

constexpr b_table global_hreg = register_handler(_global_hreg);
constexpr b_table game_hreg = register_handler(_game_hreg);
constexpr b_table nongame_hreg = register_handler(_nongame_hreg);

}  //  end namespace wte

#endif
