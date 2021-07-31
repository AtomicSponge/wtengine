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

/*
 * Allows the engine class to access private members of managers.
 * These functions are not needed for implementation.
 */
class engine_inf final : private manager<engine_inf>  {
    public:
        inline engine_inf() {};
        inline ~engine_inf() {};

        /* ********************************* */
        /* *** AUDIO INTERFACE FUNCTIONS *** */
        /* ********************************* */
        inline void audio_init(void) { mgr::audio::initialize(); };
        inline void audio_de_init(void) { mgr::audio::de_init(); };
        inline void audio_set_volume(void) { mgr::audio::set_volume(); };
        inline void audio_process_messages(const message_container& messages) { mgr::audio::process_messages(messages); };
        /* ********************************** */
        /* *** ASSETS INTERFACE FUNCTIONS *** */
        /* ********************************** */
        inline void bitmap_backup(void) { mgr::assets::backup_bitmaps(); };
        inline void bitmap_reload(void) { mgr::assets::reload_bitmaps(); };
        /* ******************************** */
        /* *** MENU INTERFACE FUNCTIONS *** */
        /* ******************************** */
        inline void menus_init(void) { mgr::menus::initialize(); };
        inline void menus_de_init(void) { mgr::menus::de_init(); };
        inline void menus_run(void) { mgr::menus::run(); };
        inline void menus_reset(void) { mgr::menus::reset(); }
        inline void menus_open(const std::string& menu_id) { mgr::menus::open_menu(menu_id); }
        inline void menus_close(void) { mgr::menus::close_menu(); }
        /* ************************************ */
        /* *** MESSAGES INTERFACE FUNCTIONS *** */
        /* ************************************ */
        inline void messages_load_file(const std::string& fname) { mgr::messages::load_file(fname); };
        inline const message_container messages_get(const std::string& arg) { return mgr::messages::get_messages(arg); };
        inline void messages_prune(void) { mgr::messages::prune(); };
        /* ************************************ */
        /* *** RENDERER INTERFACE FUNCTIONS *** */
        /* ************************************ */
        inline void renderer_init(void) { mgr::renderer::initialize(); };
        inline void renderer_de_init(void) { mgr::renderer::de_init(); };
        inline void renderer_run(void) { mgr::renderer::render(); };
        /* *********************************** */
        /* *** SPAWNER INTERFACE FUNCTIONS *** */
        /* *********************************** */
        inline void spawner_process_messages(const message_container& messages) { mgr::spawner::process_messages(messages); };
        /* *********************************** */
        /* *** SYSTEMS INTERFACE FUNCTIONS *** */
        /* *********************************** */
        inline void systems_clear(void) { mgr::systems::clear(); };
        inline void systems_finalize(void) { mgr::systems::finalize(); };
        inline const bool systems_empty(void) { return mgr::systems::empty(); };
        inline void systems_run(void) { mgr::systems::run(); };
        inline void systems_dispatch(void) { mgr::systems::dispatch(); };
};

template <> inline bool engine_inf::manager<engine_inf>::initialized = false;

} //  end namespace mgr

} //  end namespace wte

#endif
