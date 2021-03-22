/*!
 * WTEngine | File:  wte_helper_macros.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_HELPER_MACROS_HPP
#define WTE_HELPER_MACROS_HPP

/*********************************
 * Entity items                  *
 *********************************/

/*!
 * \def wte_new_component(new_cmp, ...)
 * \brief Create a new component.
 * Pass a component object and any parameters to create a new component.
 */
#define wte_new_component(ent_id, new_cmp, ...) (world.add_component(ent_id, std::make_shared<new_cmp>(__VA_ARGS__)))

/*!
 * \def wte_set_component(ent_id, the_cmp)
 * \brief Set new value for a component.
 * Pass a component object and entity ID.
 */
#define wte_set_component(ent_id, the_cmp) (world.set_component<the_cmp>(ent_id))

/*!
 * \def wte_get_component(ent_id, the_cmp)
 * \brief Get current value for a component.
 * Pass a component object and entity ID.
 */
#define wte_get_component(ent_id, the_cmp) (world.get_component<the_cmp>(ent_id))

/*********************************
 * Menu items                    *
 *********************************/

/*!
 * \def wte_menu_action(...)
 * Create a new menu action item.
 */
#define wte_menu_action(...) (std::make_shared<mnu::menu_item_action>(__VA_ARGS__))

/*!
 * \def wte_menu_apply(...)
 * Create a new menu apply item.
 */
#define wte_menu_apply(...) (std::make_shared<mnu::menu_item_apply>(__VA_ARGS__))

/*!
 * \def wte_menu_selection(...)
 * Create a new menu setting item.
 */
#define wte_menu_selection(...) (std::make_shared<mnu::menu_item_selection>(__VA_ARGS__))

/*!
 * \def wte_menu_toggle(...)
 * Create a new menu toggle item.
 */
#define wte_menu_toggle(...) (std::make_shared<mnu::menu_item_toggle>(__VA_ARGS__))

/*********************************
 * Other items                   *
 *********************************/

/*!
 * \def wte_load_sample(samp_str)
 * Load a sample into the audio manager.
 */
#define wte_load_sample(samp_str) (messages.add_message(message("audio", "load_sample", samp_str)))

/*!
 * \def wte_add_system(new_sys, ...)
 * \brief Create a new system.
 * Pass a system object and any parameters to create a new system.
 */
#define wte_add_system(new_sys, ...) (systems.add(std::make_unique<new_sys>(__VA_ARGS__)))

#endif
