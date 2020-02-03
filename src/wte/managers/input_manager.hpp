/*
  WTEngine
  By:  Matthew Evans
  File:  input_manager.hpp

  See LICENSE.txt for copyright information

  Handle player input in its own thread
*/

#ifndef WTE_MGR_INPUT_MANAGER_HPP
#define WTE_MGR_INPUT_MANAGER_HPP

#include <iostream>

#include <allegro5/allegro.h>

#include "manager.hpp"
#include "make_thread.hpp"

#include "..\engine_flags.hpp"
#include "..\input_flags.hpp"

namespace wte
{

namespace mgr
{

//!
/*!
*/
class input_manager final : public manager<input_manager>, public make_thread {
    public:
        inline input_manager() {};
        inline ~input_manager() {};

    private:
        void run(void);
};

template <> inline bool input_manager::manager<input_manager>::initialized = false;

//!
/*!
*/
inline void input_manager::run(void) {
    ALLEGRO_TIMER* input_timer;
    ALLEGRO_EVENT_QUEUE* input_queue;
    ALLEGRO_EVENT input_event;

    //  NOTE:  Joystick input is not complete
    //  TODO:  Let user define these in a menu
    struct buttons {
        int fire_1_button = 1; //  B
        int fire_2_button = 0; //  A
        int fire_3_button = 2; //  X
        int select_button = 8;
        int start_button = 9;
    } joy;

    //  Unset all input flags
    for(int i = 0; i < MAX_INPUT_FLAGS; i++) input_flags::unset(i);

    //  Timer for limiting input - 4 ticks / sec
    input_timer = al_create_timer(0.25);

    input_queue = al_create_event_queue();
    al_register_event_source(input_queue, al_get_keyboard_event_source());
    if(al_is_joystick_installed()) al_register_event_source(input_queue, al_get_joystick_event_source());

    while(is_running() == true) {
        //  After 3 ticks, stop and reset input timer
        if(al_get_timer_count(input_timer) == 2) {
            al_stop_timer(input_timer);
            al_set_timer_count(input_timer, 0);
        }

        al_get_next_event(input_queue, &input_event);

        /* *** PROCESS EVENTS WHILE MENU IS OPENED *** */
        if(engine_flags::is_set(GAME_MENU_OPENED)) {
            /* *** Keyboard events *** */
            if(input_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(input_event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        if(!al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_UP);
                            al_start_timer(input_timer);
                        }
                        break;
                    case ALLEGRO_KEY_DOWN:
                        if(!al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_DOWN);
                            al_start_timer(input_timer);
                        }
                        break;
                    case ALLEGRO_KEY_LEFT:
                        if(!al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_LEFT);
                            al_start_timer(input_timer);
                        }
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        if(!al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_RIGHT);
                            al_start_timer(input_timer);
                        }
                        break;
                    case ALLEGRO_KEY_SPACE:
                    case ALLEGRO_KEY_ENTER:
                        if(!al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_MENU_SELECT);
                            al_start_timer(input_timer);
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        //  Can only exit menus if the game is running
                        if(engine_flags::is_set(GAME_STARTED) && !al_get_timer_started(input_timer)) {
                            engine_flags::unset(GAME_MENU_OPENED);
                            for(int i = 0; i < MAX_INPUT_FLAGS; i++) input_flags::unset(i);
                            al_start_timer(input_timer);
                        }
                        break;
                } //  End switch(input_event.keyboard.keycode)
            } //  End if(input_event.type == ALLEGRO_EVENT_KEY_DOWN)

            /* *** Joystick events *** */
            if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
                //std::cout << input_event.joystick.stick << std::endl;
                switch(input_event.joystick.axis) {
                    case 0: //  X axis
                        if((input_event.joystick.pos < 0) && !al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_LEFT);
                            al_start_timer(input_timer);
                        }
                        if((input_event.joystick.pos > 0) && !al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_RIGHT);
                            al_start_timer(input_timer);
                        }
                        break;
                    case 1: //  Y axis
                        if((input_event.joystick.pos < 0) && !al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_UP);
                            al_start_timer(input_timer);
                        }
                        if((input_event.joystick.pos > 0) && !al_get_timer_started(input_timer)) {
                            input_flags::set(INPUT_DOWN);
                            al_start_timer(input_timer);
                        }
                        break;
                } //  End switch(input_event.joystick.axis)
            } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
            if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
                if((input_event.joystick.button == joy.fire_1_button) && !al_get_timer_started(input_timer)) {
                    input_flags::set(INPUT_MENU_SELECT);
                    al_start_timer(input_timer);
                }
                if(input_event.joystick.button == joy.start_button) {
                    if(engine_flags::is_set(GAME_STARTED) && !al_get_timer_started(input_timer)) {
                        engine_flags::unset(GAME_MENU_OPENED);
                        for(int i = 0; i < MAX_INPUT_FLAGS; i++) input_flags::unset(i);
                        al_start_timer(input_timer);
                    }
                }
            } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
        } //  End menu event processing

        /* *** PROCESS EVENTS WHILE GAME IS RUNNING *** */
        else {
            /* *** Keyboard events *** */
            if(input_event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(input_event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        input_flags::set(INPUT_UP);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        input_flags::set(INPUT_DOWN);
                        break;
                    case ALLEGRO_KEY_LEFT:
                        input_flags::set(INPUT_LEFT);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        input_flags::set(INPUT_RIGHT);
                        break;
                    case ALLEGRO_KEY_LCTRL:
                        input_flags::set(INPUT_ACTION_1);
                        break;
                    case ALLEGRO_KEY_ALT:
                        input_flags::set(INPUT_ACTION_2);
                        break;
                    case ALLEGRO_KEY_SPACE:
                        input_flags::set(INPUT_ACTION_3);
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        if(!al_get_timer_started(input_timer)) {
                            engine_flags::set(GAME_MENU_OPENED);
                            for(int i = 0; i < MAX_INPUT_FLAGS; i++) input_flags::unset(i);
                            al_start_timer(input_timer);
                        }
                        break;
                } //  End switch(input_event.keyboard.keycode)
            } //  End if(input_event.type == ALLEGRO_EVENT_KEY_DOWN)
            if(input_event.type == ALLEGRO_EVENT_KEY_UP) {
                switch(input_event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        input_flags::unset(INPUT_UP);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        input_flags::unset(INPUT_DOWN);
                        break;
                    case ALLEGRO_KEY_LEFT:
                        input_flags::unset(INPUT_LEFT);
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        input_flags::unset(INPUT_RIGHT);
                        break;
                    case ALLEGRO_KEY_LCTRL:
                        input_flags::unset(INPUT_ACTION_1);
                        break;
                    case ALLEGRO_KEY_ALT:
                        input_flags::unset(INPUT_ACTION_2);
                        break;
                    case ALLEGRO_KEY_SPACE:
                        input_flags::unset(INPUT_ACTION_3);
                        break;
                } //  End switch(input_event.keyboard.keycode)
            } //  End if(input_event.type == ALLEGRO_EVENT_KEY_UP)

            /* *** Joystick events *** */
            if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
                //std::cout << input_event.joystick.stick << std::endl;
                switch(input_event.joystick.axis) {
                    case 0: //  X axis
                        if(input_event.joystick.pos < 0) input_flags::set(INPUT_LEFT);
                        else input_flags::unset(INPUT_LEFT);
                        if(input_event.joystick.pos > 0) input_flags::set(INPUT_RIGHT);
                        else input_flags::unset(INPUT_RIGHT);
                        break;
                    case 1: //  Y axis
                        if(input_event.joystick.pos < 0) input_flags::set(INPUT_UP);
                        else input_flags::unset(INPUT_UP);
                        if(input_event.joystick.pos > 0) input_flags::set(INPUT_DOWN);
                        else input_flags::unset(INPUT_DOWN);
                        break;
                } //  End switch(input_event.joystick.axis)
            } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
            if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
                if(input_event.joystick.button == joy.fire_1_button) input_flags::set(INPUT_ACTION_1);
                if(input_event.joystick.button == joy.fire_2_button) input_flags::set(INPUT_ACTION_2);
                if(input_event.joystick.button == joy.fire_3_button) input_flags::set(INPUT_ACTION_3);
                if((input_event.joystick.button == joy.start_button) && !al_get_timer_started(input_timer)) {
                    engine_flags::set(GAME_MENU_OPENED);
                    for(int i = 0; i < MAX_INPUT_FLAGS; i++) input_flags::unset(i);
                    al_start_timer(input_timer);
                }
            } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
            if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
                if(input_event.joystick.button == joy.fire_1_button) input_flags::unset(INPUT_ACTION_1);
                if(input_event.joystick.button == joy.fire_2_button) input_flags::unset(INPUT_ACTION_2);
                if(input_event.joystick.button == joy.fire_3_button) input_flags::unset(INPUT_ACTION_3);
            } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
        } //  End game event processing
    } //  End while(is_running == true)

    al_destroy_event_queue(input_queue);
    al_destroy_timer(input_timer);
}

} //  namespace mgr

} //  end namespace wte

#endif
