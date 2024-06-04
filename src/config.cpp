/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/config.hpp"

namespace wte {

const int& config::gfx::screen_w = config::_gfx::screen_w;
const int& config::gfx::screen_h = config::_gfx::screen_h;
const int& config::gfx::viewport_w = config::_gfx::viewport_w;
const int& config::gfx::viewport_h = config::_gfx::viewport_h;
const std::size_t& config::gfx::vsync = config::_gfx::vsync;
const std::size_t& config::gfx::display_mode = config::_gfx::display_mode;
const float& config::gfx::scale_factor = config::_gfx::scale_factor;

}  //  end namespace wte
