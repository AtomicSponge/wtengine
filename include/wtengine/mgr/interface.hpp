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

        /* ********************************* */
        /* *** AUDIO INTERFACE FUNCTIONS *** */
        /* ********************************* */
        /*!
         *
         */
        inline void audio_init(void) {
            mgr::audio::initialize();
        };

        /*!
         *
         */
        inline void audio_de_init(void) {
            mgr::audio::de_init();
        };

        /*!
         *
         */
        inline void audio_set_volume(void) {
            mgr::audio::set_volume();
        };

        /*!
         *
         */
        inline void audio_process_messages(const message_container& messages) {
            mgr::audio::process_messages(messages);
        };

        /* ******************************** */
        /* *** MENU INTERFACE FUNCTIONS *** */
        /* ******************************** */

        /* ************************************ */
        /* *** RENDERER INTERFACE FUNCTIONS *** */
        /* ************************************ */

        /* *********************************** */
        /* *** SPAWNER INTERFACE FUNCTIONS *** */
        /* *********************************** */

        /* *********************************** */
        /* *** SYSTEMS INTERFACE FUNCTIONS *** */
        /* *********************************** */
};

} //  end namespace mgr

} //  end namespace wte

#endif
