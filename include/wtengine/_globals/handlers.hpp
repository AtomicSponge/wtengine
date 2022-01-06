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
    const int&, const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_button = std::function<void(
    const int&, const int&, const int&, const int&,
    const unsigned int&, const float&, ALLEGRO_DISPLAY*)>;
using mouse_warped = std::function<void(
    const int&, const int&, const int&, const int&, const int&,
    const int&, const int&, const int&, const float&, ALLEGRO_DISPLAY*)>;
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
    WTE_EVENT_TOUCH_BEGIN, WTE_EVENT_TOUCH_END, WTE_EVENT_TOUCH_MOVE, WTE_EVENT_TOUCH_CANCEL,
    WTE_EVENT_MAX
};

using handler_types = std::variant<
    handler::key, handler::mouse_axes, handler::mouse_button,
    handler::mouse_warped, handler::mouse_enter_display, handler::mouse_leave_display,
    handler::joy_axis, handler::joy_button,
    handler::touch_begin, handler::touch_end, handler::touch_move, handler::touch_cancel
>;

/*!
 * \class handlers
 * \tparam S Handler scope.
 * \brief Input handlers.
 */
template <size_t S>
class handlers {
    friend class input;

    using h_table = std::array<handler_types, WTE_EVENT_MAX>;
    using b_table = std::array<bool, WTE_EVENT_MAX>;

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
        };

    private:
        template <size_t IDX>
        inline constexpr static void check(const handler_types& handle) {
            static_assert(S == GLOBAL_HANDLES || S == NONGAME_HANDLES || S == GAME_HANDLES,
                "Scope must be one of the following: GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES");
            static_assert(IDX < WTE_EVENT_MAX, "Invalid Handler Event Index");
            if(std::holds_alternative<handler::key>(handle))
                static_assert(IDX == WTE_EVENT_KEY_DOWN || WTE_EVENT_KEY_UP,
                "Event Index must be a Key Event");
            _register<IDX>::set = true;
        };

        template <size_t... IDX>
        struct _register { inline static bool set = false; };

        template <size_t... IDX>
        inline constexpr static b_table register_handler(_register<IDX...>) {
            return { _register<IDX...>::set };
        };

        inline constexpr static b_table register_handlers(void) {
            return register_handler(_register<WTE_EVENT_MAX>{});
        };

        inline static h_table _handlers;
        inline constexpr static b_table _href = register_handlers();
};

}  //  end namespace wte

#endif
