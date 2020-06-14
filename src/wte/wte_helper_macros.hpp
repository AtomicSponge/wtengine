/*!
 * \brief WTEngine | File:  helper_macros.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Helper macros.
 * If using you must declare "#define _WTE_USE_HELPER_MACROS" and
 * "using namespace wte" in your source file.
 */

#ifndef WTE_HELPER_MACROS_HPP
#define WTE_HELPER_MACROS_HPP

/*!
 * \def wte_new_component(new_cmp, ...)
 * \brief Create a new component.
 * Pass a component object and any parameters to create a new component.
 */
#define wte_new_component(ent_id, new_cmp, ...) (world.add_component(ent_id, std::make_shared<new_cmp>(__VA_ARGS__)))

/*!
 * \def wte_add_system(new_sys, ...)
 * \brief Create a new system.
 * Pass a system object and any parameters to create a new system.
 */
#define wte_add_system(new_sys, ...) (systems.add(std::make_unique<new_sys>(__VA_ARGS__)))

/*!
 * \def wte_set_component(the_cmp, ent_id)
 * \brief Set new value for a component.
 * Pass a component object and entity ID.
 */
#define wte_set_component(the_cmp, ent_id) (world.set_component<the_cmp>(ent_id))

/*!
 * \def wte_get_component(the_cmp, ent_id)
 * \brief Get current value for a component.
 * Pass a component object and entity ID.
 */
#define wte_get_component(the_cmp, ent_id) (world.get_component<the_cmp>(ent_id))

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
 * \def wte_menu_setting(...)
 * Create a new menu setting item.
 */
#define wte_menu_setting(...) (std::make_shared<mnu::menu_item_setting>(__VA_ARGS__))

/*!
 * \def wte_menu_toggle(...)
 * Create a new menu toggle item.
 */
#define wte_menu_toggle(...) (std::make_shared<mnu::menu_item_toggle>(__VA_ARGS__))

#endif
