/*!
 * WTEngine | File:  menu_item_apply.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MNU_MENU_ITEM_APPLY_HPP
#define WTE_MNU_MENU_ITEM_APPLY_HPP

#include <string>
#include <vector>

#include "wtengine/mnu/menu_item.hpp"

namespace wte
{

namespace mnu
{

/*!
 * \class menu_item_apply
 * \brief An apply option for the menus.
 */
class menu_item_apply final : public menu_item {
    public:
        /*!
         * \brief Menu Item Apply constructor.
         */
        menu_item_apply() : menu_item("applier"), cmd("cancel") {};

        /*!
         * \brief Menu Item Apply destructor.
         */
        ~menu_item_apply() {};

        /*!
         * \brief On left trigger.
         * 
         * Set the apply item to cancel.
         */
        void on_left(void) override { cmd = "cancel"; };

        /*!
         * \brief On right trigger.
         * 
         * Set the apply item to apply.
         */
        void on_right(void) override { cmd = "apply"; };

        /*!
         * \brief On select trigger.
         * 
         * Return the status of the apply item.
         * 
         * \return A menu message object that will either run the apply or cancel.
         */
        const message on_select(void) override { return message("menu", cmd, ""); };

        /*!
         * \brief Return display text for the menu item when rendering.
         * 
         * This is the display status of the apply item.
         * 
         * \return Vector of display text strings.
         */
        const std::vector<std::string> get_text(void) const override {
            if(cmd == "apply") return { "Cancel", "< Apply >" };
            return { "< Cancel >", "Apply" };
        };

        /*!
         * \brief Reset to default trigger.
         * 
         * Reset the apply item to the cancel state.
         */
        void reset_to_default(void) override { cmd = "cancel"; };

        /*!
         * \brief Set to default trigger.
         * 
         * Set the apply item's default state to cancel.
         */
        void set_default(void) override { cmd = "cancel"; };

    private:
        std::string cmd;
};

}  // end namespace mnu

}  // end namespace wte

#endif
