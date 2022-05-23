/*!
 * WTEngine | File:  config.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/config.hpp"

namespace wte {

bool config::initialized = false;

const bool& config::flags::is_running = config::_flags::is_running;
const bool& config::flags::game_started = config::_flags::game_started;
const bool& config::flags::menu_opened = config::_flags::menu_opened;
const bool& config::flags::record_input = config::_flags::record_input;
const bool& config::flags::keyboard_detected = config::_flags::keyboard_detected;
const bool& config::flags::mouse_detected = config::_flags::mouse_detected;
const bool& config::flags::joystick_detected = config::_flags::joystick_detected;
const bool& config::flags::touch_detected = config::_flags::touch_detected;
const bool& config::flags::audio_installed = config::_flags::audio_installed;
const bool& config::flags::show_hitboxes = config::_flags::show_hitboxes;

const float& config::volume::main = config::_volume::main;
const float& config::volume::music = config::_volume::music;
const float& config::volume::music_a = config::_volume::music_a;
const float& config::volume::music_b = config::_volume::music_b;
const float& config::volume::sample = config::_volume::sample;
const float& config::volume::voice = config::_volume::voice;
const float& config::volume::ambiance = config::_volume::ambiance;

const int& config::gfx::screen_w = config::_gfx::screen_w;
const int& config::gfx::screen_h = config::_gfx::screen_h;
const int& config::gfx::arena_w = config::_gfx::arena_w;
const int& config::gfx::arena_h = config::_gfx::arena_h;
const std::size_t& config::gfx::vsync = config::_gfx::vsync;
const std::size_t& config::gfx::display_mode = config::_gfx::display_mode;
const float& config::gfx::scale_factor = config::_gfx::scale_factor;
const bool& config::gfx::needs_reconfig = config::_gfx::needs_reconfig;

const int& config::controls::key_menu = config::_controls::key_menu;

const int& config::controls::p1_key_up = config::_controls::p1_key_up;
const int& config::controls::p1_key_down = config::_controls::p1_key_down;
const int& config::controls::p1_key_left = config::_controls::p1_key_left;
const int& config::controls::p1_key_right = config::_controls::p1_key_right;
const int& config::controls::p1_key_action1 = config::_controls::p1_key_action1;
const int& config::controls::p1_key_action2 = config::_controls::p1_key_action2;
const int& config::controls::p1_key_action3 = config::_controls::p1_key_action3;
const int& config::controls::p1_key_action4 = config::_controls::p1_key_action4;
const int& config::controls::p1_key_action5 = config::_controls::p1_key_action5;
const int& config::controls::p1_key_action6 = config::_controls::p1_key_action6;
const int& config::controls::p1_key_action7 = config::_controls::p1_key_action7;
const int& config::controls::p1_key_action8 = config::_controls::p1_key_action8;

const int& config::controls::p1_button_up = config::_controls::p1_button_up;
const int& config::controls::p1_button_down = config::_controls::p1_button_down;
const int& config::controls::p1_button_left = config::_controls::p1_button_left;
const int& config::controls::p1_button_right = config::_controls::p1_button_right;
const int& config::controls::p1_button_action1 = config::_controls::p1_button_action1;
const int& config::controls::p1_button_action2 = config::_controls::p1_button_action2;
const int& config::controls::p1_button_action3 = config::_controls::p1_button_action3;
const int& config::controls::p1_button_action4 = config::_controls::p1_button_action4;
const int& config::controls::p1_button_action5 = config::_controls::p1_button_action5;
const int& config::controls::p1_button_action6 = config::_controls::p1_button_action6;
const int& config::controls::p1_button_action7 = config::_controls::p1_button_action7;
const int& config::controls::p1_button_action8 = config::_controls::p1_button_action8;
const int& config::controls::p1_button_start = config::_controls::p1_button_start;
const int& config::controls::p1_button_select = config::_controls::p1_button_select;
const float& config::controls::p1_pola_x = config::_controls::p1_pola_x;
const float& config::controls::p1_pola_y = config::_controls::p1_pola_y;
const float& config::controls::p1_polb_x = config::_controls::p1_polb_x;
const float& config::controls::p1_polb_y = config::_controls::p1_polb_y;
const float& config::controls::p1_polc_x = config::_controls::p1_polc_x;
const float& config::controls::p1_polc_y = config::_controls::p1_polc_y;
const float& config::controls::p1_throttle_left = config::_controls::p1_throttle_left;
const float& config::controls::p1_throttle_right = config::_controls::p1_throttle_right;

const int& config::controls::p2_key_up = config::_controls::p2_key_up;
const int& config::controls::p2_key_down = config::_controls::p2_key_down;
const int& config::controls::p2_key_left = config::_controls::p2_key_left;
const int& config::controls::p2_key_right = config::_controls::p2_key_right;
const int& config::controls::p2_key_action1 = config::_controls::p2_key_action1;
const int& config::controls::p2_key_action2 = config::_controls::p2_key_action2;
const int& config::controls::p2_key_action3 = config::_controls::p2_key_action3;
const int& config::controls::p2_key_action4 = config::_controls::p2_key_action4;
const int& config::controls::p2_key_action5 = config::_controls::p2_key_action5;
const int& config::controls::p2_key_action6 = config::_controls::p2_key_action6;
const int& config::controls::p2_key_action7 = config::_controls::p2_key_action7;
const int& config::controls::p2_key_action8 = config::_controls::p2_key_action8;

const int& config::controls::p2_button_up = config::_controls::p2_button_up;
const int& config::controls::p2_button_down = config::_controls::p2_button_down;
const int& config::controls::p2_button_left = config::_controls::p2_button_left;
const int& config::controls::p2_button_right = config::_controls::p2_button_right;
const int& config::controls::p2_button_action1 = config::_controls::p2_button_action1;
const int& config::controls::p2_button_action2 = config::_controls::p2_button_action2;
const int& config::controls::p2_button_action3 = config::_controls::p2_button_action3;
const int& config::controls::p2_button_action4 = config::_controls::p2_button_action4;
const int& config::controls::p2_button_action5 = config::_controls::p2_button_action5;
const int& config::controls::p2_button_action6 = config::_controls::p2_button_action6;
const int& config::controls::p2_button_action7 = config::_controls::p2_button_action7;
const int& config::controls::p2_button_action8 = config::_controls::p2_button_action8;
const int& config::controls::p2_button_start = config::_controls::p2_button_start;
const int& config::controls::p2_button_select = config::_controls::p2_button_select;
const float& config::controls::p2_pola_x = config::_controls::p2_pola_x;
const float& config::controls::p2_pola_y = config::_controls::p2_pola_y;
const float& config::controls::p2_polb_x = config::_controls::p2_polb_x;
const float& config::controls::p2_polb_y = config::_controls::p2_polb_y;
const float& config::controls::p2_polc_x = config::_controls::p2_polc_x;
const float& config::controls::p2_polc_y = config::_controls::p2_polc_y;
const float& config::controls::p2_throttle_left = config::_controls::p2_throttle_left;
const float& config::controls::p2_throttle_right = config::_controls::p2_throttle_right;

/*
 *
 */
config::config() {
    if(initialized == true) throw std::runtime_error("Config instance already running!");
    initialized = true;
}

/*
 *
 */
const bool config::load(void) {
    std::ifstream dfile("settings.cfg", std::ios::binary);
    //  Data file doesn't exist, create one
    if(!dfile.good()) {
        if(!save()) return false;
        dfile.open("settings.cfg", std::ios::binary);
    }
    if(!dfile.good()) return false;

    try {
        dfile.read(reinterpret_cast<char*>(&flags::draw_fps), sizeof flags::draw_fps);
        dfile.read(reinterpret_cast<char*>(&_gfx::vsync), sizeof _gfx::vsync);
        dfile.read(reinterpret_cast<char*>(&_gfx::display_mode), sizeof _gfx::display_mode);
        dfile.read(reinterpret_cast<char*>(&_gfx::scale_factor), sizeof _gfx::scale_factor);
        dfile.read(reinterpret_cast<char*>(&_volume::main), sizeof _volume::main);
        dfile.read(reinterpret_cast<char*>(&_volume::music), sizeof _volume::music);
        dfile.read(reinterpret_cast<char*>(&_volume::sample), sizeof _volume::sample);
        dfile.read(reinterpret_cast<char*>(&_volume::voice), sizeof _volume::voice);
        dfile.read(reinterpret_cast<char*>(&_volume::ambiance), sizeof _volume::ambiance);
        dfile.read(reinterpret_cast<char*>(&_controls::key_menu), sizeof _controls::key_menu);

        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_up), sizeof _controls::p1_key_up);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_down), sizeof _controls::p1_key_down);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_left), sizeof _controls::p1_key_left);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_right), sizeof _controls::p1_key_right);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action1), sizeof _controls::p1_key_action1);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action2), sizeof _controls::p1_key_action2);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action3), sizeof _controls::p1_key_action3);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action4), sizeof _controls::p1_key_action4);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action5), sizeof _controls::p1_key_action5);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action6), sizeof _controls::p1_key_action6);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action7), sizeof _controls::p1_key_action7);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_key_action8), sizeof _controls::p1_key_action8);

        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_up), sizeof _controls::p1_button_up);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_down), sizeof _controls::p1_button_down);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_left), sizeof _controls::p1_button_left);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_right), sizeof _controls::p1_button_right);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action1), sizeof _controls::p1_button_action1);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action2), sizeof _controls::p1_button_action2);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action3), sizeof _controls::p1_button_action3);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action4), sizeof _controls::p1_button_action4);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action5), sizeof _controls::p1_button_action5);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action6), sizeof _controls::p1_button_action6);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action7), sizeof _controls::p1_button_action7);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_action8), sizeof _controls::p1_button_action8);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_start), sizeof _controls::p1_button_start);
        dfile.read(reinterpret_cast<char*>(&_controls::p1_button_select), sizeof _controls::p1_button_select);

        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_up), sizeof _controls::p2_key_up);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_down), sizeof _controls::p2_key_down);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_left), sizeof _controls::p2_key_left);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_right), sizeof _controls::p2_key_right);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action1), sizeof _controls::p2_key_action1);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action2), sizeof _controls::p2_key_action2);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action3), sizeof _controls::p2_key_action3);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action4), sizeof _controls::p2_key_action4);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action5), sizeof _controls::p2_key_action5);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action6), sizeof _controls::p2_key_action6);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action7), sizeof _controls::p2_key_action7);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_key_action8), sizeof _controls::p2_key_action8);

        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_up), sizeof _controls::p2_button_up);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_down), sizeof _controls::p2_button_down);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_left), sizeof _controls::p2_button_left);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_right), sizeof _controls::p2_button_right);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action1), sizeof _controls::p2_button_action1);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action2), sizeof _controls::p2_button_action2);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action3), sizeof _controls::p2_button_action3);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action4), sizeof _controls::p2_button_action4);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action5), sizeof _controls::p2_button_action5);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action6), sizeof _controls::p2_button_action6);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action7), sizeof _controls::p2_button_action7);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_action8), sizeof _controls::p2_button_action8);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_start), sizeof _controls::p2_button_start);
        dfile.read(reinterpret_cast<char*>(&_controls::p2_button_select), sizeof _controls::p2_button_select);
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
const bool config::save(void) {
    std::ofstream dfile("settings.cfg", std::ios::binary | std::ofstream::trunc);
    if(!dfile.good()) return false;

    try {
        dfile.write(reinterpret_cast<const char*>(&flags::draw_fps), sizeof flags::draw_fps);
        dfile.write(reinterpret_cast<const char*>(&gfx::vsync), sizeof gfx::vsync);
        dfile.write(reinterpret_cast<const char*>(&gfx::display_mode), sizeof gfx::display_mode);
        dfile.write(reinterpret_cast<const char*>(&gfx::scale_factor), sizeof gfx::scale_factor);
        dfile.write(reinterpret_cast<const char*>(&volume::main), sizeof volume::main);
        dfile.write(reinterpret_cast<const char*>(&volume::music), sizeof volume::music);
        dfile.write(reinterpret_cast<const char*>(&volume::sample), sizeof volume::sample);
        dfile.write(reinterpret_cast<const char*>(&volume::voice), sizeof volume::voice);
        dfile.write(reinterpret_cast<const char*>(&volume::ambiance), sizeof volume::ambiance);
        dfile.write(reinterpret_cast<const char*>(&controls::key_menu), sizeof controls::key_menu);
        
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_up), sizeof controls::p1_key_up);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_down), sizeof controls::p1_key_down);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_left), sizeof controls::p1_key_left);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_right), sizeof controls::p1_key_right);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action1), sizeof controls::p1_key_action1);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action2), sizeof controls::p1_key_action2);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action3), sizeof controls::p1_key_action3);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action4), sizeof controls::p1_key_action4);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action5), sizeof controls::p1_key_action5);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action6), sizeof controls::p1_key_action6);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action7), sizeof controls::p1_key_action7);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action8), sizeof controls::p1_key_action8);
        
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_up), sizeof controls::p1_button_up);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_down), sizeof controls::p1_button_down);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_left), sizeof controls::p1_button_left);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_right), sizeof controls::p1_button_right);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action1), sizeof controls::p1_button_action1);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action2), sizeof controls::p1_button_action2);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action3), sizeof controls::p1_button_action3);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action4), sizeof controls::p1_button_action4);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action5), sizeof controls::p1_button_action5);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action6), sizeof controls::p1_button_action6);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action7), sizeof controls::p1_button_action7);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action8), sizeof controls::p1_button_action8);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_start), sizeof controls::p1_button_start);
        dfile.write(reinterpret_cast<const char*>(&controls::p1_button_select), sizeof controls::p1_button_select);
        
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_up), sizeof controls::p2_key_up);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_down), sizeof controls::p2_key_down);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_left), sizeof controls::p2_key_left);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_right), sizeof controls::p2_key_right);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action1), sizeof controls::p2_key_action1);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action2), sizeof controls::p2_key_action2);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action3), sizeof controls::p2_key_action3);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action4), sizeof controls::p2_key_action4);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action5), sizeof controls::p2_key_action5);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action6), sizeof controls::p2_key_action6);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action7), sizeof controls::p2_key_action7);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action8), sizeof controls::p2_key_action8);

        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_up), sizeof controls::p2_button_up);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_down), sizeof controls::p2_button_down);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_left), sizeof controls::p2_button_left);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_right), sizeof controls::p2_button_right);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action1), sizeof controls::p2_button_action1);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action2), sizeof controls::p2_button_action2);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action3), sizeof controls::p2_button_action3);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action4), sizeof controls::p2_button_action4);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action5), sizeof controls::p2_button_action5);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action6), sizeof controls::p2_button_action6);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action7), sizeof controls::p2_button_action7);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action8), sizeof controls::p2_button_action8);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_start), sizeof controls::p2_button_start);
        dfile.write(reinterpret_cast<const char*>(&controls::p2_button_select), sizeof controls::p2_button_select);
    } catch(...) {
        dfile.close();
        return false;
    }

    dfile.close();
    return true;
}

}  //  end namespace wte
