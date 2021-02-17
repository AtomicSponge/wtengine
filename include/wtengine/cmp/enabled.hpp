/*!
 * WTEngine | File:  enabled.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_ENABLED_HPP
#define WTE_CMP_ENABLED_HPP

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class enabled
 * \brief Toggle an entity to enabled or disabled state.
 */
class enabled final : public component {
    public:
        /*!
         * \brief Enabled constructor.
         * 
         * If passed nothing, enabled by default.
         */
        enabled() : is_enabled(true) {};

        /*!
         * \brief Enabled constructor.
         * 
         * Pass boolean value to the component to set if it is enabled.
         */
        enabled(const bool e) : is_enabled(e) {};

        /*!
         * \brief Check if the entity is enabled.
         * 
         * \return True if enabled, false if not.
         */
        const bool check(void) const {
            return is_enabled;
        };

        /*!
         * \brief Set enabled.
         * 
         * Sets internal flag to true.
         */
        void enable(void) {
            is_enabled = true;
        };

        /*!
         * \brief Set disabled.
         * 
         * Sets internal flag to false.
         */
        void disable(void) {
            is_enabled = false;
        };

    private:
        bool is_enabled;
};

} //  namespace cmp

} //  namespace wte

#endif
