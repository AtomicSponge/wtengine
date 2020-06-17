/*!
 * \brief WTEngine | File:  menu_item_apply.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Setting menu item.
 */

#ifndef WTE_MNU_MENU_ITEM_APPLY_HPP
#define WTE_MNU_MENU_ITEM_APPLY_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!  Menu Item Apply class.
/*!
 * An apply option for the menus.
 */
class menu_item_apply final : public menu_item {
    public:
        /*!
         * Menu Item Apply constructor.
         * \param void
         * \return void
         */
        inline menu_item_apply() : menu_item("applier"), cmd("cancel") {};

        /*!
         * Menu Item Apply destructor.
         * \param void
         * \return void
         */
        inline ~menu_item_apply() {};

        /*!
         * \brief On left trigger.
         * Set the apply item to cancel.
         * \param void
         * \return void
         */
        inline void on_left(bool alt_trigger) override { cmd = "cancel"; };

        /*!
         * \brief On right trigger.
         * Set the apply item to apply.
         * \param void
         * \return void
         */
        inline void on_right(bool alt_trigger) override { cmd = "apply"; };

        /*!
         * \brief On select trigger.
         * Return the status of the apply item.
         * \param void
         * \return A message object.
         */
        inline const message on_select(void) override { return message("menu", cmd, ""); };

        /*!
         * \brief Get menu item text.
         * Return the display status of the apply item.
         * \param void
         * \return void
         */
        inline const std::vector<std::string> get_text(void) override {
            if(cmd == "apply") return { "Cancel", "< Apply >" };
            return { "< Cancel >", "Apply" };
        };

        /*!
         * \brief Reset to default trigger.
         * Reset the apply item to the cancel state.
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { cmd = "cancel"; };

        /*!
         * \brief Set to default trigger.
         * Reset the apply item to the cancel state.
         * \param void
         * \return void
         */
        inline void set_default(void) override { cmd = "cancel"; };

    private:
        std::string cmd;
};

}  // end namespace mnu

}  // end namespace wte

#endif
