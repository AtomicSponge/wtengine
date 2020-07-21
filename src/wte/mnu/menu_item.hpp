/*!
 * WTEngine | File:  menu_item.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MNU_MENU_ITEM_HPP
#define WTE_MNU_MENU_ITEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "../message.hpp"

namespace wte
{

namespace mnu
{

enum MENU_SETTING_TYPE {
    GAME_SETTING,       AUDIO_SETTING,
    ENGINE_SETTING,     ENGINE_SETTING_RECONF,
    DEFAULT_SETTING
};

/*!
 * \class menu_item
 * \brief Menu item interface class.
 */
class menu_item {
    public:
        /*!
         * Menu item destructor.
         */
        inline virtual ~menu_item() {};

        /*!
         * Return display text for the menu item when rendering.
         * \return Vector of display text.
         */
        inline virtual const std::vector<std::string> get_text(void) const { return {"null"}; };

        /*!
         * Define left click process.
         */
        inline virtual void on_left(void) {};

        /*!
         * Define right click process.
         */
        inline virtual void on_right(void) {};

        /*!
         * Define selection process.
         */
        inline virtual const message on_select(void) { return message("system", "null", "null"); };

        /*!
         * Define reset process.
         */
        inline virtual void reset_to_default(void) {};

        /*!
         * Define default value process.
         */
        inline virtual void set_default(void) {};

        /*!
         * Define setting changed process.
         * \return Default return false.
         */
        inline virtual const bool setting_changed(void) const { return false; };

        /*!
         * Get the display label.
         * \return The label text.
         */
        inline const std::string get_label(void) const { return label; };

        /*!
         * Get the setting type.
         * \return Setting type.
         */
        inline const std::size_t get_setting_type(void) const { return setting_type; };

        /*!
         * Check if it is an engine setting.
         * \return True if an engine setting, false if a game setting.
         */
        inline const bool is_engine_setting(void) const { 
            if(setting_type == ENGINE_SETTING ||
               setting_type == ENGINE_SETTING_RECONF ||
               setting_type == AUDIO_SETTING)
                return true;
            return false;
        };

    protected:
        /*!
         * Menu item constructor.
         * \param l Label of the menu item.
         */
        inline menu_item(const std::string l) :
        label(l), setting_type(DEFAULT_SETTING) {};

        /*!
         * Menu item constructor.
         * \param l Label of the menu item.
         * \param st Setting type.
         */
        inline menu_item(const std::string l, const std::size_t st) :
        label(l), setting_type(st) {};

    private:
        std::string label;
        std::size_t setting_type;
};

//! Menu Item shared pointer
typedef std::shared_ptr<menu_item> menu_item_sptr;

}  // end namespace mnu

}  // end namespace wte

#endif
