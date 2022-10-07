/*!
 * wtengine | File:  input.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/input.hpp"

namespace wte {

const ALLEGRO_KEYBOARD_STATE& input::states::keyboard = input::_states::keyboard;
const ALLEGRO_MOUSE_STATE& input::states::mouse = input::_states::mouse;
const ALLEGRO_TOUCH_INPUT_STATE& input::states::touches = input::_states::touches;
const ALLEGRO_TOUCH_STATE& input::states::first_touch = input::_states::first_touch;
const ALLEGRO_TOUCH_STATE& input::states::last_touch = input::_states::last_touch;
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
bool input::initialized = false;

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
        //  Toggle menus
        if(event.type == ALLEGRO_EVENT_KEY_DOWN &&
           event.keyboard.keycode == config::controls::key_menu)
            config::_flags::menu_opened = !config::flags::menu_opened;
        //  Record input if enabled.
        if(config::flags::record_input) record_event(event);
        run_handles<GLOBAL_HANDLES>(event);
        (config::flags::game_started ?
            run_handles<GAME_HANDLES>(event) :
            run_handles<NONGAME_HANDLES>(event));
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

}  //  end namespace wte
