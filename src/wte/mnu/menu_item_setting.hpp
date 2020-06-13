/*!
 * \brief WTEngine | File:  menu_item_setting.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Setting menu item.
 */

#ifndef WTE_MENU_MENU_ITEM_SETTING_HPP
#define WTE_MENU_MENU_ITEM_SETTING_HPP

#include <string>
#include <vector>
#include <algorithm>

#include "menu_item.hpp"

#include "../engine_cfg_map.hpp"
#include "../game_cfg_map.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
*/
class menu_item_setting final : public menu_item {
    public:
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_setting(const std::string l,
                                 const std::string vr,
                                 const std::vector<std::string> vl,
                                 const bool is_engine_set) :
        menu_item(l), var(vr), vals(vl), is_eng_setting(is_engine_set) {
            current_val = vals.begin();
            default_val = current_val;
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline ~menu_item_setting() {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_left(void) override { if(current_val != vals.begin()) current_val--; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_right(void) override {
            if(current_val != vals.end()) current_val++;
            if(current_val == vals.end()) current_val--;
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::vector<std::string> get_text(void) override { return { get_label(), "< " + *current_val + " >" }; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { current_val = default_val; };

        /*!
         * Description
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
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_setting(void) { return var + "=" + *current_val; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const bool is_engine_setting(void) { return is_eng_setting; }

    private:
        bool is_eng_setting;
        std::string var;
        std::vector<std::string> vals;
        std::vector<std::string>::const_iterator current_val;
        std::vector<std::string>::const_iterator default_val;
};

/*!
 * \def wte_menu_setting(...)
 * Create a new menu setting item.
 */
#define wte_menu_setting(...) (std::make_shared<mnu::menu_item_setting>(__VA_ARGS__))

}  // end namespace mnu

}  // end namespace wte

#endif
