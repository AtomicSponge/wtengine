/*!
 * WTEngine | File:  interface.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_INTERFACE_HPP
#define WTE_MGR_INTERFACE_HPP

#include <allegro5/allegro.h>

#include "wtengine/mgr/assets.hpp"
#include "wtengine/mgr/manager.hpp"
#include "wtengine/mgr/audio.hpp"
#include "wtengine/mgr/menus.hpp"
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
 * The engine class initializes this as a private member.
 * This way the engine object can be extended, but only the
 * engine internals will be able to access these commands.
 */
class interface : private manager<interface>  {
    public:
        inline interface() {};
        inline ~interface() {};

        /* ********************************* */
        /* *** AUDIO INTERFACE FUNCTIONS *** */
        /* ********************************* */
        /*!
         * \brief Initialize audio manager,
         */
        inline void audio_init(void) {
            mgr::audio::initialize();
        };

        /*!
         * \brief De-initialize audio manager,
         */
        inline void audio_de_init(void) {
            mgr::audio::de_init();
        };

        /*!
         * \brief Set audio manager volume,
         */
        inline void audio_set_volume(void) {
            mgr::audio::set_volume();
        };

        /*!
         * \brief Process audio manager messages,
         */
        inline void audio_process_messages(const message_container& messages) {
            mgr::audio::process_messages(messages);
        };

        /* ********************************** */
        /* *** ASSETS INTERFACE FUNCTIONS *** */
        /* ********************************** */
        /*!
         * \brief Backup all temp bitmaps.
         */
        inline void bitmap_backup(void) {
            mgr::assets::backup();
        };

        /*!
         * \brief Reload all temp bitmaps.
         */
        inline void bitmap_reload(void) {
            mgr::assets::reload();
        };

        /* ******************************** */
        /* *** MENU INTERFACE FUNCTIONS *** */
        /* ******************************** */
        /*!
         * \brief Initialize menu manager,
         */
        inline void menus_init(void) {
            mgr::menus::initialize();
        };

        /*!
         * \brief De-initialize menu manager,
         */
        inline void menus_de_init(void) {
            mgr::menus::de_init();
        };

        /*!
         * \brief Run menus,
         */
        inline void menus_run(void) {
            mgr::menus::run();
        };

        /* ************************************ */
        /* *** MESSAGES INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         * \brief Load a file into the message manager.
         */
        inline void messages_load_file(const std::string& fname) {
            mgr::messages::load_file(fname);
        };

        /*!
         * \brief Get messages based on their system/manager.
         */
        inline const message_container messages_get(const std::string& arg) {
            return mgr::messages::get_messages(arg);
        };

        /*!
         * \brief Prune unprocessed timed messages.
         */
        inline void messages_prune(void) {
            mgr::messages::prune();
        };

        /* ************************************ */
        /* *** RENDERER INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         * \brief Initialize renderer.
         */
        inline void renderer_init(void) {
            mgr::renderer::initialize();
        };

        /*!
         * \brief De-initialize renderer.
         */
        inline void renderer_de_init(void) {
            mgr::renderer::de_init();
        };

        /*!
         * \brief Run renderer.
         */
        inline void renderer_run(void) {
            mgr::renderer::render();
        };

        /* *********************************** */
        /* *** SPAWNER INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         * \brief Process messages for the spawner.
         */
        inline void spawner_process_messages(const message_container& messages) {
            mgr::spawner::process_messages(messages);
        };

        /* *********************************** */
        /* *** SYSTEMS INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         * \brief Clear systems.
         */
        inline void systems_clear(void) {
            mgr::systems::clear();
        };

        /*!
         * \brief Finalize system manager.
         */
        inline void systems_finalize(void) {
            mgr::systems::finalize();
        };

        /*!
         * \brief Empty the system manager for loading.
         */
        inline const bool systems_empty(void) {
            return mgr::systems::empty();
        };

        /*!
         * \brief Run the system manager.
         */
        inline void systems_run(void) {
            mgr::systems::run();
        };

        /*!
         * \brief Dispatch messages for systems.
         */
        inline void systems_dispatch(void) {
            mgr::systems::dispatch();
        };
};

template <> inline bool interface::manager<interface>::initialized = false;

} //  end namespace mgr

} //  end namespace wte

#endif
