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

#include <allegro5/allegro.h>

#include "wtengine/message.hpp"
#include "wtengine/mgr/audio.hpp"
#include "wtengine/mgr/menu.hpp"
#include "wtengine/mgr/messages.hpp"
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
        inline void audio_get_volume(void) {
            mgr::audio::get_volume();
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

        /*!
         *
         */
        inline void menu_reload_bitmap(void) {
            mgr::menu::reload_menu_bitmap();
        };

        /*!
         *
         */
        inline void menu_run(void) {
            mgr::menu::run();
        };

        /* ************************************ */
        /* *** MESSAGES INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         *
         */
        inline void messages_load_file(const std::string& fname) {
            mgr::messages::load_file(fname);
        };

        /*!
         *
         */
        inline const message_container messages_get(const std::string& arg) {
            return mgr::messages::get_messages(arg);
        };

        /*!
         *
         */
        inline void messages_prune(void) {
            mgr::messages::prune();
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

        //static void reload_arena_bitmap(void);
        /*!
         *
         */
        inline void renderer_reload_arena_bitmap(void) {
            mgr::renderer::reload_arena_bitmap();
        };

        /*!
         *
         */
        inline void renderer_render(void) {
            mgr::renderer::render();
        };

        /* *********************************** */
        /* *** SPAWNER INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         *
         */
        inline void spawner_process_messages(const message_container& messages) {
            mgr::spawner::process_messages(messages);
        };

        /* *********************************** */
        /* *** SYSTEMS INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         *
         */
        inline void systems_clear(void) {
            mgr::systems::clear();
        };

        /*!
         *
         */
        inline void systems_finalize(void) {
            mgr::systems::finalize();
        };

        /*!
         *
         */
        inline const bool systems_empty(void) {
            return mgr::systems::empty();
        };

        /*!
         *
         */
        inline void systems_run(void) {
            mgr::systems::run();
        };

        /*!
         *
         */
        inline void systems_dispatch(void) {
            mgr::systems::dispatch();
        };

        /*!
         *
         */
        inline void systems_reload_temp_bitmaps(void) {
            mgr::systems::reload_temp_bitmaps();
        };
};

} //  end namespace mgr

} //  end namespace wte

#endif
