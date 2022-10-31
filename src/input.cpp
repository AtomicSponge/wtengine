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

ALLEGRO_EVENT_QUEUE* input::input_event_queue;
std::ofstream input::input_event_file;
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

    if(build_options.keyboard_enabled && config::flags::keyboard_installed)
        al_register_event_source(input_event_queue, al_get_keyboard_event_source());
    if(build_options.mouse_enabled && config::flags::mouse_installed)
        al_register_event_source(input_event_queue, al_get_mouse_event_source());
    if(build_options.joystick_enabled && config::flags::joystick_installed)
        al_register_event_source(input_event_queue, al_get_joystick_event_source());
    if(build_options.touch_enabled && config::flags::touch_installed)
        al_register_event_source(input_event_queue, al_get_touch_input_event_source());
}

/*
 *
 */
void input::destroy_event_queue(void) { al_destroy_event_queue(input_event_queue); }

/*
 *
 */
void input::start_recording(void) {
    input_event_file.open("input_events", std::ios::binary | std::ofstream::app);
    config::_flags::record_input = true;
}

/*
 *
 */
void input::stop_recording(void) {
    if(input_event_file.is_open()) input_event_file.close();
    config::_flags::record_input = false;
}

/*
 *
 */
void input::record_event(const int64_t& time, const ALLEGRO_EVENT& event) { 
    input_event_file.write(reinterpret_cast<const char*>(time), sizeof(int64_t));
    input_event_file.write(reinterpret_cast<const char*>(sizeof(event)), sizeof(std::size_t));
    input_event_file.write(reinterpret_cast<const char*>(&event), sizeof(event));
}

/*
 *
 */
void input::check_events(void) {
    ALLEGRO_EVENT event;
    while(al_get_next_event(input_event_queue, &event)) {
        //  Process Dear ImGui events
        if constexpr(build_options.use_imgui)
            ImGui_ImplAllegro5_ProcessEvent(&event);
        //  Record input if enabled.
        if(config::flags::record_input) record_event(engine_time::check(), event);
        //  Run the handles
        run_handles<GLOBAL_HANDLES>(event);        //  Run global handles
        if(config::flags::engine_started) {
            if(config::flags::input_enabled)
                run_handles<GAME_HANDLES>(event);  //  Run game handles
        } else {
            run_handles<NONGAME_HANDLES>(event);   //  Run non-game handles
        }
    }
}

}  //  end namespace wte
