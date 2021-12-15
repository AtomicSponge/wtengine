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

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/config.hpp"
#include "wtengine/states.hpp"

namespace wte {

/*!
 * \class handlers
 * \brief Input handlers.
 */
class handlers {
    friend class states;

    public:
        virtual ~handlers() = default;             //!<  Default virtual destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add a keyboard input handler.
         * \param keycode 
         * \param handle 
         * \param global 
         * \return True if added, else false.
         */
        inline static const bool add_key_handler(
            const int& keycode,
            const std::function<void(void)>& handle,
            const std::optional<bool>& global
        ) {
            if(global) {
                //
            } else {
                //
            }
            return false;
        };

        /*!
         * \brief Add a button input handler.
         * \param button 
         * \param handle 
         * \param global 
         * \return True if added, else false.
         */
        inline static const bool add_button_handler(
            const int& button,
            const std::function<void(void)>& handle,
            const std::optional<bool>& global
        ) {
            if(global) {
                //
            } else {
                //
            }
            return false;
        };

        /*!
         * \brief Add a joystick input handler.
         * \param joystick 
         * \param handle 
         * \param global 
         * \return True if added, else false.
         */
        inline static const bool add_joystick_handler(
            const int& joystick,
            const std::function<void(void)>& handle,
            const std::optional<bool>& global
        ) {
            if(global) {
                //
            } else {
                //
            }
            return false;
        };

        /*!
         * \brief Add a mouse input handler.
         * \param event 
         * \param handle 
         * \param global 
         * \return True if added, else false.
         */
        inline static const bool add_mouse_handler(
            const unsigned int& event,
            const std::function<void(void)>& handle,
            const std::optional<bool>& global
        ) {
            if(global) {
                //
            } else {
                //
            }
            return false;
        };

        /*!
         * \brief Add a touch input handler.
         * \param event 
         * \param handle 
         * \param global 
         * \return True if added, else false.
         */
        inline static const bool add_touch_handler(
            const unsigned int& event,
            const std::function<void(void)>& handle,
            const std::optional<bool>& global
        ) {
            if(global) {
                //
            } else {
                //
            }
            return false;
        };

    protected:
        handlers();  //!<  Constructor

        //  Process global input events.
        inline static void run_handlers(const ALLEGRO_EVENT&) {};
        //  Process out-of-game input events.
        inline static void run_non_game_handlers(const ALLEGRO_EVENT&) {};

        struct global_handlers {
            // keyboard
            // mouse
            // buttons
            // joysticks
            // touch
        };

        struct non_game_handlers {
            // keyboard
            // mouse
            // buttons
            // joysticks
            // touch
        };

    private:
        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
