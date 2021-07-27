/*!
 * WTEngine | File:  interface.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_INTERFACE_HPP
#define WTE_MGR_INTERFACE_HPP

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
class interface final : private manager<interface>  {
    public:
        interface();
        ~interface();

        /* ********************************* */
        /* *** AUDIO INTERFACE FUNCTIONS *** */
        /* ********************************* */
        /*!
         * \brief Initialize audio manager.
         */
        void audio_init(void);

        /*!
         * \brief De-initialize audio manager.
         */
        void audio_de_init(void);

        /*!
         * \brief Set audio manager volume.
         */
        void audio_set_volume(void);

        /*!
         * \brief Process audio manager messages.
         * \param messages Container of messages to process.
         */
        void audio_process_messages(const message_container& messages);

        /* ********************************** */
        /* *** ASSETS INTERFACE FUNCTIONS *** */
        /* ********************************** */
        /*!
         * \brief Backup all temp bitmaps.
         */
        void bitmap_backup(void);

        /*!
         * \brief Reload all temp bitmaps.
         */
        void bitmap_reload(void);

        /* ******************************** */
        /* *** MENU INTERFACE FUNCTIONS *** */
        /* ******************************** */
        /*!
         * \brief Initialize menu manager.
         */
        void menus_init(void);

        /*!
         * \brief De-initialize menu manager.
         */
        void menus_de_init(void);

        /*!
         * \brief Run menus.
         */
        void menus_run(void);

        /* ************************************ */
        /* *** MESSAGES INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         * \brief Load a file into the message manager.
         * \param fname Filename to load.
         */
        void messages_load_file(const std::string& fname);

        /*!
         * \brief Get messages based on their system/manager.
         * \param arg Name of system or manager.
         */
        const message_container messages_get(const std::string& arg);

        /*!
         * \brief Prune unprocessed timed messages.
         */
        void messages_prune(void);

        /* ************************************ */
        /* *** RENDERER INTERFACE FUNCTIONS *** */
        /* ************************************ */
        /*!
         * \brief Initialize renderer.
         */
        void renderer_init(void);

        /*!
         * \brief De-initialize renderer.
         */
        void renderer_de_init(void);

        /*!
         * \brief Run renderer.
         */
        void renderer_run(void);

        /* *********************************** */
        /* *** SPAWNER INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         * \brief Process messages for the spawner.
         * \param messages Container of messages to process.
         */
        void spawner_process_messages(const message_container& messages);

        /* *********************************** */
        /* *** SYSTEMS INTERFACE FUNCTIONS *** */
        /* *********************************** */
        /*!
         * \brief Clear systems.
         */
        void systems_clear(void);

        /*!
         * \brief Finalize system manager.
         */
        void systems_finalize(void);

        /*!
         * \brief Empty the system manager for loading.
         */
        const bool systems_empty(void);

        /*!
         * \brief Run the system manager.
         */
        void systems_run(void);

        /*!
         * \brief Dispatch messages for systems.
         */
        void systems_dispatch(void);
};

} //  end namespace mgr

} //  end namespace wte

#endif
