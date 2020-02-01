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

    //  TODO: let user define these in a menu
    struct buttons {
        int fire_1_button = 1; //  B
        int fire_2_button = 0; //  A
        int fire_3_button = 2; //  X
        int select_button = 8;
        int start_button = 9;
    } joy;

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

        /*
          Keyboard events
        */
        if(input_event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(input_event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if(engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(input_timer)) {
                        input_flags::set(KEY_UP);
                        al_start_timer(input_timer);
                    } else {
                        input_flags::set(KEY_UP);
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(input_timer)) {
                        input_flags::set(KEY_DOWN);
                        al_start_timer(input_timer);
                    } else {
                        input_flags::set(KEY_DOWN);
                    }
                    break;
                case ALLEGRO_KEY_LEFT:
                    if(engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(input_timer)) {
                        input_flags::set(KEY_LEFT);
                        al_start_timer(input_timer);
                    } else {
                        input_flags::set(KEY_LEFT);
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if(engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(input_timer)) {
                        input_flags::set(KEY_RIGHT);
                        al_start_timer(input_timer);
                    } else {
                        input_flags::set(KEY_RIGHT);
                    }
                    break;
                case ALLEGRO_KEY_LCTRL:
                    input_flags::set(KEY_FIRE_1);
                    break;
                case ALLEGRO_KEY_ALT:
                    input_flags::set(KEY_FIRE_2);
                    break;
                case ALLEGRO_KEY_SPACE:
                    input_flags::set(KEY_FIRE_3);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    //  Open / close menu if game is running
                    if(!al_get_timer_started(input_timer) && engine_flags::is_set(GAME_STARTED)) {
                        if(engine_flags::is_set(GAME_MENU_OPENED)) {
                            engine_flags::unset(GAME_MENU_OPENED);
                        } else {
                            engine_flags::set(GAME_MENU_OPENED);
                        }
                        al_start_timer(input_timer);
                    }
                    break;
            } //  End switch(input_event.keyboard.keycode)
        } //  End if(input_event.type == ALLEGRO_EVENT_KEY_DOWN)
        if(input_event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(input_event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    input_flags::unset(KEY_UP);
                    break;
                case ALLEGRO_KEY_DOWN:
                    input_flags::unset(KEY_DOWN);
                    break;
                case ALLEGRO_KEY_LEFT:
                    input_flags::unset(KEY_LEFT);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    input_flags::unset(KEY_RIGHT);
                    break;
                case ALLEGRO_KEY_LCTRL:
                    input_flags::unset(KEY_FIRE_1);
                    break;
                case ALLEGRO_KEY_ALT:
                    input_flags::unset(KEY_FIRE_2);
                    break;
                case ALLEGRO_KEY_SPACE:
                    input_flags::unset(KEY_FIRE_3);
                    break;
            } //  End switch(input_event.keyboard.keycode)
        } //  End if(input_event.type == ALLEGRO_EVENT_KEY_UP)

        /*
          Joystick events
        */
        if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
            //std::cout << input_event.joystick.stick << std::endl;
            switch(input_event.joystick.axis) {
                case 0: //  X axis
                    if(input_event.joystick.pos < 0) input_flags::set(KEY_LEFT);
                    else input_flags::unset(KEY_LEFT);
                    if(input_event.joystick.pos > 0) input_flags::set(KEY_RIGHT);
                    else input_flags::unset(KEY_RIGHT);
                    break;
                case 1: //  Y axis
                    if(input_event.joystick.pos < 0) input_flags::set(KEY_UP);
                    else input_flags::unset(KEY_UP);
                    if(input_event.joystick.pos > 0) input_flags::set(KEY_DOWN);
                    else input_flags::unset(KEY_DOWN);
                    break;
            } //  End switch(input_event.joystick.axis)
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
        if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
            if(input_event.joystick.button == joy.fire_1_button) input_flags::set(KEY_FIRE_1);
            if(input_event.joystick.button == joy.fire_2_button) input_flags::set(KEY_FIRE_2);
            if(input_event.joystick.button == joy.fire_3_button) input_flags::set(KEY_FIRE_3);
            if(input_event.joystick.button == joy.start_button) {
                if(engine_flags::is_set(GAME_MENU_OPENED) && engine_flags::is_set(GAME_STARTED)) {
                    engine_flags::unset(GAME_MENU_OPENED);
                } else {
                    engine_flags::set(GAME_MENU_OPENED);
                }
            }
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
        if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
            if(input_event.joystick.button == joy.fire_1_button) input_flags::unset(KEY_FIRE_1);
            if(input_event.joystick.button == joy.fire_2_button) input_flags::unset(KEY_FIRE_2);
            if(input_event.joystick.button == joy.fire_3_button) input_flags::unset(KEY_FIRE_3);
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)

    } //  End while( is_running == true )

    al_destroy_event_queue(input_queue);
    al_destroy_timer(input_timer);
}

} //  namespace mgr

} //  end namespace wte

#endif
