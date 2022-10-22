/*!
 * wtengine | File:  input.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/input.hpp"

namespace wte {

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
    if(initialized == true) throw engine_error("Input instance already running!");
    initialized = true;
}

/*
 *
 */
void input::create_event_queue(void) {
    input_event_queue = al_create_event_queue();
    if(!input_event_queue) throw engine_error("Failed to create input event queue!");

    if(build_options.keyboard_enabled && config::flags::keyboard_detected)
        al_register_event_source(input_event_queue, al_get_keyboard_event_source());
    if(build_options.mouse_enabled && config::flags::mouse_detected)
        al_register_event_source(input_event_queue, al_get_mouse_event_source());
    if(build_options.joystick_enabled && config::flags::joystick_detected)
        al_register_event_source(input_event_queue, al_get_joystick_event_source());
    if(build_options.touch_enabled && config::flags::touch_detected)
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
bool input::save_recorder(void) {
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
bool input::check_events(void) {
    ALLEGRO_EVENT event;
    while(al_get_next_event(input_event_queue, &event)) {
        //  Record input if enabled.
        if(config::flags::record_input) record_event(event);
        //  Run the handles
        run_handles<GLOBAL_HANDLES>(event);        //  Run global handles
        (config::flags::game_started ?
            run_handles<GAME_HANDLES>(event) :     //  Run game handles
            run_handles<NONGAME_HANDLES>(event));  //  Run non-game handles
    }
    return true;
}

}  //  end namespace wte
