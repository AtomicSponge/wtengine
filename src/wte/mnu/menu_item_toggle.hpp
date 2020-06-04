/*!
 * \brief WTEngine | File:  menu_item_toggle.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Toggle menu item.
 */

#ifndef WTE_MENU_MENU_ITEM_TOGGLE_HPP
#define WTE_MENU_MENU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>
#include <functional>

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
         * Description
         * \param void
         * \return void
         */
        inline menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff,
                                bool func(void)) :
        menu_item(label), cmd_on(con), arg_on(aon), cmd_off(coff), arg_off(aoff), defaulter(func) {
            toggled = default_val = defaulter();
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline ~menu_item_toggle() {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_left(void) override { toggled = true; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_right(void) override { toggled = false; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::vector<std::string> get_text(void) override {
            if(toggled) return { get_label(), "< On >", "Off" };
            return { get_label(), "On", "< Off >" };
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_active_cmd(void) {
            if(toggled) return cmd_on;
            return cmd_off;
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_active_args(void) {
            if(toggled) return arg_on;
            return arg_off;
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { toggled = default_val; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void set_default(void) override { toggled = default_val = defaulter(); };

    private:
        bool toggled, default_val;
        std::string cmd_on;
        std::string cmd_off;
        std::string arg_on;
        std::string arg_off;

        std::function<bool(void)> defaulter;
};

}  // end namespace mnu

}  // end namespace wte

#endif
