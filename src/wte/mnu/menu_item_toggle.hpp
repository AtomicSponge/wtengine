/*!
 * \brief WTEngine | File:  menu_item_toggle.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Toggle menu item.  WIP
 */

#ifndef WTE_MENU_MENU_ITEM_TOGGLE_HPP
#define WTE_MENU_MENU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
 *
 */
class menu_item_toggle final : public menu_item {
    public:
        /*!
         *
         */
        inline menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff) :
        menu_item(label), cmd_on(con), arg_on(aon), cmd_off(coff), arg_off(aoff) {};

        /*!
         *
         */
        inline ~menu_item_toggle() {};

        /*!
         *
         */
        inline void on_left(void) override { toggled = true; };

        /*!
         *
         */
        inline void on_right(void) override { toggled = false; };

        /*!
         *
         */
        inline std::vector<std::string> get_text(void) override {
            if(toggled) return { get_label(), "< On >", "Off" };
            return { get_label(), "On", "< Off >" };
        };

        /*!
         *
         */
        inline std::string get_active_cmd(void) {
            if(toggled) return cmd_on;
            return cmd_off;
        };

        /*!
         *
         */
        inline std::string get_active_args(void) {
            if(toggled) return arg_on;
            return arg_off;
        };

        /*!
         *
         */
        inline void reset_to_default(void) {};

        /*!
         *
         */
        inline void set_default(void) {};

    private:
        bool toggled;
        std::string cmd_on;
        std::string cmd_off;
        std::string arg_on;
        std::string arg_off;
};

}  // end namespace mnu

}  // end namespace wte

#endif
