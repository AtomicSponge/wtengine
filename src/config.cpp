/*!
 * WTEngine | File:  config.cpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/config.hpp"

namespace wte
{

/*
 *
 */
const bool config::load(void) {
    std::ifstream dfile("settings.cfg", std::ios::binary);
    //  Data file doesn't exist, create one
    if(!dfile.good()) {
        save();
        dfile.open("settings.cfg");
    }
    if(!dfile.good()) return false;

    try {
        dfile.read(reinterpret_cast<char*>(&config::flags::draw_fps), sizeof config::flags::draw_fps);
        dfile.read(reinterpret_cast<char*>(&config::gfx::vsync), sizeof config::gfx::vsync);
        dfile.read(reinterpret_cast<char*>(&config::gfx::display_mode), sizeof config::gfx::display_mode);
        dfile.read(reinterpret_cast<char*>(&config::gfx::scale_factor), sizeof config::gfx::scale_factor);
        dfile.read(reinterpret_cast<char*>(&config::volume::main), sizeof config::volume::main);
        dfile.read(reinterpret_cast<char*>(&config::volume::mix1), sizeof config::volume::mix1);
        dfile.read(reinterpret_cast<char*>(&config::volume::mix2), sizeof config::volume::mix2);
        dfile.read(reinterpret_cast<char*>(&config::volume::mix3), sizeof config::volume::mix3);
        dfile.read(reinterpret_cast<char*>(&config::volume::mix4), sizeof config::volume::mix4);
        dfile.read(reinterpret_cast<char*>(&config::controls::key_menu), sizeof config::controls::key_menu);
        dfile.read(reinterpret_cast<char*>(&config::controls::key_menu_select), sizeof config::controls::key_menu_select);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_up), sizeof config::controls::p1_key_up);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_down), sizeof config::controls::p1_key_down);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_left), sizeof config::controls::p1_key_left);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_right), sizeof config::controls::p1_key_right);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action1), sizeof config::controls::p1_key_action1);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action2), sizeof config::controls::p1_key_action2);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action3), sizeof config::controls::p1_key_action3);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action4), sizeof config::controls::p1_key_action4);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action5), sizeof config::controls::p1_key_action5);
        dfile.read(reinterpret_cast<char*>(&config::controls::p1_key_action6), sizeof config::controls::p1_key_action6);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_up), sizeof config::controls::p2_key_up);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_down), sizeof config::controls::p2_key_down);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_left), sizeof config::controls::p2_key_left);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_right), sizeof config::controls::p2_key_right);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action1), sizeof config::controls::p2_key_action1);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action2), sizeof config::controls::p2_key_action2);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action3), sizeof config::controls::p2_key_action3);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action4), sizeof config::controls::p2_key_action4);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action5), sizeof config::controls::p2_key_action5);
        dfile.read(reinterpret_cast<char*>(&config::controls::p2_key_action6), sizeof config::controls::p2_key_action6);
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
    std::ofstream dfile("settings.cfg", std::ios::binary);
    if(!dfile.good()) return false;

    try {
        dfile.write(reinterpret_cast<const char*>(&config::flags::draw_fps), sizeof config::flags::draw_fps);
        dfile.write(reinterpret_cast<const char*>(&config::gfx::vsync), sizeof config::gfx::vsync);
        dfile.write(reinterpret_cast<const char*>(&config::gfx::display_mode), sizeof config::gfx::display_mode);
        dfile.write(reinterpret_cast<const char*>(&config::gfx::scale_factor), sizeof config::gfx::scale_factor);
        dfile.write(reinterpret_cast<const char*>(&config::volume::main), sizeof config::volume::main);
        dfile.write(reinterpret_cast<const char*>(&config::volume::mix1), sizeof config::volume::mix1);
        dfile.write(reinterpret_cast<const char*>(&config::volume::mix2), sizeof config::volume::mix2);
        dfile.write(reinterpret_cast<const char*>(&config::volume::mix3), sizeof config::volume::mix3);
        dfile.write(reinterpret_cast<const char*>(&config::volume::mix4), sizeof config::volume::mix4);
        dfile.write(reinterpret_cast<const char*>(&config::controls::key_menu), sizeof config::controls::key_menu);
        dfile.write(reinterpret_cast<const char*>(&config::controls::key_menu_select), sizeof config::controls::key_menu_select);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_up), sizeof config::controls::p1_key_up);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_down), sizeof config::controls::p1_key_down);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_left), sizeof config::controls::p1_key_left);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_right), sizeof config::controls::p1_key_right);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action1), sizeof config::controls::p1_key_action1);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action2), sizeof config::controls::p1_key_action2);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action3), sizeof config::controls::p1_key_action3);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action4), sizeof config::controls::p1_key_action4);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action5), sizeof config::controls::p1_key_action5);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p1_key_action6), sizeof config::controls::p1_key_action6);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_up), sizeof config::controls::p2_key_up);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_down), sizeof config::controls::p2_key_down);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_left), sizeof config::controls::p2_key_left);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_right), sizeof config::controls::p2_key_right);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action1), sizeof config::controls::p2_key_action1);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action2), sizeof config::controls::p2_key_action2);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action3), sizeof config::controls::p2_key_action3);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action4), sizeof config::controls::p2_key_action4);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action5), sizeof config::controls::p2_key_action5);
        dfile.write(reinterpret_cast<const char*>(&config::controls::p2_key_action6), sizeof config::controls::p2_key_action6);
    } catch(...) {
        dfile.close();
        return false;
    }

    dfile.close();
    return true;
}

} //  end namespace wte
