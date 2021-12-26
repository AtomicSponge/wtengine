/*!
 * WTEngine | File:  input.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/input.hpp"

namespace wte {

const ALLEGRO_KEYBOARD_STATE& input::states::keyboard = input::_states::keyboard;
const ALLEGRO_MOUSE_STATE& input::states::mouse = input::_states::mouse;
const ALLEGRO_TOUCH_INPUT_STATE& input::states::touches = input::_states::touches;
const ALLEGRO_TOUCH_STATE& input::states::first_touch = input::_states::first_touch;
const ALLEGRO_TOUCH_STATE& input::states::last_touch = input::_states::last_touch;

bool input::initialized = false;
const int64_t& input::lastkeypress::timer = input::_lastkeypress::timer;
const int& input::lastkeypress::key = input::_lastkeypress::key;
const int64_t& input::lastbuttonpress::timer = input::_lastbuttonpress::timer;
const int& input::lastbuttonpress::button = input::_lastbuttonpress::button;
int64_t input::last_tick = 0;
std::vector<
    std::pair<const int64_t, const std::vector<ALLEGRO_EVENT>>
> input::input_recorder;
std::vector<ALLEGRO_EVENT> input::event_recorder;
ALLEGRO_EVENT_QUEUE* input::input_event_queue;

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
void input::create_event_queue(void) {
    input_event_queue = al_create_event_queue();
    if(!input_event_queue) throw std::runtime_error("Failed to create input event queue!");

    if(config::flags::keyboard_detected)
        al_register_event_source(input_event_queue, al_get_keyboard_event_source());
    if(config::flags::mouse_detected)
        al_register_event_source(input_event_queue, al_get_mouse_event_source());
    if(config::flags::joystick_detected)
        al_register_event_source(input_event_queue, al_get_joystick_event_source());
    if(config::flags::touch_detected)
        al_register_event_source(input_event_queue, al_get_touch_input_event_source());
}

/*
 *
 */
void input::destroy_event_queue(void) { al_destroy_event_queue(input_event_queue); }

/*
 *
 */
void input::reset_p1_pols(void) {
    config::_controls::p1_pola_x = 0.0f;
    config::_controls::p1_pola_y = 0.0f;
    config::_controls::p1_polb_x = 0.0f;
    config::_controls::p1_polb_y = 0.0f;
    config::_controls::p1_polc_x = 0.0f;
    config::_controls::p1_polc_y = 0.0f;
    config::_controls::p1_throttle_left = 0.0f;
    config::_controls::p1_throttle_right = 0.0f;
}

/*
 *
 */
void input::reset_p2_pols(void) {
    config::_controls::p2_pola_x = 0.0f;
    config::_controls::p2_pola_y = 0.0f;
    config::_controls::p2_polb_x = 0.0f;
    config::_controls::p2_polb_y = 0.0f;
    config::_controls::p2_polc_x = 0.0f;
    config::_controls::p2_polc_y = 0.0f;
    config::_controls::p2_throttle_left = 0.0f;
    config::_controls::p2_throttle_right = 0.0f;
}

/*
 *
 */
void input::reset_pols(void) {
    reset_p1_pols();
    reset_p2_pols();
}

/*
 *
 */
void input::toggle_recording(void) {
    if(config::flags::record_input) {
        //  Turn recording off
        config::_flags::record_input = false;
        if(!input_recorder.empty()) save_recorder();
    } else {
        //  Turn recording on
        event_recorder.clear();
        input_recorder.clear();
        config::_flags::record_input = true;
    }
}

/*
 *
 */
void input::record_event(const ALLEGRO_EVENT& event) { 
    if(engine_time::check() > last_tick && !event_recorder.empty())  {
        input_recorder.push_back(std::make_pair(last_tick, event_recorder));
        event_recorder.clear();
    }
    event_recorder.push_back(event);
    last_tick = engine_time::check();
}

/*
 *
 */
const bool input::save_recorder(void) {
    std::ostringstream oss;
    {const std::time_t t = std::time(nullptr);
    const std::tm tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%F_%H%M%S");}
    const std::string fname = oss.str() + ".inputrec";
    std::ofstream dfile(fname, std::ios::binary | std::ofstream::trunc);
    if(!dfile.good()) return false;

    try {
        for(auto& it: input_recorder) {
            dfile.write(reinterpret_cast<const char*>(&it.first), sizeof(int64_t));
            const std::size_t num_events = it.second.size();
            dfile.write(reinterpret_cast<const char*>(&num_events), sizeof(std::size_t));
            for(auto& e_it: it.second) {
                dfile.write(reinterpret_cast<const char*>(sizeof(e_it)), sizeof(int32_t));
                dfile.write(reinterpret_cast<const char*>(&e_it), sizeof(e_it));
            }
        }
    } catch(...) {
        dfile.close();
        return false;
    }
    dfile.close();
    return true;
}

/*
 *
 */
const bool input::check_events(void) {
    ALLEGRO_EVENT event;
    while(al_get_next_event(input_event_queue, &event)) {
        capture_states(event);
        //  Clear any active alerts on input event
        if(alert::is_set() &&
            (event.type == ALLEGRO_EVENT_KEY_DOWN ||
             event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)) alert::clear();
        //  Toggle menus
        if(event.type == ALLEGRO_EVENT_KEY_DOWN &&
           event.keyboard.keycode == config::controls::key_menu)
            (config::flags::menu_opened ?
                config::_flags::menu_opened = false :
                config::_flags::menu_opened = true);
        //  Record input if enabled.
        if(config::flags::record_input) record_event(event);
        handlers::run(event);
        run_game_handlers(event);  //  todo: remove
    }
    return true;
}

/*
 *
 */
void input::capture_states(const ALLEGRO_EVENT& event) {
    if(config::flags::keyboard_detected)
        al_get_keyboard_state(&input::_states::keyboard);
    if(config::flags::mouse_detected)
        al_get_mouse_state(&input::_states::mouse);
    //if(config::flags::joystick_detected)
        //al_get_joystick_state(&input::_states::joystick);
    if(config::flags::touch_detected)
        al_get_touch_input_state(&input::_states::touches);
}

/*
 *
 */
void input::run_game_handlers(const ALLEGRO_EVENT& event) {
    switch(event.type) {
    /* *********************** */
    /* *** Mouse events ****** */
    /* *********************** */
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

    /* *********************** */
    /* *** Touch events ****** */
    /* *********************** */
    case ALLEGRO_EVENT_TOUCH_BEGIN:
        break;
    case ALLEGRO_EVENT_TOUCH_END:
        break;
    case ALLEGRO_EVENT_TOUCH_MOVE:
        break;
    case ALLEGRO_EVENT_TOUCH_CANCEL:
        break;

    /* *********************** */
    /* *** Keyboard events *** */
    /* *********************** */
    /////////////////////////////////////////////////////////////
    case ALLEGRO_EVENT_KEY_DOWN:
        //  Record last key pressed.
        _lastkeypress::timer = engine_time::check();
        _lastkeypress::key = event.keyboard.keycode;
        if(build_options.debug_mode && event.keyboard.keycode == WTE_KEY_TOGGLE_HITBOX) {
            (config::flags::show_hitboxes ?
                config::_flags::show_hitboxes = false :
                config::_flags::show_hitboxes = true);
        }
        if(config::flags::input_enabled) { try {
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 1 *** */
            if(event.keyboard.keycode == config::controls::p1_key_up)
                try { 
                    config::_controls::p1_polc_y = -1.0f;
                    input::event::p1::ondown::up();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_down)
                try {
                    config::_controls::p1_polc_y = 1.0f;
                    input::event::p1::ondown::down();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_left)
                try {
                    config::_controls::p1_polc_x = -1.0f;
                    input::event::p1::ondown::left();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_right)
                try {
                    config::_controls::p1_polc_x = 1.0f;
                    input::event::p1::ondown::right();
                } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action1)
                try { input::event::p1::ondown::action1(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action2)
                try { input::event::p1::ondown::action2(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action3)
                try { input::event::p1::ondown::action3(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action4)
                try { input::event::p1::ondown::action4(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action5)
                try { input::event::p1::ondown::action5(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action6)
                try { input::event::p1::ondown::action6(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action7)
                try { input::event::p1::ondown::action7(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action8)
                try { input::event::p1::ondown::action8(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 2 *** */
            if(event.keyboard.keycode == config::controls::p2_key_up)
                try {
                    config::_controls::p2_polc_y = -1.0f;
                    input::event::p2::ondown::up();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_down)
                try {
                    config::_controls::p2_polc_y = 1.0f;
                    input::event::p2::ondown::down();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_left)
                try {
                    config::_controls::p2_polc_x = -1.0f;
                    input::event::p2::ondown::left();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_right)
                try {
                    config::_controls::p2_polc_x = 1.0f;
                    input::event::p2::ondown::right();
                } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action1)
                try { input::event::p2::ondown::action1(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action2)
                try { input::event::p2::ondown::action2(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action3)
                try { input::event::p2::ondown::action3(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action4)
                try { input::event::p2::ondown::action4(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action5)
                try { input::event::p2::ondown::action5(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action6)
                try { input::event::p2::ondown::action6(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action7)
                try { input::event::p2::ondown::action7(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action8)
                try { input::event::p2::ondown::action8(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        } catch(wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }}
        break;  //  End case ALLEGRO_EVENT_KEY_DOWN
    /////////////////////////////////////////////////////////////
    case ALLEGRO_EVENT_KEY_UP:
        if(config::flags::input_enabled) { try {
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 1 *** */
            if(event.keyboard.keycode == config::controls::p1_key_up)
                try {
                    if(config::controls::p1_polc_y != 1.0f) config::_controls::p1_polc_y = 0.0f;
                    input::event::p1::onup::up();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_down)
                try {
                    if(config::controls::p1_polc_y != -1.0f) config::_controls::p1_polc_y = 0.0f;
                    input::event::p1::onup::down();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_left)
                try {
                    if(config::controls::p1_polc_x != 1.0f) config::_controls::p1_polc_x = 0.0f;
                    input::event::p1::onup::left();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_right)
                try {
                    if(config::controls::p1_polc_x != -1.0f) config::_controls::p1_polc_x = 0.0f;
                    input::event::p1::onup::right();
                } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action1)
                try { input::event::p1::onup::action1(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action2)
                try { input::event::p1::onup::action2(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action3)
                try { input::event::p1::onup::action3(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action4)
                try { input::event::p1::onup::action4(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action5)
                try { input::event::p1::onup::action5(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action6)
                try { input::event::p1::onup::action6(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action7)
                try { input::event::p1::onup::action7(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p1_key_action8)
                try { input::event::p1::onup::action8(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
            /////////////////////////////////////////////////////////////
            /* *** PLAYER 2 *** */
            if(event.keyboard.keycode == config::controls::p2_key_up)
                try {
                    if(config::controls::p2_polc_y != 1.0f) config::_controls::p2_polc_y = 0.0f;
                    input::event::p2::onup::up();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_down)
                try {
                    if(config::controls::p2_polc_y != -1.0f) config::_controls::p2_polc_y = 0.0f;
                    input::event::p2::onup::down();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_left)
                try {
                    if(config::controls::p2_polc_x != 1.0f) config::_controls::p2_polc_x = 0.0f;
                    input::event::p2::onup::left();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_right)
                try {
                    if(config::controls::p2_polc_x != -1.0f) config::_controls::p2_polc_x = 0.0f;
                    input::event::p2::onup::right();
                } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action1)
                try { input::event::p2::onup::action1(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action2)
                try { input::event::p2::onup::action2(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action3)
                try { input::event::p2::onup::action3(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action4)
                try { input::event::p2::onup::action4(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action5)
                try { input::event::p2::onup::action5(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action6)
                try { input::event::p2::onup::action6(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action7)
                try { input::event::p2::onup::action7(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
            if(event.keyboard.keycode == config::controls::p2_key_action8)
                try { input::event::p2::onup::action8(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        } catch(wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }}
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
            } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_down)
            try {
                config::_controls::p1_polc_y = 1.0f;
                input::event::p1::ondown::down();
            } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_left)
            try {
                config::_controls::p1_polc_x = -1.0f;
                input::event::p1::ondown::left();
            } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_right)
            try {
                config::_controls::p1_polc_x = 1.0f;
                input::event::p1::ondown::right();
            } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action1)
            try { input::event::p1::ondown::action1(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action2)
            try { input::event::p1::ondown::action2(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action3)
            try { input::event::p1::ondown::action3(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action4)
            try { input::event::p1::ondown::action4(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action5)
            try { input::event::p1::ondown::action5(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action6)
            try { input::event::p1::ondown::action6(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action7)
            try { input::event::p1::ondown::action7(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action8)
            try { input::event::p1::ondown::action8(); } catch(...) { throw wte_exception("Error processing player1 ondown input", "input", engine_time::check()); }
        /////////////////////////////////////////////////////////////
        /* *** PLAYER 2 *** */
        if(event.joystick.button == config::controls::p2_button_up)
            try { 
                config::_controls::p2_polc_y = -1.0f;
                input::event::p2::ondown::up();
            } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_down)
            try {
                config::_controls::p2_polc_y = 1.0f;
                input::event::p2::ondown::down();
            } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_left)
            try {
                config::_controls::p2_polc_x = -1.0f;
                input::event::p1::ondown::left();
            } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_right)
            try {
                config::_controls::p2_polc_x = 1.0f;
                input::event::p2::ondown::right();
            } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action1)
            try { input::event::p2::ondown::action1(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action2)
            try { input::event::p2::ondown::action2(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action3)
            try { input::event::p2::ondown::action3(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action4)
            try { input::event::p2::ondown::action4(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action5)
            try { input::event::p2::ondown::action5(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action6)
            try { input::event::p2::ondown::action6(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action7)
            try { input::event::p2::ondown::action7(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action8)
            try { input::event::p2::ondown::action8(); } catch(...) { throw wte_exception("Error processing player2 ondown input", "input", engine_time::check()); }
        //  Record last button pressed.
        _lastbuttonpress::timer = engine_time::check();
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
            } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_down)
            try {
                if(config::controls::p1_polc_y != -1.0f) config::_controls::p1_polc_y = 0.0f;
                input::event::p1::onup::down();
            } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_left)
            try {
                if(config::controls::p1_polc_x != 1.0f) config::_controls::p1_polc_x = 0.0f;
                input::event::p1::onup::left();
            } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_right)
            try {
                if(config::controls::p1_polc_x != -1.0f) config::_controls::p1_polc_x = 0.0f;
                input::event::p1::onup::right();
            } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action1)
            try { input::event::p1::onup::action1(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action2)
            try { input::event::p1::onup::action2(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action3)
            try { input::event::p1::onup::action3(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action4)
            try { input::event::p1::onup::action4(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action5)
            try { input::event::p1::onup::action5(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action6)
            try { input::event::p1::onup::action6(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action7)
            try { input::event::p1::onup::action7(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p1_button_action8)
            try { input::event::p1::onup::action8(); } catch(...) { throw wte_exception("Error processing player1 onup input", "input", engine_time::check()); }
        /////////////////////////////////////////////////////////////
        /* *** PLAYER 2 *** */
        if(event.joystick.button == config::controls::p2_button_up)
            try { 
                if(config::controls::p2_polc_y != 1.0f) config::_controls::p2_polc_y = 0.0f;
                input::event::p2::onup::up();
            } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_down)
            try {
                if(config::controls::p2_polc_y != -1.0f) config::_controls::p2_polc_y = 0.0f;
                input::event::p2::onup::down();
            } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_left)
            try {
                if(config::controls::p2_polc_x != 1.0f) config::_controls::p2_polc_x = 0.0f;
                input::event::p1::onup::left();
            } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_right)
            try {
                if(config::controls::p2_polc_x != -1.0f) config::_controls::p2_polc_x = 0.0f;
                input::event::p2::onup::right();
            } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action1)
            try { input::event::p2::onup::action1(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action2)
            try { input::event::p2::onup::action2(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action3)
            try { input::event::p2::onup::action3(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action4)
            try { input::event::p2::onup::action4(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action5)
            try { input::event::p2::onup::action5(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action6)
            try { input::event::p2::onup::action6(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action7)
            try { input::event::p2::onup::action7(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        if(event.joystick.button == config::controls::p2_button_action8)
            try { input::event::p2::onup::action8(); } catch(...) { throw wte_exception("Error processing player2 onup input", "input", engine_time::check()); }
        break;  //  end ALLEGRO_EVENT_JOYSTICK_BUTTON_UP
    }  //  End switch(event.type)
}

}  //  end namespace wte
