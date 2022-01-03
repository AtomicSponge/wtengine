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
using mouse_axes = std::function<void(
    const int&, const int&, const int&, const int&, const int&,
    const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_button = std::function<void(
    const int&, const int&, const int&, const int&,
    const unsigned int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_warped = std::function<void(
    const int&, const int&, const int&, const int&, const int&,
    const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_enter_display = std::function<void(
    const int&, const int&, const int&, const int&, ALLEGRO_DISPLAY*)>;
using mouse_leave_display = std::function<void(
    const int&, const int&, const int&, const int&, ALLEGRO_DISPLAY*)>;
using joy_axis = std::function<void(const int&, const int&, const float&, ALLEGRO_JOYSTICK*)>;
using joy_button = std::function<void(const int&, ALLEGRO_JOYSTICK*)>;
using touch_begin = std::function<void(
    const int&, const float&, const float&, const float&,
    const float&, const bool&, ALLEGRO_DISPLAY*)>;
using touch_end = std::function<void(
    const int&, const float&, const float&, const float&,
    const float&, const bool&, ALLEGRO_DISPLAY*)>;
using touch_move = std::function<void(
    const int&, const float&, const float&, const float&,
    const float&, const bool&, ALLEGRO_DISPLAY*)>;
using touch_cancel = std::function<void(
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
    WTE_EVENT_TOUCH_BEGIN, WTE_EVENT_TOUCH_END, WTE_EVENT_TOUCH_MOVE, WTE_EVENT_TOUCH_CANCEL
};

using handler_types = std::variant<
    handler::key, handler::mouse_axes, handler::mouse_button,
    handler::mouse_warped, handler::mouse_enter_display, handler::mouse_leave_display,
    handler::joy_axis, handler::joy_button,
    handler::touch_begin, handler::touch_end, handler::touch_move, handler::touch_cancel
>;

/*!
 * \class handlers
 * \brief Input handlers.
 */
template <size_t S>
class handlers {
    friend class input;

    public:
        inline handlers() {
            static_assert(S == GLOBAL_HANDLES || S == NONGAME_HANDLES || S == GAME_HANDLES,
                "Scope must be one of the following: GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES");
        };

        ~handlers() = default;                     //!<  Default destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an input handler.
         * \tparam IDX Handler event index.
         * \param handle Input handler.
         */
        template <size_t IDX>
        inline constexpr void add(const handler_types& handle) {
            static_assert(IDX < 15, "Invalid Handler Event Index");
            _temp_handlers[IDX] = handle;
        };

    private:
        inline static std::array<handler_types, 15> _temp_handlers;
        inline constexpr std::array<handler_types, 15>& builder(std::array<handler_types, 15>& temp) {
            return temp;
        };
        inline static const std::array<handler_types, 15> _handlers = builder(_temp_handlers);

        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif