/*!
 * WTEngine | File:  animate.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_ANIMATE_HPP
#define WTE_SYS_ANIMATE_HPP

#include "wtengine/sys/system.hpp"

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
         */
        void run(void) override;
};

} //  namespace sys

} //  namespace wte

#endif
