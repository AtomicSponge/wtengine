/*!
 * WTEngine | File:  enabled.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_ENABLED_HPP
#define WTE_CMP_ENABLED_HPP

#include "wtengine/cmp/component.hpp"

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
         * \brief Create a new Enabled componenet.
         */
        enabled();

        /*!
         * \brief Create a new Enabled componenet.
         * \param e Boolean value to set enabled flag.
         */
        enabled(const bool& e);

        /*!
         * \brief Check if the entity is enabled.
         * \return True if enabled, false if not.
         */
        const bool check(void) const;

        /*!
         * \brief Set to enabled.
         */
        void enable(void);

        /*!
         * \brief Set to disabled.
         */
        void disable(void);

    private:
        bool is_enabled;  //  Enabled flag.
};

} //  namespace cmp

} //  namespace wte

#endif
