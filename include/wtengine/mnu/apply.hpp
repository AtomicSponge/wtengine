/*!
 * WTEngine | File:  apply.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_APPLY_HPP
#define WTE_MNU_ITEM_APPLY_HPP

#include <string>
#include <vector>

#include "wtengine/mnu/item.hpp"

#include "wtengine/mgr/menus.hpp"
#include "wtengine/mgr/messages.hpp"

namespace wte::mnu {

/*!
 * \class apply
 * \brief An apply option for the menus.
 */
class apply final : public item {
    public:
        /*!
         * \brief Menu Item Apply constructor.
         */
        apply();

        ~apply() = default;  //!<  Default destructor.

    private:
        /*
         * Set the apply item to cancel.
         */
        void on_left(void) override;

        /*
         * Set the apply item to apply.
         */
        void on_right(void) override;

        /*
         * On select trigger.
         */
        void on_select(void) override;

        /*
         * Return display text for the menu item when rendering.
         */
        const std::vector<std::string> get_text(void) const override;

        /*
         * Reset the apply item to the canceled state.
         */
        void reset_to_default(void) override;

        /*
         * Set the apply item's default state to canceled.
         */
        void set_default(void) override;

        /*
         * Reset the apply item to the canceled state.
         */
        void apply_setting(void) override;

        bool do_apply;
};

}  //  end namespace wte::mnu

#endif
