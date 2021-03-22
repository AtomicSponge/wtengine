/*!
 * WTEngine | File:  collision.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_COLISION_HPP
#define WTE_SYS_COLISION_HPP

#include "wtengine/sys/system.hpp"

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
         * 
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void run(mgr::entity_manager& world,
                 mgr::message_manager& messages,
                 const int64_t& current_time) override;
};

} //  namespace sys

} //  namespace wte

#endif
