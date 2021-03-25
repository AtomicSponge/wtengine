/*!
 * WTEngine | File:  wte_input.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/wte_input.hpp"

namespace wte
{

wte_input::wte_input() {
    for(std::size_t i = 0; i < WTE_MAX_JOYSTICK_FLAGS; i++) {
        x_axis[i] = 0.0f;
        y_axis[i] = 0.0f;
        last_x_axis[i] = 0.0f;
        last_y_axis[i] = 0.0f;
    }

    set_binding(false);
}

void wte_input::create_input_event_queue(void) {
    input_event_queue = al_create_event_queue();
    if(!input_event_queue) throw std::runtime_error("Failed to create input event queue!");

    al_register_event_source(input_event_queue, al_get_keyboard_event_source());
    if(al_is_joystick_installed()) al_register_event_source(input_event_queue, al_get_joystick_event_source());

    set_binding(al_is_joystick_installed());  //  Called from wte_input_map
}

void wte_input::destroy_input_event_queue(void) {
    al_destroy_event_queue(input_event_queue);
}

void wte_input::check_input_events(void) {
    ALLEGRO_EVENT event;
    const bool queue_not_empty = al_get_next_event(input_event_queue, &event);
    if(queue_not_empty) handle_input_event(event);
}

void wte_input::handle_input_event(const ALLEGRO_EVENT& event) { 
    //  Clear any active alerts on input event
    if(alert::is_set() &&
       (event.type == ALLEGRO_EVENT_KEY_DOWN ||
        event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) alert::clear();

    if(event.type == ALLEGRO_EVENT_KEY_DOWN) input_flags::set_last_keypress(event.keyboard.keycode);
    if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) input_flags::set_last_buttonpress(event.joystick.button);

    /* ************************************************************* */
    /* *** PROCESS EVENTS WHILE MENU IS OPENED ********************* */
    /* ************************************************************* */
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        switch(event.type) {
            /* *********************** */
            /* *** Keyboard events *** */
            /* *********************** */
            /////////////////////////////////////////////////////////////
            case ALLEGRO_EVENT_KEY_DOWN:
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
                    case ALLEGRO_KEY_SPACE:
                    case ALLEGRO_KEY_ENTER:
                        input_flags::set_button_event(WTE_INPUT_MENU_SELECT, WTE_BUTTON_EVENT_DOWN);
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        input_flags::set_button_event(WTE_INPUT_MENU_CLOSE, WTE_BUTTON_EVENT_DOWN);
                        break;
                } //  End switch(event.keyboard.keycode)
                break;
            /////////////////////////////////////////////////////////////
            case ALLEGRO_EVENT_KEY_UP:
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_A:
                        input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                    case ALLEGRO_KEY_D:
                        input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
                        break;
                } //  End switch(event.keyboard.keycode)
                break;

            /* *********************** */
            /* *** Joystick events *** */
            /* *********************** */
            /////////////////////////////////////////////////////////////
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_UP])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_DOWN])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_LEFT])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_RIGHT])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_1])
                    input_flags::set_button_event(WTE_INPUT_MENU_SELECT, WTE_BUTTON_EVENT_DOWN);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_2] ||
                   event.joystick.button == button_bind[WTE_INPUT_BUTTON_START])
                    input_flags::set_button_event(WTE_INPUT_MENU_CLOSE, WTE_BUTTON_EVENT_DOWN);
                break;
            /////////////////////////////////////////////////////////////
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_LEFT])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
                if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_RIGHT])
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
                break;
            /////////////////////////////////////////////////////////////
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                switch(event.joystick.axis) {
                    case 0:  //  X axis
                        if(event.joystick.pos < -0.6f && last_x_axis[WTE_JOYSTICK_A] > -0.6f) {
                            input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
                            last_x_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                            break;
                        }
                        if(event.joystick.pos > 0.6f && last_x_axis[WTE_JOYSTICK_A] < 0.6f) {
                            input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
                            last_x_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                            break;
                        }
                        if(event.joystick.pos > -0.6f && last_x_axis[WTE_JOYSTICK_A] < -0.6f) {
                            input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
                            last_x_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                            break;
                        }
                        if(event.joystick.pos < 0.6f && last_x_axis[WTE_JOYSTICK_A] > 0.6f) {
                            input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
                            last_x_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                            break;
                        }
                        break;
                    case 1:  //  Y axis
                        if(event.joystick.pos < -0.6f && last_y_axis[WTE_JOYSTICK_A] > -0.6f)
                            input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
                        if(event.joystick.pos > 0.6f && last_y_axis[WTE_JOYSTICK_A] < 0.6f)
                            input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
                        last_y_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                        break;
                }
                break;
        }  //  End switch(event.type)
    }  //  End menu event processing

    /* ************************************************************* */
    /* *** PROCESS EVENTS WHILE GAME IS RUNNING ******************** */
    /* ************************************************************* */
    else {
        switch(event.type) {
        /* *********************** */
        /* *** Keyboard events *** */
        /* *********************** */
        /*******************************************************************/
        case ALLEGRO_EVENT_KEY_DOWN:
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                y_axis[WTE_JOYSTICK_A] = -1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                y_axis[WTE_JOYSTICK_A] = 1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                x_axis[WTE_JOYSTICK_A] = -1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                x_axis[WTE_JOYSTICK_A] = 1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_4, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                engine_flags::set(GAME_MENU_OPENED);
                input_flags::unset_all();
            }
        break;  //  End case ALLEGRO_EVENT_KEY_DOWN
        /*******************************************************************/
        case ALLEGRO_EVENT_KEY_UP:
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                if(y_axis[WTE_JOYSTICK_A] < 0.0f) {
                    y_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                if(y_axis[WTE_JOYSTICK_A] > 0.0f) {
                    y_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                if(x_axis[WTE_JOYSTICK_A] < 0.0f) {
                    x_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_B])
            {
                #if WTE_INPUT_MODE == 0
                if(x_axis[WTE_JOYSTICK_A] > 0.0f) {
                    x_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                #endif
                input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_A] ||
               event.keyboard.keycode == keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_B])
                input_flags::set_button_event(WTE_INPUT_BUTTON_4, WTE_BUTTON_EVENT_UP);
        break;  //  End case ALLEGRO_EVENT_KEY_UP

        /* *********************** */
        /* *** Joystick events *** */
        /* *********************** */
        /*******************************************************************/
        case ALLEGRO_EVENT_JOYSTICK_AXIS:
            std::size_t THE_STICK;
            if(event.joystick.stick == 0) THE_STICK = WTE_JOYSTICK_A;
            else if(event.joystick.stick == 1) THE_STICK = WTE_JOYSTICK_B;
            else if(event.joystick.stick == 2) {  //  Process left trigger.
                //  Build digital or analogue input, depending on global define.
                #if WTE_INPUT_MODE == 0
                if(event.joystick.pos > 0.5f && last_x_axis[LEFT_TRIGGER] < 0.5f) {
                    //  set button
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT_TRIGGER, WTE_BUTTON_EVENT_DOWN);
                    last_x_axis[LEFT_TRIGGER] = event.joystick.pos;
                }
                if(event.joystick.pos < 0.25f && last_x_axis[LEFT_TRIGGER] > 0.5f) {
                    //  unset button
                    input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT_TRIGGER, WTE_BUTTON_EVENT_UP);
                    last_x_axis[LEFT_TRIGGER] = event.joystick.pos;
                }
                #else
                x_axis[LEFT_TRIGGER] = event.joystick.pos;
                if(x_axis[LEFT_TRIGGER] > 0.0f)
                    input_flags::joystick_toggle(LEFT_TRIGGER, WTE_INPUT_DIRECTION_SET);
                else
                    input_flags::joystick_toggle(LEFT_TRIGGER, WTE_INPUT_DIRECTION_UNSET);
                #endif
                break;
            }
            else if(event.joystick.stick == 3) {  //  Process right trigger.
                //  Build digital or analogue input, depending on global define.
                #if WTE_INPUT_MODE == 0
                if(event.joystick.pos > 0.5f && last_x_axis[RIGHT_TRIGGER] < 0.5f) {
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT_TRIGGER, WTE_BUTTON_EVENT_DOWN);
                    last_x_axis[RIGHT_TRIGGER] = event.joystick.pos;
                }
                if(event.joystick.pos < 0.25f && last_x_axis[RIGHT_TRIGGER] > 0.5f) {
                    input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT_TRIGGER, WTE_BUTTON_EVENT_UP);
                    last_x_axis[RIGHT_TRIGGER] = event.joystick.pos;
                }
                #else
                x_axis[RIGHT_TRIGGER] = event.joystick.pos;
                if(x_axis[RIGHT_TRIGGER] > 0.0f)
                    input_flags::joystick_toggle(RIGHT_TRIGGER, WTE_INPUT_DIRECTION_SET);
                else
                    input_flags::joystick_toggle(RIGHT_TRIGGER, WTE_INPUT_DIRECTION_UNSET);
                #endif
                break;
            }
            else break;  //  Not a known joystick axis, end.
            //  Build digital or analogue input, depending on global define.
            #if WTE_INPUT_MODE == 0
            /* ********* INPUT DIGITAL MODE ********* */
            switch(event.joystick.axis) {
                case 0:  //  X axis
                    if(event.joystick.pos < -0.6f && last_x_axis[THE_STICK] > -0.6f) {
                        x_axis[THE_STICK] = -1.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_x_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos > 0.6f && last_x_axis[THE_STICK] < 0.6f) {
                        x_axis[THE_STICK] = 1.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_x_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos > -0.6f && last_x_axis[THE_STICK] < -0.6f) {
                        x_axis[THE_STICK] = 0.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_x_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos < 0.6f && last_x_axis[THE_STICK] > 0.6f) {
                        x_axis[THE_STICK] = 0.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_x_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    break;
                case 1:  //  Y axis
                    if(event.joystick.pos < -0.6f && last_y_axis[THE_STICK] > -0.6f) {
                        y_axis[THE_STICK] = -1.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[WTE_JOYSTICK_A]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_y_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos > 0.6f && last_y_axis[THE_STICK] < 0.6f) {
                        y_axis[THE_STICK] = 1.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_y_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos > -0.6f && last_y_axis[THE_STICK] < -0.6f) {
                        y_axis[THE_STICK] = 0.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_y_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos < 0.6f && last_y_axis[THE_STICK] > 0.6f) {
                        y_axis[THE_STICK] = 0.0f;
                        input_flags::set_joystick_radians(THE_STICK,
                            std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                        input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                        input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                        if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                        else
                            input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                        last_y_axis[THE_STICK] = event.joystick.pos;
                        break;
                    }
                    break;
            }  //  End switch(event.joystick.axis)
            #else
            /* ********* INPUT ANALOGUE MODE ********* */
            switch(event.joystick.axis) {
                case 0:  //  X axis
                    x_axis[THE_STICK] = event.joystick.pos;
                    input_flags::set_joystick_radians(THE_STICK,
                        std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                    input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                    input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                    if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                    break;
                case 1:  //  Y axis
                    y_axis[THE_STICK] = event.joystick.pos;
                    input_flags::set_joystick_radians(THE_STICK,
                        std::atan2(y_axis[THE_STICK], x_axis[THE_STICK]));
                    input_flags::set_joystick_pol_x(THE_STICK, x_axis[THE_STICK]);
                    input_flags::set_joystick_pol_y(THE_STICK, y_axis[THE_STICK]);
                    if(x_axis[THE_STICK] == 0.0f && y_axis[THE_STICK] == 0.0f)
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(THE_STICK, WTE_INPUT_DIRECTION_SET);
                    break;
            }  //  End switch(event.joystick.axis)
            #endif
        break;  //  End case ALLEGRO_EVENT_JOYSTICK_AXIS

        /*******************************************************************/
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_UP]) {
                y_axis[WTE_JOYSTICK_A] = -1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_DOWN]) {
                y_axis[WTE_JOYSTICK_A] = 1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_LEFT]) {
                x_axis[WTE_JOYSTICK_A] = -1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_RIGHT]) {
                x_axis[WTE_JOYSTICK_A] = 1.0f;
                input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_DOWN);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_1])
                input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_2])
                input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_3])
                input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_4])
                input_flags::set_button_event(WTE_INPUT_BUTTON_4, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_5])
                input_flags::set_button_event(WTE_INPUT_BUTTON_5, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_6])
                input_flags::set_button_event(WTE_INPUT_BUTTON_6, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_7])
                input_flags::set_button_event(WTE_INPUT_BUTTON_7, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_8])
                input_flags::set_button_event(WTE_INPUT_BUTTON_8, WTE_BUTTON_EVENT_DOWN);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_START]) {
                engine_flags::set(GAME_MENU_OPENED);
                input_flags::unset_all();
            }
        break;  //  End case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN

        /*******************************************************************/
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_UP]) {
                if(y_axis[WTE_JOYSTICK_A] < 0.0f) {
                    y_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_DOWN]) {
                if(y_axis[WTE_JOYSTICK_A] > 0.0f) {
                    y_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                input_flags::set_button_event(WTE_INPUT_BUTTON_DOWN, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_LEFT]) {
                if(x_axis[WTE_JOYSTICK_A] < 0.0f) {
                    x_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_RIGHT]) {
                if(x_axis[WTE_JOYSTICK_A] > 0.0f) {
                    x_axis[WTE_JOYSTICK_A] = 0.0f;
                    input_flags::set_joystick_radians(WTE_JOYSTICK_A,
                            std::atan2(y_axis[WTE_JOYSTICK_A], x_axis[WTE_JOYSTICK_A]));
                    input_flags::set_joystick_pol_x(WTE_JOYSTICK_A, x_axis[WTE_JOYSTICK_A]);
                    input_flags::set_joystick_pol_y(WTE_JOYSTICK_A, y_axis[WTE_JOYSTICK_A]);
                    if(x_axis[WTE_JOYSTICK_A] == 0.0f && y_axis[WTE_JOYSTICK_A] == 0.0f)
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET);
                    else
                        input_flags::joystick_toggle(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET);
                }
                input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_UP);
            }
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_1])
                input_flags::set_button_event(WTE_INPUT_BUTTON_1, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_2])
                input_flags::set_button_event(WTE_INPUT_BUTTON_2, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_3])
                input_flags::set_button_event(WTE_INPUT_BUTTON_3, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_4])
                input_flags::set_button_event(WTE_INPUT_BUTTON_4, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_5])
                input_flags::set_button_event(WTE_INPUT_BUTTON_5, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_6])
                input_flags::set_button_event(WTE_INPUT_BUTTON_6, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_7])
                input_flags::set_button_event(WTE_INPUT_BUTTON_7, WTE_BUTTON_EVENT_UP);
            /////////////////////////////////////////////////////////////
            if(event.joystick.button == button_bind[WTE_INPUT_BUTTON_8])
                input_flags::set_button_event(WTE_INPUT_BUTTON_8, WTE_BUTTON_EVENT_UP);
        break;  //  End case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP
        }  //  End switch(event.type)
    } //  End game event processing
}

} //  end namespace wte
