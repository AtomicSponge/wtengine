/*!
 * \brief WTEngine | File:  game_cfg_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine config.
 */

#ifndef WTE_GAME_CFG_MAP_HPP
#define WTE_GAME_CFG_MAP_HPP

#include "variable_map.hpp"

namespace wte
{

//!  Game config
/*!
 * Statics to store game config variables
 */
class game_cfg_map final : public variable_map<game_cfg_map> {};

typedef game_cfg_map::variable_map<game_cfg_map> game_cfg;

template <> inline std::map<std::string, std::string> game_cfg::_map = {};

} //  end namespace wte

#endif
