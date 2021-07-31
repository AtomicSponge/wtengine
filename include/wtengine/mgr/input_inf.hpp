/*!
 * WTEngine | File:  input_inf.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_INPUT_INF_HPP
#define WTE_MGR_INPUT_INF_HPP

#include "wtengine/mgr/menus.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class input_inf
 * \brief Allows the input subsystem to access private members of the menu manager.
 *
 * These functions are not needed for implementation
 * and are only here as a reference.
 */
class input_inf final : private manager<input_inf>  {
    public:
        inline input_inf() {};
        inline ~input_inf() {};

        inline void menu_up(void) { mgr::menus::menu_pos_up(); };
        inline void menu_down(void) { mgr::menus::menu_pos_down(); };
        inline void menu_start_left(void) { mgr::menus::menu_pos_start_left(); };
        inline void menu_stop_left(void) { mgr::menus::menu_pos_stop_left(); };
        inline void menu_start_right(void) { mgr::menus::menu_pos_start_right(); };
        inline void menu_stop_right(void) { mgr::menus::menu_pos_stop_right(); };
        inline void menu_select(void) { mgr::menus::menu_item_select(); };
        inline void menu_close(void) { mgr::menus::close_menu(); };
};

template <> inline bool input_inf::manager<input_inf>::initialized = false;

} //  end namespace mgr

} //  end namespace wte

#endif
