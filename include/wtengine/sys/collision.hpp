/*!
 * WTEngine | File:  collision.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_COLISION_HPP
#define WTE_SYS_COLISION_HPP

#include "wtengine/sys/system.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class colision
 * \brief Will select components by team and test for a colision
 */
class colision final : public system {
    public:
        colision();
        ~colision();

        /*!
         * \brief Allow the colision system to be disabled.
         */
        void disable(void) override;

        /*!
         * \brief Colision system run.
         * 
         * Overrides virtual run member inherited from system object.
         * Selects components by team, then tests each team to see if there is a colision.
         */
        void run(void) override;
};

} //  namespace sys

} //  namespace wte

#endif
