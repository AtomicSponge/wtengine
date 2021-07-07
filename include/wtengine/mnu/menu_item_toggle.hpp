/*!
 * WTEngine | File:  menu_item_toggle.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_ITEM_TOGGLE_HPP
#define WTE_MNU_MENU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>
#include <functional>

#include "wtengine/mnu/menu_item.hpp"

namespace wte
{

namespace mnu
{

/*!
 * \class menu_item_toggle
 * \brief On/off menu item.
 */
class menu_item_toggle final : public menu_item {
    public:
        /*!
         * \brief Create a menu toggle item.  Uses default setting type.
         * 
         * \param label Item display label.
         * \param con Command to run when toggled on.
         * \param aon Arguments to use when toggled on.
         * \param coff Command to run when toggled off.
         * \param aoff Arguments to use when toggled off.
         * \param func Fuction to determine defaut setting.
         */
        menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff,
                                bool func(void));

        /*!
         * \brief Create a menu toggle item.
         * 
         * \param label Item display label.
         * \param con Command to run when toggled on.
         * \param aon Arguments to use when toggled on.
         * \param coff Command to run when toggled off.
         * \param aoff Arguments to use when toggled off.
         * \param st Setting type.
         * \param func Fuction to determine defaut setting.
         */
        menu_item_toggle(const std::string label,
                                const std::string con,
                                const std::string aon,
                                const std::string coff,
                                const std::string aoff,
                                const std::size_t st,
                                bool func(void));

        /*!
         * \brief Menu toggle item destructor.
         */
        ~menu_item_toggle();

        /*!
         * \brief On left process.
         * 
         * Toggles on.
         */
        void on_left(void) override;

        /*!
         * \brief On right process.
         * 
         * Toggles off.
         */
        void on_right(void) override;

        /*!
         * \brief Get display text
         * 
         * \return Vector of strings to render.
         */
        const std::vector<std::string> get_text(void) const override;

        /*!
         * \brief Get the active command.
         * 
         * Return the command depending if the setting is toggled on or off.
         * 
         * \return Active command string.
         */
        const std::string get_active_cmd(void);

        /*!
         * \brief Get the active arguments.
         * 
         * Return arguments depending if the setting is toggled on or off.
         * 
         * \return Active arguments string.
         */
        const std::string get_active_args(void);

        /*!
         * \brief Reset to default value.
         * 
         * If the menu is canceled, return to proper setting.
         */
        void reset_to_default(void) override;

        /*!
         * \brief Set default value.
         * 
         * Determine and set the default value for the menu toggle item.
         */
        void set_default(void) override;

        /*!
         * \brief Setting changed process.
         * 
         * Checks if the current selection is the same as the default.
         * 
         * \return True if the setting changed, false if it did not.
         */
        const bool setting_changed(void) const override;

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
