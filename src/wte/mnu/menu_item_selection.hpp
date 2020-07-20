/*!
 * \brief WTEngine | File:  menu_item_selection.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Selection menu item.
 */

#ifndef WTE_MNU_MENU_ITEM_SELECTION_HPP
#define WTE_MNU_MENU_ITEM_SELECTION_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "menu_item.hpp"

#include "../_globals/engine_cfg.hpp"
#include "../_globals/game_cfg.hpp"

namespace wte
{

namespace mnu
{

/*!
 * \class menu_item_selection
 * \brief Create a new selection menu item.
 */
class menu_item_selection final : public menu_item {
    public:
        /*!
         * Menu item selection constructor.
         * \param label Item display label.
         * \param vr Variable to adjust.
         * \param vl Vector of allowed settings.
         * \param st Type of setting.
         */
        inline menu_item_selection(const std::string label,
                                   const std::string vr,
                                   const std::vector<std::string> dvl,
                                   const std::vector<std::string> vl,
                                   const std::size_t st) :
        menu_item(label, st), var(vr), display_vals(dvl), vals(vl) {
            if(dvl.size() != vl.size())
                throw std::runtime_error("Selection item label and value counts must be the same!");
            current_val = vals.begin();
            default_val = current_val;
        };

        /*!
         * Menu item selection destructor.
         */
        inline ~menu_item_selection() {};

        /*!
         * Define left click process.
         * \param alt_trigger Alt trigger flag (unused).
         */
        inline void on_left(void) override {
            if(current_val != vals.begin()) current_val--;
        };

        /*!
         * Define right click process.
         * \param alt_trigger Alt trigger flag (unused).
         */
        inline void on_right(void) override {
            if(current_val != vals.end()) current_val++;
            if(current_val == vals.end()) current_val--;
        };

        /*!
         * Return display text for the menu item when rendering.
         * \return Vector of display text strings.
         */
        inline const std::vector<std::string> get_text(void) const override {
            return {
                get_label(), "< " + display_vals.at(std::distance(vals.cbegin(), current_val)) + " >"
            };
        };

        /*!
         * Reset to the default variable.
         */
        inline void reset_to_default(void) override { current_val = default_val; };

        /*!
         * Set the default variable.
         */
        inline void set_default(void) override {
            if(is_engine_setting()) current_val = std::find(std::begin(vals), std::end(vals), engine_cfg::get(var));
            else current_val = std::find(std::begin(vals), std::end(vals), game_cfg::get(var));
            if(current_val == vals.end()) current_val = vals.begin();
            default_val = current_val;
        };

        /*!
         * \brief Setting changed process.
         * Checks if the current selection is the same as the default.
         * \return True if the setting changed, false if it did not.
         */
        inline const bool setting_changed(void) const override {
            if(current_val == default_val) return false;
            return true;
        };

        /*!
         * Get the setting to be applied by the system.
         * \return A string in format var=val
         */
        inline const std::string get_setting(void) { return var + "=" + *current_val; };

    private:
        std::string var;
        std::vector<std::string> display_vals;
        std::vector<std::string> vals;
        std::vector<std::string>::const_iterator current_val;
        std::vector<std::string>::const_iterator default_val;
};

}  // end namespace mnu

}  // end namespace wte

#endif
