/*!
 * \brief WTEngine | File:  menu_item.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details A menu item.
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

//!
/*!
*/
class menu_item {
    public:
        inline virtual ~menu_item() {};

        inline virtual std::vector<std::string> get_text(void) { return {"null"}; };

        inline virtual void on_left(void) {};
        inline virtual void on_right(void) {};

        inline virtual message on_select(void) { return message("system", "null", "null"); };

        inline const std::string get_label(void) const { return label; };

    private:
        std::string label;

    protected:
        inline menu_item(const std::string l) : label(l) {};
};

//! Menu Item shared pointer
typedef std::shared_ptr<menu_item> menu_item_sptr;

}  // end namespace mnu

}  // end namespace wte

#endif
