/*!
 * \brief WTEngine | File:  engine_cfg.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine config.
 */

#ifndef WTE_ENGINE_CFG_HPP
#define WTE_ENGINE_CFG_HPP

#include <string>
#include <map>

namespace wte
{

//!  Engine config
/*!
 * Statics to store engine config variables
 */
class engine_cfg {
    public:
        //!  Remove copy constructor
        engine_cfg(const engine_cfg&) = delete;
        //!  Remove assignment operator
        void operator=(engine_cfg const&) = delete;

        //static reg(const std::string, data);
        //static set(const std::string, data);
        //<type>get(const std::string);

        //!  Set the timer rate
        static const float TICKS_PER_SECOND;

        //!  Set screen width
        static unsigned int screen_width;
        //!  Set screen height
        static unsigned int screen_height;

    private:
        inline engine_cfg() {};
        inline ~engine_cfg() {};

        static std::map<std::string, int> engine_cfg_map; 
};

inline unsigned int engine_cfg::screen_width;
inline unsigned int engine_cfg::screen_height;

} //  end namespace wte

#endif
