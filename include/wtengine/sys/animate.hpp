/*!
 * WTEngine | File:  animate.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_SYS_ANIMATE_HPP
#define WTE_SYS_ANIMATE_HPP

#include "system.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class animate
 * \brief Find the animate components and process them.
 */
class animate final : public system {
    public:
        animate();
        ~animate();

        /*!
         * \brief Animate system run.
         * 
         * Gets all animation components and processes their run members.
         * The entity must also have the visible component and is set visible to be drawn.
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
