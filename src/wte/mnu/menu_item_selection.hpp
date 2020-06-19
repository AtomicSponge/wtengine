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

#include "menu_item.hpp"

#include "../_globals/engine_cfg_map.hpp"
#include "../_globals/game_cfg_map.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
*/
class menu_item_selection final : public menu_item {
    public:
        /*!
         * Menu item selection constructor.
         * \param l Item display label.
         * \param vr Variable to adjust.
         * \param vl Vector of allowed settings.
         * \param is_engine_set True if an engine setting, false if a game setting.
         * \return void
         */
        inline menu_item_selection(const std::string l,
                                   const std::string vr,
                                   const std::vector<std::string> vl,
                                   const bool is_engine_set) :
        menu_item(l), var(vr), vals(vl), is_eng_setting(is_engine_set) {
            current_val = vals.begin();
            default_val = current_val;
        };

        /*!
         * Menu item selection destructor.
         * \param void
         * \return void
         */
        inline ~menu_item_selection() {};

        /*!
         * Define left click process.
         * \param alt_trigger Is menu option key down.
         * \return void
         */
        inline void on_left(bool alt_trigger) override {
            if(current_val != vals.begin()) current_val--;
        };

        /*!
         * Define right click process.
         * \param alt_trigger Is menu option key down.
         * \return void
         */
        inline void on_right(bool alt_trigger) override {
            if(current_val != vals.end()) current_val++;
            if(current_val == vals.end()) current_val--;
        };

        /*!
         * Return display text for the menu item when rendering.
         * \param void
         * \return Vector of display text.
         */
        inline const std::vector<std::string> get_text(void) override {
            return { get_label(), "< " + *current_val + " >" };
        };

        /*!
         * Reset to the default variable.
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { current_val = default_val; };

        /*!
         * Set the default variable.
         * \param void
         * \return void
         */
        inline void set_default(void) override {
            if(is_eng_setting) current_val = std::find(std::begin(vals), std::end(vals), engine_cfg::get(var));
            else current_val = std::find(std::begin(vals), std::end(vals), game_cfg::get(var));
            if(current_val == vals.end()) current_val = vals.begin();
            default_val = current_val;
        };

        /*!
         * Get the setting to be applied by the system.
         * \param void
         * \return A string in format var=val
         */
        inline const std::string get_setting(void) { return var + "=" + *current_val; };

        /*!
         * Check if it is an engine setting.
         * \param void
         * \return True if an engine setting, false if a game setting.
         */
        inline const bool is_engine_setting(void) { return is_eng_setting; }

    private:
        bool is_eng_setting;
        std::string var;
        std::vector<std::string> vals;
        std::vector<std::string>::const_iterator current_val;
        std::vector<std::string>::const_iterator default_val;
};

}  // end namespace mnu

}  // end namespace wte

#endif
