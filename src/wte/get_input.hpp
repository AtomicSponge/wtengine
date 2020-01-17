/*
  WTEngine
  By:  Matthew Evans
  File:  get_input.hpp

  See LICENSE.txt for copyright information

  Handle player input in its own thread
*/

#ifndef WTE_GET_INPUT_HPP
#define WTE_GET_INPUT_HPP

#include <iostream>

#include <allegro5/allegro.h>

#include "wte_globals.hpp"

namespace wte
{

//! Get Input thread
/*!
  Run a loop and listen for key presses
*/
inline void *get_input(void *arg) {
    ALLEGRO_TIMER *input_timer;
    ALLEGRO_EVENT_QUEUE *input_queue;
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

    while(game_flag[IS_RUNNING]) {
        //std::cout << game_flag[GAME_STARTED] << std::endl;
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
                    if(game_flag[GAME_MENU_OPENED] && !al_get_timer_started(input_timer)) {
                        key[KEY_UP] = true;
                        al_start_timer(input_timer);
                    } else {
                        key[KEY_UP] = true;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(game_flag[GAME_MENU_OPENED] && !al_get_timer_started(input_timer)) {
                        key[KEY_DOWN] = true;
                        al_start_timer(input_timer);
                    } else {
                        key[KEY_DOWN] = true;
                    }
                    break;
                case ALLEGRO_KEY_LEFT:
                    if(game_flag[GAME_MENU_OPENED] && !al_get_timer_started(input_timer)) {
                        key[KEY_LEFT] = true;
                        al_start_timer(input_timer);
                    } else {
                        key[KEY_LEFT] = true;
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if(game_flag[GAME_MENU_OPENED] && !al_get_timer_started(input_timer)) {
                        key[KEY_RIGHT] = true;
                        al_start_timer(input_timer);
                    } else {
                        key[KEY_RIGHT] = true;
                    }
                    break;
                case ALLEGRO_KEY_LCTRL:
                    key[KEY_FIRE_1] = true;
                    break;
                case ALLEGRO_KEY_ALT:
                    key[KEY_FIRE_2] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_FIRE_3] = true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    //  Open / close menu if game is running
                    if(!al_get_timer_started(input_timer) && game_flag[GAME_STARTED]) {
                        if(game_flag[GAME_MENU_OPENED]) {
                            game_flag[GAME_MENU_OPENED] = false;
                        } else {
                            game_flag[GAME_MENU_OPENED] = true;
                        }
                        al_start_timer(input_timer);
                    }
                    break;
            } //  End switch(input_event.keyboard.keycode)
        } //  End if(input_event.type == ALLEGRO_EVENT_KEY_DOWN)
        if(input_event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(input_event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;
                case ALLEGRO_KEY_LCTRL:
                    key[KEY_FIRE_1] = false;
                    break;
                case ALLEGRO_KEY_ALT:
                    key[KEY_FIRE_2] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_FIRE_3] = false;
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
                    if(input_event.joystick.pos < 0) key[KEY_LEFT] = true;
                    else key[KEY_LEFT] = false;
                    if(input_event.joystick.pos > 0) key[KEY_RIGHT] = true;
                    else key[KEY_RIGHT] = false;
                    break;
                case 1: //  Y axis
                    if(input_event.joystick.pos < 0) key[KEY_UP] = true;
                    else key[KEY_UP] = false;
                    if(input_event.joystick.pos > 0) key[KEY_DOWN] = true;
                    else key[KEY_DOWN] = false;
                    break;
            } //  End switch(input_event.joystick.axis)
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
        if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
            if(input_event.joystick.button == joy.fire_1_button) key[KEY_FIRE_1] = true;
            if(input_event.joystick.button == joy.fire_2_button) key[KEY_FIRE_2] = true;
            if(input_event.joystick.button == joy.fire_3_button) key[KEY_FIRE_3] = true;
            if(input_event.joystick.button == joy.start_button) {
                if(game_flag[GAME_MENU_OPENED] && game_flag[GAME_STARTED]) {
                    game_flag[GAME_MENU_OPENED] = false;
                } else {
                    game_flag[GAME_MENU_OPENED] = true;
                }
            }
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
        if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
            if(input_event.joystick.button == joy.fire_1_button) key[KEY_FIRE_1] = false;
            if(input_event.joystick.button == joy.fire_2_button) key[KEY_FIRE_2] = false;
            if(input_event.joystick.button == joy.fire_3_button) key[KEY_FIRE_3] = false;
        } //  End if(input_event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)

    } //  End while( IS_RUNNING )

    al_destroy_timer(input_timer);
    al_destroy_event_queue(input_queue);

    return NULL;
}

} //  end namespace wte

#endif
