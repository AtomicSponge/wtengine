/*!
 * \brief WTEngine | File:  menu_item.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Menu item interface class.
 */

#ifndef WTE_MENU_MENU_ITEM_HPP
#define WTE_MENU_MENU_ITEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "../message.hpp"

namespace wte
{

namespace mnu
{

/*!
 *
 */
class menu_item {
    public:
        /*!
         * Menu item destructor.
         * \param void
         * \return void
         */
        inline virtual ~menu_item() {};

        /*!
         * Return display text for the menu item when rendering.
         * \param void
         * \return Vector of display text.
         */
        inline virtual const std::vector<std::string> get_text(void) { return {"null"}; };

        /*!
         * Define left click process.
         * \param alt_trigger Is menu option key down.
         * \return void
         */
        inline virtual void on_left(bool alt_trigger) {};

        /*!
         * Define right click process.
         * \param alt_trigger Is menu option key down.
         * \return void
         */
        inline virtual void on_right(bool alt_trigger) {};

        /*!
         * Define selection process.
         * \param void
         * \return void
         */
        inline virtual const message on_select(void) { return message("system", "null", "null"); };

        /*!
         * Define reset process.
         * \param void
         * \return void
         */
        inline virtual void reset_to_default(void) {};

        /*!
         * Define default value process.
         * \param void
         * \return void
         */
        inline virtual void set_default(void) {};

        /*!
         * Get the display label.
         * \param void
         * \return The label text.
         */
        inline const std::string get_label(void) const { return label; };

        /*!
         *
         */
        inline const std::size_t get_var_type(void) const { return variable_type; };

    private:
        std::string label;
        std::size_t variable_type;

    protected:
        /*!
         * Menu item constructor.
         * \param l Label of the menu item.
         * \return void
         */
        inline menu_item(const std::string l) :
        label(l) {};
};

//! Menu Item shared pointer
typedef std::shared_ptr<menu_item> menu_item_sptr;

}  // end namespace mnu

}  // end namespace wte

#endif
