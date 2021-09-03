/*!
 * WTEngine | File:  input.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/input.hpp"

namespace wte {

bool input::initialized = false;
const int64_t& input::lastkeypress::timer = input::_lastkeypress::timer;
const int& input::lastkeypress::key = input::_lastkeypress::key;
const int64_t& input::lastbuttonpress::timer = input::_lastbuttonpress::timer;
const int& input::lastbuttonpress::button = input::_lastbuttonpress::button;
std::vector<int> input::input_recorder;

/*
 *
 */
input::input() {
    if(initialized == true) throw std::runtime_error("Input instance already running!");
    initialized = true;
}

/*
 *
 */
void input::toggle_input_recording(void) {
    if(config::flags::record_input) {
        //  Turn recording off
        config::_flags::record_input = false;
        //  Save vector to file
    } else {
        //  Turn recording on
        config::_flags::record_input = true;
        input_recorder.clear();
    }
}

/*
 *
 */
void input::create_input_event_queue(void) {
    input_event_queue = al_create_event_queue();
    if(!input_event_queue) throw std::runtime_error("Failed to create input event queue!");

    al_register_event_source(input_event_queue, al_get_keyboard_event_source());
    if(al_is_joystick_installed()) al_register_event_source(input_event_queue, al_get_joystick_event_source());
}

/*
 *
 */
void input::destroy_input_event_queue(void) { al_destroy_event_queue(input_event_queue); }

/*
 *
 */
void input::check_input_events(void) {
    bool queue_not_empty = true;
    while(queue_not_empty) {
        ALLEGRO_EVENT event;
        queue_not_empty = al_get_next_event(input_event_queue, &event);
        if(queue_not_empty) handle_input_event(event);
    }
}

/*
 *
 */
void input::handle_input_event(const ALLEGRO_EVENT& event) { 
    //  Clear any active alerts or notices on input event
    if(alert::is_set() &&
       (event.type == ALLEGRO_EVENT_KEY_DOWN ||
        event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) alert::clear();
    if(notice::is_set() &&
       (event.type == ALLEGRO_EVENT_KEY_DOWN ||
        event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) notice::clear();

    //  Record input if enabled.
    if(config::flags::record_input) {
        //
    }

    /* ************************************************************* */
    /* *** PROCESS EVENTS WHILE MENU IS OPENED ********************* */
    /* ************************************************************* */
    if(config::flags::menu_opened) {
        switch(event.type) {
        /* *********************** */
        /* *** Keyboard events *** */
        /* *********************** */
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == config::controls::key_menu) {
                mgr::menus::close_menu();
            }
            if(event.keyboard.keycode == config::controls::key_menu_select) {
                mgr::menus::menu_item_select();
            }
            if(event.keyboard.keycode == config::controls::p1_key_up ||
                event.keyboard.keycode == config::controls::p2_key_up) {
                mgr::menus::menu_pos_up();
            }
            if(event.keyboard.keycode == config::controls::p1_key_down ||
                event.keyboard.keycode == config::controls::p2_key_down) {
                mgr::menus::menu_pos_down();
            }
            if(event.keyboard.keycode == config::controls::p1_key_left ||
                event.keyboard.keycode == config::controls::p2_key_left) {
                mgr::menus::menu_pos_start_left();
            }
            if(event.keyboard.keycode == config::controls::p1_key_right ||
                event.keyboard.keycode == config::controls::p2_key_right) {
                mgr::menus::menu_pos_start_right();
            }
            break;
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_KEY_UP:
            if(event.keyboard.keycode == config::controls::p1_key_left ||
                event.keyboard.keycode == config::controls::p2_key_left) {
                mgr::menus::menu_pos_stop_left();
            }
            if(event.keyboard.keycode == config::controls::p1_key_right ||
                event.keyboard.keycode == config::controls::p2_key_right) {
                mgr::menus::menu_pos_stop_right();
            }
            break;

        /* *********************** */
        /* *** Joystick events *** */
        /* *********************** */
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
            if(event.joystick.button == config::controls::key_menu) {
                mgr::menus::close_menu();
            }
            if(event.joystick.button == config::controls::key_menu_select) {
                mgr::menus::menu_item_select();
            }
            if(event.joystick.button == config::controls::p1_button_up ||
                event.joystick.button == config::controls::p2_button_up) {
                mgr::menus::menu_pos_up();
            }
            if(event.joystick.button == config::controls::p1_button_down ||
                event.joystick.button == config::controls::p2_button_down) {
                mgr::menus::menu_pos_down();
            }
            if(event.joystick.button == config::controls::p1_button_left ||
                event.joystick.button == config::controls::p2_button_left) {
                mgr::menus::menu_pos_start_left();
            }
            if(event.joystick.button == config::controls::p1_button_right ||
                event.joystick.button == config::controls::p2_button_right) {
                mgr::menus::menu_pos_start_right();
            }
            break;
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
            if(event.joystick.button == config::controls::p1_button_left ||
                event.joystick.button == config::controls::p2_button_left) {
                mgr::menus::menu_pos_stop_left();
            }
            if(event.joystick.button == config::controls::p1_button_right ||
                event.joystick.button == config::controls::p2_button_right) {
                mgr::menus::menu_pos_stop_right();
            }
            break;
        /////////////////////////////////////////////////////////////
        /*case ALLEGRO_EVENT_JOYSTICK_AXIS:
            switch(event.joystick.axis) {
                case 0:  //  X axis
                    if(event.joystick.pos < -0.6f && last_x_axis[WTE_JOYSTICK_A] > -0.6f) {
                        input_flags::set_button_event(WTE_INPUT_BUTTON_LEFT, WTE_BUTTON_EVENT_ondown);
                        last_x_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                        break;
                    }
                    if(event.joystick.pos > 0.6f && last_x_axis[WTE_JOYSTICK_A] < 0.6f) {
                        input_flags::set_button_event(WTE_INPUT_BUTTON_RIGHT, WTE_BUTTON_EVENT_ondown);
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
                        input_flags::set_button_event(WTE_INPUT_BUTTON_UP, WTE_BUTTON_EVENT_ondown);
                    if(event.joystick.pos > 0.6f && last_y_axis[WTE_JOYSTICK_A] < 0.6f)
                        input_flags::set_button_event(WTE_INPUT_BUTTON_ondown, WTE_BUTTON_EVENT_ondown);
                    last_y_axis[WTE_JOYSTICK_A] = event.joystick.pos;
                    break;
            }
            break;*/
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
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_KEY_DOWN:
            #if WTE_DEBUG_MODE
            if(event.keyboard.keycode == WTE_KEY_TOGGLE_HITBOX) {
                if(config::flags::show_hitboxes) config::_flags::show_hitboxes = false ? true;
            }
            #endif
            if(event.keyboard.keycode == config::controls::key_menu) config::_flags::menu_opened = true;
            if(config::flags::input_enabled) {
                try {
                    /////////////////////////////////////////////////////////////
                    /* *** PLAYER 1 *** */
                    if(event.keyboard.keycode == config::controls::p1_key_up)
                        try { 
                            config::_controls::p1_polc_y = -1.0f;
                            input::event::p1::ondown::up();
                        } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_down)
                        try {
                            config::_controls::p1_polc_y = 1.0f;
                            input::event::p1::ondown::down();
                        } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_left)
                        try {
                            config::_controls::p1_polc_x = -1.0f;
                            input::event::p1::ondown::left();
                        } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_right)
                        try {
                            config::_controls::p1_polc_x = 1.0f;
                            input::event::p1::ondown::right();
                        } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action1)
                        try { input::event::p1::ondown::action1(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action2)
                        try { input::event::p1::ondown::action2(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action3)
                        try { input::event::p1::ondown::action3(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action4)
                        try { input::event::p1::ondown::action4(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action5)
                        try { input::event::p1::ondown::action5(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action6)
                        try { input::event::p1::ondown::action6(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action7)
                        try { input::event::p1::ondown::action7(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action8)
                        try { input::event::p1::ondown::action8(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
                    /////////////////////////////////////////////////////////////
                    /* *** PLAYER 2 *** */
                    if(event.keyboard.keycode == config::controls::p2_key_up)
                        try {
                            config::_controls::p2_polc_y = -1.0f;
                            input::event::p2::ondown::up();
                        } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_down)
                        try {
                            config::_controls::p2_polc_y = 1.0f;
                            input::event::p2::ondown::down();
                        } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_left)
                        try {
                            config::_controls::p2_polc_x = -1.0f;
                            input::event::p2::ondown::left();
                        } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_right)
                        try {
                            config::_controls::p2_polc_x = 1.0f;
                            input::event::p2::ondown::right();
                        } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action1)
                        try { input::event::p2::ondown::action1(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action2)
                        try { input::event::p2::ondown::action2(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action3)
                        try { input::event::p2::ondown::action3(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action4)
                        try { input::event::p2::ondown::action4(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action5)
                        try { input::event::p2::ondown::action5(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action6)
                        try { input::event::p2::ondown::action6(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action7)
                        try { input::event::p2::ondown::action7(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action8)
                        try { input::event::p2::ondown::action8(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
                } catch(wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
            }
            //  Record last key pressed.
            _lastkeypress::timer = engine_time::check_time();
            _lastkeypress::key = event.keyboard.keycode;
            break;  //  End case ALLEGRO_EVENT_KEY_DOWN
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_KEY_UP:
            if(config::flags::input_enabled) {
                try {
                    /////////////////////////////////////////////////////////////
                    /* *** PLAYER 1 *** */
                    if(event.keyboard.keycode == config::controls::p1_key_up)
                        try {
                            if(config::controls::p1_polc_y != 1.0f) config::_controls::p1_polc_y = 0.0f;
                            input::event::p1::onup::up();
                        } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_down)
                        try {
                            if(config::controls::p1_polc_y != -1.0f) config::_controls::p1_polc_y = 0.0f;
                            input::event::p1::onup::down();
                        } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_left)
                        try {
                            if(config::controls::p1_polc_x != 1.0f) config::_controls::p1_polc_x = 0.0f;
                            input::event::p1::onup::left();
                        } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_right)
                        try {
                            if(config::controls::p1_polc_x != -1.0f) config::_controls::p1_polc_x = 0.0f;
                            input::event::p1::onup::right();
                        } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action1)
                        try { input::event::p1::onup::action1(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action2)
                        try { input::event::p1::onup::action2(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action3)
                        try { input::event::p1::onup::action3(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action4)
                        try { input::event::p1::onup::action4(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action5)
                        try { input::event::p1::onup::action5(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action6)
                        try { input::event::p1::onup::action6(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action7)
                        try { input::event::p1::onup::action7(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p1_key_action8)
                        try { input::event::p1::onup::action8(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
                    /////////////////////////////////////////////////////////////
                    /* *** PLAYER 2 *** */
                    if(event.keyboard.keycode == config::controls::p2_key_up)
                        try {
                            if(config::controls::p2_polc_y != 1.0f) config::_controls::p2_polc_y = 0.0f;
                            input::event::p2::onup::up();
                        } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_down)
                        try {
                            if(config::controls::p2_polc_y != -1.0f) config::_controls::p2_polc_y = 0.0f;
                            input::event::p2::onup::down();
                        } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_left)
                        try {
                            if(config::controls::p2_polc_x != 1.0f) config::_controls::p2_polc_x = 0.0f;
                            input::event::p2::onup::left();
                        } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_right)
                        try {
                            if(config::controls::p2_polc_x != -1.0f) config::_controls::p2_polc_x = 0.0f;
                            input::event::p2::onup::right();
                        } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action1)
                        try { input::event::p2::onup::action1(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action2)
                        try { input::event::p2::onup::action2(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action3)
                        try { input::event::p2::onup::action3(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action4)
                        try { input::event::p2::onup::action4(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action5)
                        try { input::event::p2::onup::action5(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action6)
                        try { input::event::p2::onup::action6(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action7)
                        try { input::event::p2::onup::action7(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    if(event.keyboard.keycode == config::controls::p2_key_action8)
                        try { input::event::p2::onup::action8(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
                    
                } catch(wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
            }
            break;  //  End case ALLEGRO_EVENT_KEY_UP

        /* *********************** */
        /* *** Joystick events *** */
        /* *********************** */
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_JOYSTICK_AXIS:
            /* *** PLAYER 1 *** */
            /* *** PLAYER 2 *** */
            break;
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
            /* *** PLAYER 1 *** */
            if(event.joystick.button == config::controls::p1_button_up)
                try { 
                    config::_controls::p1_polc_y = -1.0f;
                    input::event::p1::ondown::up();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_down)
                try {
                    config::_controls::p1_polc_y = 1.0f;
                    input::event::p1::ondown::down();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_left)
                try {
                    config::_controls::p1_polc_x = -1.0f;
                    input::event::p1::ondown::left();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_right)
                try {
                    config::_controls::p1_polc_x = 1.0f;
                    input::event::p1::ondown::right();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action1)
                try { input::event::p1::ondown::action1(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action2)
                try { input::event::p1::ondown::action2(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action3)
                try { input::event::p1::ondown::action3(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action4)
                try { input::event::p1::ondown::action4(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action5)
                try { input::event::p1::ondown::action5(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action6)
                try { input::event::p1::ondown::action6(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action7)
                try { input::event::p1::ondown::action7(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action8)
                try { input::event::p1::ondown::action8(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check_time()); }
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 2 *** */
            if(event.joystick.button == config::controls::p2_button_up)
                try { 
                    config::_controls::p2_polc_y = -1.0f;
                    input::event::p2::ondown::up();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_down)
                try {
                    config::_controls::p2_polc_y = 1.0f;
                    input::event::p2::ondown::down();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_left)
                try {
                    config::_controls::p2_polc_x = -1.0f;
                    input::event::p1::ondown::left();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_right)
                try {
                    config::_controls::p2_polc_x = 1.0f;
                    input::event::p2::ondown::right();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action1)
                try { input::event::p2::ondown::action1(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action2)
                try { input::event::p2::ondown::action2(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action3)
                try { input::event::p2::ondown::action3(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action4)
                try { input::event::p2::ondown::action4(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action5)
                try { input::event::p2::ondown::action5(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action6)
                try { input::event::p2::ondown::action6(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action7)
                try { input::event::p2::ondown::action7(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action8)
                try { input::event::p2::ondown::action8(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check_time()); }
            //  Record last button pressed.
            _lastbuttonpress::timer = engine_time::check_time();
            _lastbuttonpress::button = event.joystick.button;
            break;  //  end ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN
        /////////////////////////////////////////////////////////////
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 1 *** */
            if(event.joystick.button == config::controls::p1_button_up)
                try { 
                    if(config::controls::p1_polc_y != 1.0f) config::_controls::p1_polc_y = 0.0f;
                    input::event::p1::onup::up();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_down)
                try {
                    if(config::controls::p1_polc_y != -1.0f) config::_controls::p1_polc_y = 0.0f;
                    input::event::p1::onup::down();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_left)
                try {
                    if(config::controls::p1_polc_x != 1.0f) config::_controls::p1_polc_x = 0.0f;
                    input::event::p1::onup::left();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_right)
                try {
                    if(config::controls::p1_polc_x != -1.0f) config::_controls::p1_polc_x = 0.0f;
                    input::event::p1::onup::right();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action1)
                try { input::event::p1::onup::action1(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action2)
                try { input::event::p1::onup::action2(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action3)
                try { input::event::p1::onup::action3(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action4)
                try { input::event::p1::onup::action4(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action5)
                try { input::event::p1::onup::action5(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action6)
                try { input::event::p1::onup::action6(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action7)
                try { input::event::p1::onup::action7(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p1_button_action8)
                try { input::event::p1::onup::action8(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check_time()); }
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 2 *** */
            if(event.joystick.button == config::controls::p2_button_up)
                try { 
                    if(config::controls::p2_polc_y != 1.0f) config::_controls::p2_polc_y = 0.0f;
                    input::event::p2::onup::up();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_down)
                try {
                    if(config::controls::p2_polc_y != -1.0f) config::_controls::p2_polc_y = 0.0f;
                    input::event::p2::onup::down();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_left)
                try {
                    if(config::controls::p2_polc_x != 1.0f) config::_controls::p2_polc_x = 0.0f;
                    input::event::p1::onup::left();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_right)
                try {
                    if(config::controls::p2_polc_x != -1.0f) config::_controls::p2_polc_x = 0.0f;
                    input::event::p2::onup::right();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action1)
                try { input::event::p2::onup::action1(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action2)
                try { input::event::p2::onup::action2(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action3)
                try { input::event::p2::onup::action3(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action4)
                try { input::event::p2::onup::action4(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action5)
                try { input::event::p2::onup::action5(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action6)
                try { input::event::p2::onup::action6(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action7)
                try { input::event::p2::onup::action7(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            if(event.joystick.button == config::controls::p2_button_action8)
                try { input::event::p2::onup::action8(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check_time()); }
            break;  //  end ALLEGRO_EVENT_JOYSTICK_BUTTON_UP
        }  //  End switch(event.type)
    } //  End game event processing
}

}  //  end namespace wte
