/*!
 * WTEngine | File:  interface.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_INTERFACE_HPP
#define WTE_MGR_INTERFACE_HPP

#include "wtengine/mgr/audio.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class interface
 * \brief Allows the internal engine to access private members of managers.
 * 
 * .
 */
class interface {
    public:
        inline interface() {};
        inline ~interface() {};

        void audio_init(void) {
            mgr::audio::initialize();
        };

        void audio_de_init(void) {
            mgr::audio::de_init();
        };
};

} //  end namespace mgr

} //  end namespace wte

#endif
