/*!
 * \brief WTEngine | File:  menu_item_apply.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Setting menu item.
 */

#ifndef WTE_MENU_MENU_ITEM_APPLY_HPP
#define WTE_MENU_MENU_ITEM_APPLY_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
 *
 */
class menu_item_apply final : public menu_item {
    public:
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_apply() : menu_item("applier"), cmd("cancel") {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline ~menu_item_apply() {};

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_left(void) override { cmd = "cancel"; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void on_right(void) override { cmd = "apply"; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const message on_select(void) override { return message("menu", cmd, ""); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::vector<std::string> get_text(void) override {
            if(cmd == "apply") return { "Cancel", "< Apply >" };
            return { "< Cancel >", "Apply" };
        };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void reset_to_default(void) override { cmd = "cancel"; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void set_default(void) override { cmd = "cancel"; };

    private:
        std::string cmd;
};

/*!
 * \def wte_menu_apply(...)
 * Create a new menu apply item.
 */
#define wte_menu_apply(...) (std::make_shared<mnu::menu_item_apply>(__VA_ARGS__))

}  // end namespace mnu

}  // end namespace wte

#endif
