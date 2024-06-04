/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/config.hpp"

namespace wte {

bool config::initialized = false;

const bool& config::flags::is_running = config::_flags::is_running;
const bool& config::flags::engine_started = config::_flags::engine_started;
const bool& config::flags::record_input = config::_flags::record_input;
const bool& config::flags::keyboard_installed = config::_flags::keyboard_installed;
const bool& config::flags::mouse_installed = config::_flags::mouse_installed;
const bool& config::flags::joystick_installed = config::_flags::joystick_installed;
const bool& config::flags::touch_installed = config::_flags::touch_installed;
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
const int& config::gfx::viewport_w = config::_gfx::viewport_w;
const int& config::gfx::viewport_h = config::_gfx::viewport_h;
const std::size_t& config::gfx::vsync = config::_gfx::vsync;
const std::size_t& config::gfx::display_mode = config::_gfx::display_mode;
const float& config::gfx::scale_factor = config::_gfx::scale_factor;

}  //  end namespace wte
