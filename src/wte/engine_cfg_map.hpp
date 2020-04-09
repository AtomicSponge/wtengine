/*!
 * \brief WTEngine | File:  engine_cfg_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine config.
 */

#ifndef WTE_ENGINE_CFG_MAP_HPP
#define WTE_ENGINE_CFG_MAP_HPP

#include "variable_map.hpp"

namespace wte
{

//!  Engine config
/*!
 * Statics to store engine config variables
 */
class engine_cfg_map final : public variable_map<engine_cfg_map> {
    public:
        inline engine_cfg_map() {};
        inline ~engine_cfg_map() {};
};

typedef engine_cfg_map::variable_map<engine_cfg_map> engine_cfg;

template <> inline std::map<std::string, std::string> engine_cfg::_map = {};

} //  end namespace wte

#endif
