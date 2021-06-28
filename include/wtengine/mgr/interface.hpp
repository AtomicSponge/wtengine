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
#include "wtengine/mgr/menu.hpp"
#include "wtengine/mgr/renderer.hpp"
#include "wtengine/mgr/spawner.hpp"
#include "wtengine/mgr/systems.hpp"

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
        /*!
         *
         */
        inline void menu_init(void) {
            mgr::menu::initialize();
        };

        /*!
         *
         */
        inline void menu_de_init(void) {
            mgr::menu::de_init();
        };

        /* ************************************ */
        /* *** RENDERER INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         *
         */
        inline void renderer_init(void) {
            mgr::renderer::initialize();
        };

        /*!
         *
         */
        inline void renderer_de_init(void) {
            mgr::renderer::de_init();
        };

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
