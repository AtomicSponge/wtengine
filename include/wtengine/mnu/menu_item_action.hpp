/*!
 * WTEngine | File:  menu_item_action.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_ITEM_ACTION_HPP
#define WTE_MNU_MENU_ITEM_ACTION_HPP

#include <string>
#include <vector>

#include "wtengine/mnu/menu_item.hpp"

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
         * 
         * Call this to define a message with arguments.
         * 
         * \param l Label for item.
         * \param c Command to run for item.
         * \param a Arguments for command.
         */
        menu_item_action(const std::string l, const std::string c, const std::string a);

        /*!
         * \brief Menu item action constructor.
         * 
         * Call this to define a message with no arguments.
         * 
         * \param l Label for item.
         * \param c Command to run for item.
         */
        menu_item_action(const std::string l, const std::string c);

        /*!
         * \brief Menu item action destructor.
         */
        ~menu_item_action();

        /*!
         * \brief Menu item action on selection.
         * 
         * Return the stored command when selected.
         */
        const message on_select(void) override;

        /*!
         * \brief Return display text for the menu item when rendering.
         * 
         * Displays the action item's label.
         * 
         * \return Vector of display text strings.
         */
        const std::vector<std::string> get_text(void) const override;

    private:
        std::string cmd;
        std::string arg;
};

}  // end namespace mnu

}  // end namespace wte

#endif
