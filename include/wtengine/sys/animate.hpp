/*!
 * WTEngine | File:  animate.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_ANIMATE_HPP
#define WTE_SYS_ANIMATE_HPP

#include "wtengine/sys/system.hpp"

namespace wte::sys::gfx {

/*!
 * \class animate
 * \brief Find the animate components and process them.
 */
class animate final : public system {
    public:
        animate();
        ~animate() = default;

        /*!
         * \brief Gets all animation components and processes their run members.
         * 
         * The entity must also have the visible component and is set visible to be drawn.
         */
        void run(void) override;
};

}  //  end namespace wte::sys

#endif
