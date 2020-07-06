/*!
 * \brief WTEngine | File:  wte_input.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Handle player input.
 * 
 * \bug WIP
 */

#ifndef WTE_INPUT_HPP
#define WTE_INPUT_HPP

#include <cmath>
#include <stdexcept>

#include <allegro5/allegro.h>

#include "_globals/engine_flags.hpp"
#include "_globals/input_flags.hpp"
#include "_globals/alert.hpp"

namespace wte
{

/*!
 * \class wte_input class
 * Manage the input queue and process events.
 */
class wte_input {
    public:
        /*!
         * WTE Input destructor.
         * \param void
         * \return void
         */
        inline virtual ~wte_input() {};

    protected:
        /*!
         * WTE Input constructor.
         * \param void
         * \return void
         */
        inline wte_input() {
            for(std::size_t i = 0; i < WTE_MAX_JOYSTICK_FLAGS; i++) {
                x_axis[i] = 0.0f;
                y_axis[i] = 0.0f;
            }
        };

        /*!
         * \brief Create the input queue for capturing events.
         * Called during engine initialization.
         * \param void
         * \return void
         */
        inline void create_input_queue(void) {
            input_queue = al_create_event_queue();
            if(!input_queue) throw std::runtime_error("Failed to create input event queue!");

            al_register_event_source(input_queue, al_get_keyboard_event_source());
            if(al_is_joystick_installed()) al_register_event_source(input_queue, al_get_joystick_event_source());
        };

        /*!
         * \brief Destroy the input queue.
         * Called during engine de-initialization.
         * \param void
         * \return void
         */
        inline void destroy_input_queue(void) {
            al_destroy_event_queue(input_queue);
        };

        /*!
         * \brief Check the input queue for events.
         * Called during the main engine loop.  Pass any events to the input handler.
         * \param void
         * \return void
         */
        inline void check_input_events(void) {
            ALLEGRO_EVENT event;
            const bool queue_not_empty = al_get_next_event(input_queue, &event);
            if(queue_not_empty) handle_input_event(event);
        };

    private:
        void handle_input_event(const ALLEGRO_EVENT&);

        ALLEGRO_EVENT_QUEUE* input_queue;

        float x_axis[WTE_MAX_JOYSTICK_FLAGS];
        float y_axis[WTE_MAX_JOYSTICK_FLAGS];
};

/*!
 * Process input events.
 * \param event Event ID.
 * \return void
 */
inline void wte_input::handle_input_event(const ALLEGRO_EVENT& event) { 
    //  Clear any active alerts on input event
    if(alert::is_set() &&
       (event.type == ALLEGRO_EVENT_KEY_DOWN ||
        event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) alert::clear();

    if(event.type == ALLEGRO_EVENT_KEY_DOWN) input_flags::set_last_keypress(event.keyboard.keycode);

    /* ************************************************************* */
    /* *** PROCESS EVENTS WHILE MENU IS OPENED ********************* */
    /* ************************************************************* */
    //  Menu input events are cleared in the menu manager
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        /* *********************** */
        /* *** Keyboard events *** */
        /* *********************** */
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_LCTRL:
                case ALLEGRO_KEY_RCTRL:
                case ALLEGRO_KEY_ALT:
                case ALLEGRO_KEY_ALTGR:
                case ALLEGRO_KEY_LSHIFT:
                case ALLEGRO_KEY_RSHIFT:
                    input_flags::set_button_event(WTE_INPUT_MENU_ALT, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_SPACE:
                case ALLEGRO_KEY_ENTER:
                    input_flags::set_button_event(WTE_INPUT_MENU_SELECT, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    input_flags::set_button_event(WTE_INPUT_MENU_CLOSE, WTE_BUTTON_EVENT_DOWN);
                    break;
            } //  End switch(event.keyboard.keycode)
        } //  End if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_LCTRL:
                case ALLEGRO_KEY_RCTRL:
                case ALLEGRO_KEY_ALT:
                case ALLEGRO_KEY_ALTGR:
                case ALLEGRO_KEY_LSHIFT:
                case ALLEGRO_KEY_RSHIFT:
                    input_flags::set_button_event(WTE_INPUT_MENU_ALT, WTE_BUTTON_EVENT_UP);
                    break;
            } //  End switch(event.keyboard.keycode)
        } //  End if(event.type == ALLEGRO_EVENT_KEY_UP)

        /* *********************** */
        /* *** Joystick events *** */
        /* *********************** */
        /*if(event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
            //std::cout << event.joystick.stick << std::endl;
            switch(event.joystick.axis) {
                case 0: //  X axis
                    if((event.joystick.pos < 0)) input_flags::set(WTE_INPUT_LEFT);
                    if((event.joystick.pos > 0)) input_flags::set(WTE_INPUT_RIGHT);
                    break;
                case 1: //  Y axis
                    if((event.joystick.pos < 0)) input_flags::set(WTE_INPUT_UP);
                    if((event.joystick.pos > 0)) input_flags::set(WTE_INPUT_DOWN);
                    break;
            } //  End switch(event.joystick.axis)
        } //  End if(event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
        if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
            if((event.joystick.button == joy.fire_1_button)) {
                input_flags::set(WTE_INPUT_MENU_SELECT);
            }
            if(event.joystick.button == joy.start_button) {
                if(engine_flags::is_set(GAME_STARTED)) {
                    engine_flags::unset(GAME_MENU_OPENED);
                    input_flags::unset_all();
                }
            }
        } //  End if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)*/
    } //  End menu event processing

    /* ************************************************************* */
    /* *** PROCESS EVENTS WHILE GAME IS RUNNING ******************** */
    /* ************************************************************* */
    else {
        /* *********************** */
        /* *** Keyboard events *** */
        /* *********************** */
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    y_axis[WTE_JOYSTICK_A] = -1.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    y_axis[WTE_JOYSTICK_A] = 1.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    x_axis[WTE_JOYSTICK_A] = -1.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    x_axis[WTE_JOYSTICK_A] = 1.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LCTRL:
                case ALLEGRO_KEY_RCTRL:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_ALT:
                case ALLEGRO_KEY_ALTGR:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_DOWN);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LSHIFT:
                case ALLEGRO_KEY_RSHIFT:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_DOWN);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    engine_flags::set(GAME_MENU_OPENED);
                    input_flags::unset_all();
                    break;
            } //  End switch(event.keyboard.keycode)
        } //  End if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    if(y_axis[WTE_JOYSTICK_A] < 0.0f) {
                        y_axis[WTE_JOYSTICK_A] = 0.0f;
                        input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                        input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                        input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                        if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    }
                    input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    if(y_axis[WTE_JOYSTICK_A] > 0.0f) {
                        y_axis[WTE_JOYSTICK_A] = 0.0f;
                        input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                        input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                        input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                        if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    }
                    input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    if(x_axis[WTE_JOYSTICK_A] < 0.0f) {
                        x_axis[WTE_JOYSTICK_A] = 0.0f;
                        input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                        input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                        input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                        if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    }
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    if(x_axis[WTE_JOYSTICK_A] > 0.0f) {
                        x_axis[WTE_JOYSTICK_A] = 0.0f;
                        input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                                std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                        input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                        input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                        if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTON_SET);
                    }
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LCTRL:
                case ALLEGRO_KEY_RCTRL:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_ALT:
                case ALLEGRO_KEY_ALTGR:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_UP);
                    break;
                /////////////////////////////////////////////////////////////
                case ALLEGRO_KEY_LSHIFT:
                case ALLEGRO_KEY_RSHIFT:
                    input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_UP);
                    break;
            } //  End switch(event.keyboard.keycode)
        } //  End if(event.type == ALLEGRO_EVENT_KEY_UP)

        /* *********************** */
        /* *** Joystick events *** */
        /* *********************** */
        /*if(event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
            //std::cout << event.joystick.stick << std::endl;
            switch(event.joystick.axis) {
                case 0: //  X axis
                    if(event.joystick.pos < 0) input_flags::set(WTE_INPUT_LEFT);
                    else input_flags::unset(WTE_INPUT_LEFT);
                    if(event.joystick.pos > 0) input_flags::set(WTE_INPUT_RIGHT);
                    else input_flags::unset(WTE_INPUT_RIGHT);
                    break;
                case 1: //  Y axis
                    if(event.joystick.pos < 0) input_flags::set(WTE_INPUT_UP);
                    else input_flags::unset(WTE_INPUT_UP);
                    if(event.joystick.pos > 0) input_flags::set(WTE_INPUT_DOWN);
                    else input_flags::unset(WTE_INPUT_DOWN);
                    break;
            } //  End switch(event.joystick.axis)
        } //  End if(event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
        if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
            if(event.joystick.button == joy.fire_1_button) input_flags::set(WTE_INPUT_ACTION_1);
            if(event.joystick.button == joy.fire_2_button) input_flags::set(WTE_INPUT_ACTION_2);
            if(event.joystick.button == joy.fire_3_button) input_flags::set(WTE_INPUT_ACTION_3);
            if(event.joystick.button == joy.start_button) {
                engine_flags::set(GAME_MENU_OPENED);
                input_flags::unset_all();
            }
        } //  End if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
        if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
            if(event.joystick.button == joy.fire_1_button) input_flags::unset(WTE_INPUT_ACTION_1);
            if(event.joystick.button == joy.fire_2_button) input_flags::unset(WTE_INPUT_ACTION_2);
            if(event.joystick.button == joy.fire_3_button) input_flags::unset(WTE_INPUT_ACTION_3);
        } //  End if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)*/
    } //  End game event processing
}

} //  end namespace wte

#endif
