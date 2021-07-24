/*!
 * WTEngine | File:  interface.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/interface.hpp"

namespace wte
{

namespace mgr
{

template <> bool interface::manager<interface>::initialized = false;

interface::interface() {}
interface::~interface() {}

/*
 *
 */
void interface::audio_init(void) { mgr::audio::initialize(); }

/*
 *
 */
void interface::audio_de_init(void) { mgr::audio::de_init(); }

/*
 *
 */
void interface::audio_set_volume(void) { mgr::audio::set_volume(); }

/*
 *
 */
void interface::audio_process_messages(const message_container& messages) { mgr::audio::process_messages(messages); }

/*
 *
 */
void interface::bitmap_backup(void) { mgr::assets::backup_bitmaps(); }

/*
 *
 */
void interface::bitmap_reload(void) { mgr::assets::reload_bitmaps(); }

/*
 *
 */
void interface::menus_init(void) { mgr::menus::initialize(); }

/*
 *
 */
void interface::menus_de_init(void) { mgr::menus::de_init(); }

/*
 *
 */
void interface::menus_run(void) { mgr::menus::run(); }

/*
 *
 */
void interface::messages_load_file(const std::string& fname) { mgr::messages::load_file(fname); }

/*
 *
 */
const message_container interface::messages_get(const std::string& arg) { return mgr::messages::get_messages(arg); }

/*
 *
 */
void interface::messages_prune(void) { mgr::messages::prune(); }

/*
 *
 */
void interface::renderer_init(void) { mgr::renderer::initialize(); }

/*
 *
 */
void interface::renderer_de_init(void) { mgr::renderer::de_init(); }

/*
 *
 */
void interface::renderer_run(void) { mgr::renderer::render(); }

/*
 *
 */
void interface::spawner_process_messages(const message_container& messages) { mgr::spawner::process_messages(messages); }

/*
 *
 */
void interface::systems_clear(void) { mgr::systems::clear(); }

/*
 *
 */
void interface::systems_finalize(void) { mgr::systems::finalize(); }

/*
 *
 */
const bool interface::systems_empty(void) { return mgr::systems::empty(); }

/*
 *
 */
void interface::systems_run(void) { mgr::systems::run(); }

/*
 *
 */
void interface::systems_dispatch(void) { mgr::systems::dispatch(); }

} //  end namespace mgr

} //  end namespace wte
