/*!
 * WTEngine | File:  action.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_ACTION_HPP
#define WTE_MNU_ITEM_ACTION_HPP

#include <string>
#include <vector>

#include "wtengine/mnu/item.hpp"

#include "wtengine/mgr/messages.hpp"

namespace wte::mnu {

/*!
 * \class action
 * \brief Menu item that runs a message
 */
class action final : public item {
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
        action(
            const std::string& l,
            const std::string& c,
            const std::string& a
        );

        /*!
         * \brief Menu item action constructor.
         * 
         * Call this to define a message with no arguments.
         * 
         * \param l Label for item.
         * \param c Command to run for item.
         */
        action(
            const std::string& l,
            const std::string& c
        );

        ~action() = default;  //!<  Default destructor.

    private:
        /*
         * Menu item action on selection.
         */
        void on_select(void) override;

        /*
         * Return display text for the menu item when rendering.
         */
        const std::vector<std::string> get_text(void) const override;

        const std::string cmd;
        const std::string arg;
};

}  //  end namespace wte::mnu

#endif
