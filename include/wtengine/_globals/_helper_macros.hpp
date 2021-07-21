/*!
 * WTEngine | File:  _helper_macros.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_HELPER_MACROS_HPP
#define WTE_HELPER_MACROS_HPP

/*  NOTE:  Will remove after menus are done */

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
