/*!
 * WTEngine | File:  menu_item.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_ITEM_HPP
#define WTE_MNU_MENU_ITEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "wtengine/message.hpp"

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
         * \brief Menu item destructor.
         */
        virtual ~menu_item() {};

        /*!
         * \brief Return display text for the menu item when rendering.
         * 
         * \return Vector of display text.
         */
        virtual const std::vector<std::string> get_text(void) const { return {"null"}; };

        /*!
         * \brief Define left click process.
         */
        virtual void on_left(void) {};

        /*!
         * \brief Define right click process.
         */
        virtual void on_right(void) {};

        /*!
         * \brief Define selection process.
         */
        virtual const message on_select(void) { return message("system", "null", "null"); };

        /*!
         * \brief Define reset process.
         */
        virtual void reset_to_default(void) {};

        /*!
         * \brief Define default value process.
         */
        virtual void set_default(void) {};

        /*!
         * \brief Define setting changed process.
         * 
         * \return Default return false.
         */
        virtual const bool setting_changed(void) const { return false; };

        /*!
         * \brief Get the display label.
         * 
         * \return The label text.
         */
        const std::string get_label(void) const { return label; };

        /*!
         * \brief Get the setting type.
         * 
         * \return Setting type.
         */
        const std::size_t get_setting_type(void) const { return setting_type; };

        /*!
         * \brief Check if it is an engine setting.
         * 
         * \return True if an engine setting, false if a game setting.
         */
        const bool is_engine_setting(void) const { 
            if(setting_type == ENGINE_SETTING ||
               setting_type == ENGINE_SETTING_RECONF ||
               setting_type == AUDIO_SETTING)
                return true;
            return false;
        };

    protected:
        /*!
         * \brief Menu item constructor.
         * 
         * \param l Label of the menu item.
         */
        menu_item(const std::string l) :
        label(l), setting_type(DEFAULT_SETTING) {};

        /*!
         * \brief Menu item constructor.
         * 
         * \param l Label of the menu item.
         * \param st Setting type.
         */
        menu_item(const std::string l, const std::size_t st) :
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
