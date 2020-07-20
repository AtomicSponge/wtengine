/*!
 * \brief WTEngine | File:  menu_item_action.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Action menu item.
 */

#ifndef WTE_MNU_MENU_ITEM_ACTION_HPP
#define WTE_MNU_MENU_ITEM_ACTION_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

/*!
 * \class menu_item_action
 * \brief Menu item that runs a message
 */
class menu_item_action final : public menu_item {
    public:
        /*!
         * \brief Menu item action constructor.
         * Call this to define a message with arguments.
         * \param l Label for item.
         * \param c Command to run for item.
         * \param a Arguments for command.
         */
        inline menu_item_action(const std::string l, const std::string c, const std::string a) :
            menu_item(l), cmd(c), arg(a) {};

        /*!
         * \brief Menu item action constructor.
         * Call this to define a message with no arguments.
         * \param l Label for item.
         * \param c Command to run for item.
         */
        inline menu_item_action(const std::string l, const std::string c) :
            menu_item(l), cmd(c), arg("") {};

        /*!
         * Menu item action destructor.
         */
        inline ~menu_item_action() {};

        /*!
         * \brief Menu item action on selection.
         * Return the stored command when selected.
         */
        inline const message on_select(void) override { return message("system", cmd, arg); };

        /*!
         * \brief Return display text for the menu item when rendering.
         * Displays the action item's label.
         * \return Vector of display text strings.
         */
        inline const std::vector<std::string> get_text(void) const override { return { get_label() }; };

    private:
        std::string cmd;
        std::string arg;
};

}  // end namespace mnu

}  // end namespace wte

#endif
