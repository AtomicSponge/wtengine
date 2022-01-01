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
#include <utility>
#include <variant>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/config.hpp"

namespace wte::handler {

using key = std::function<void(int, ALLEGRO_DISPLAY*)>;
using mouse_axes = std::function<void(int, int, int, int, int, int, int, float, ALLEGRO_DISPLAY*)>;
using mouse_button = std::function<void(int, int, int, int, unsigned int, float, ALLEGRO_DISPLAY*)>;
using mouse_warped = std::function<void(int, int, int, int, int, int, int, float, ALLEGRO_DISPLAY*)>;
using mouse_enter_display = std::function<void(int, int, int, int, ALLEGRO_DISPLAY*)>;
using mouse_leave_display = std::function<void(int, int, int, int, ALLEGRO_DISPLAY*)>;
using joy_axis = std::function<void(int, int, float, ALLEGRO_JOYSTICK*)>;
using joy_button = std::function<void(int, ALLEGRO_JOYSTICK*)>;
using touch_begin = std::function<void(int, float, float, float, float, bool, ALLEGRO_DISPLAY*)>;
using touch_end = std::function<void(int, float, float, float, float, bool, ALLEGRO_DISPLAY*)>;
using touch_move = std::function<void(int, float, float, float, float, bool, ALLEGRO_DISPLAY*)>;
using touch_cancel = std::function<void(int, float, float, float, float, bool, ALLEGRO_DISPLAY*)>;

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
class handlers {
    friend class input;

    public:
        virtual ~handlers() = default;             //!<  Default virtual destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an input handler.
         * \tparam S Handler scope.
         * \tparam IDX Handler event index.
         * \param handle Input handler.
         */
        template <size_t S, size_t IDX>
        inline static void add(const handler_types& handle) {
            static_assert(IDX < 15, "Invalid Handler Event Index");
            static_assert(S == GLOBAL_HANDLES || S == NONGAME_HANDLES || S == GAME_HANDLES,
                "Scope must be one of the following: GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES");

            if constexpr (S == GLOBAL_HANDLES) _global_handlers[IDX] = handle;
            if constexpr (S == NONGAME_HANDLES) _game_handlers[IDX] = handle;
            if constexpr (S == GAME_HANDLES) _non_game_handlers[IDX] = handle;
        };

    protected:
        handlers();

    private:
        static void run(const ALLEGRO_EVENT& event);
        
        template <size_t S>
        inline static void run_handlers(const ALLEGRO_EVENT& event) {
            switch(event.type) {
            //  Keyboard events
            case ALLEGRO_EVENT_KEY_DOWN:
                break;
            case ALLEGRO_EVENT_KEY_UP:
                break;

            //  Mouse events
            case ALLEGRO_EVENT_MOUSE_AXES:
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                break;
            case ALLEGRO_EVENT_MOUSE_WARPED:
                break;
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                break;
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                break;

            //  Joystick events
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                break;

            //  Touch events
            case ALLEGRO_EVENT_TOUCH_BEGIN:
                break;
            case ALLEGRO_EVENT_TOUCH_END:
                break;
            case ALLEGRO_EVENT_TOUCH_MOVE:
                break;
            case ALLEGRO_EVENT_TOUCH_CANCEL:
                break;
            }
        };

        static std::array<handler_types, 15> _global_handlers;
        static std::array<handler_types, 15> _game_handlers;
        static std::array<handler_types, 15> _non_game_handlers;

        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
