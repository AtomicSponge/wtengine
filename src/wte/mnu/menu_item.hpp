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

/*!
 *
 */
class menu_item {
    public:
        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual ~menu_item() {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual const std::vector<std::string> get_text(void) { return {"null"}; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual void on_left(void) {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual void on_right(void) {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual const message on_select(void) { return message("system", "null", "null"); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual void reset_to_default(void) {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline virtual void set_default(void) {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_label(void) const { return label; };

    private:
        std::string label;

    protected:
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item(const std::string l) : label(l) {};
};

//! Menu Item shared pointer
typedef std::shared_ptr<menu_item> menu_item_sptr;

}  // end namespace mnu

}  // end namespace wte

#endif
