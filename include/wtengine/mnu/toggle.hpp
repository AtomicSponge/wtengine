/*!
 * WTEngine | File:  toggle.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_TOGGLE_HPP
#define WTE_MNU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>
#include <functional>

#include "wtengine/mnu/item.hpp"

namespace wte::mnu {

/*!
 * \class toggle
 * \brief On/off menu item.
 */
class toggle final : public item {
    public:
        /*!
         * \brief Create a menu toggle item.  Uses default setting type.
         * \param label Item display label.
         * \param con Command to run when toggled on.
         * \param aon Arguments to use when toggled on.
         * \param coff Command to run when toggled off.
         * \param aoff Arguments to use when toggled off.
         * \param func Fuction to determine defaut setting.
         */
        toggle(
            const std::string& label,
            const std::string& con,
            const std::string& aon,
            const std::string& coff,
            const std::string& aoff,
            const std::function<bool(void)>& func
        );

        ~toggle() = default;  //!<  Default destructor.

        /*!
         * \brief Toggle on.
         */
        void on_left(void) override;

        /*!
         * \brief Toggle off.
         */
        void on_right(void) override;

        /*!
         * \brief Get display text
         * \return Vector of strings to render.
         */
        const std::vector<std::string> get_text(void) const override;

        /*!
         * \brief Return the command depending if the setting is toggled on or off.
         * \return Active command string.
         */
        const std::string get_active_cmd(void);

        /*!
         * \brief Return arguments depending if the setting is toggled on or off.
         * \return Active arguments string.
         */
        const std::string get_active_args(void);

        /*!
         * \brief Reset to default value.
         */
        void reset_to_default(void) override;

        /*!
         * \brief Determine and set the default value for the menu toggle item.
         */
        void set_default(void) override;

        /*!
         * \brief Checks if the current selection is the same as the default.
         * \return True if the setting changed, false if it did not.
         */
        const bool setting_changed(void) const override;

    private:
        bool toggled, default_val;
        std::string cmd_on;
        std::string cmd_off;
        std::string arg_on;
        std::string arg_off;

        std::function<bool(void)> defaulter;
};

}  //  end namespace wte::mnu

#endif
