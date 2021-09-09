/*!
 * WTEngine | File:  menu_item.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_ITEM_HPP
#define WTE_MNU_MENU_ITEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "wtengine/_globals/message.hpp"

namespace wte::mnu {

/*!
 * \class menu_item
 * \brief Menu item interface class.
 */
class menu_item {
    public:
        inline virtual ~menu_item() = default;    //!<  Default virtual destructor.

        /*!
         * \brief Return display text for the menu item when rendering.
         * \return Vector of display text.
         */
        inline virtual const std::vector<std::string> get_text(void) const { return {"null"}; };

        /*!
         * \brief Define left click process.
         */
        inline virtual void on_left(void) {};

        /*!
         * \brief Define right click process.
         */
        inline virtual void on_right(void) {};

        /*!
         * \brief Define selection process.
         */
        inline virtual void on_select(void) {};

        /*!
         * \brief Define reset process.
         */
        inline virtual void reset_to_default(void) {};

        /*!
         * \brief Define default value process.
         */
        inline virtual void set_default(void) {};

        /*!
         * \brief Define setting changed process.
         * \return Default return false.
         */
        inline virtual const bool setting_changed(void) const { return false; };

        /*!
         * \brief Get the display label.
         * \return The label text.
         */
        inline const std::string get_label(void) const { return label; };

    protected:
        /*!
         * \brief Menu item constructor.
         * \param l Label of the menu item.
         */
        inline menu_item(
            const std::string& l
        ) : label(l) {};

    private:
        std::string label;
};

//! Menu Item shared pointer
typedef std::shared_ptr<menu_item> menu_item_sptr;

}  //  end namespace wte::mnu

#endif
