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

namespace wte
{

//!  Engine config
/*!
 * Statics to store engine config variables
 */
class engine_cfg final {
    public:
        //!  Set the timer rate
        static const float TICKS_PER_SECOND;

        //!  Set screen width
        static unsigned int screen_width;
        //!  Set screen height
        static unsigned int screen_height;
};

inline unsigned int engine_cfg::screen_width;
inline unsigned int engine_cfg::screen_height;

} //  end namespace wte

#endif
