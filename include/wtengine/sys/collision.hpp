/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SYS_COLISION_HPP)
#define WTE_SYS_COLISION_HPP

#include "wtengine/sys/system.hpp"

namespace wte::sys {

/*!
 * \class colision
 * \brief Selects components by team and tests for colisions.
 */
class colision final : public system {
    public:
        colision();
        ~colision() = default;

        /*!
         * \brief Selects components by team, then tests each team to see if there is a colision.
         */
        void run(void) override;
};

}  //  end namespace wte::sys

#endif
