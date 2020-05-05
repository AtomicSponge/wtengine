/*!
 * \brief WTEngine | File:  animator.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Animator component.
 */

#ifndef WTE_CMP_ANIMATOR_HPP
#define WTE_CMP_ANIMATOR_HPP

//#include <allegro5/allegro.h>

#include <functional>

#include "../../mgr/entity_manager.hpp"

namespace wte
{

namespace cmp
{

//! Animator component
/*!
 * Animator
 */
class animator {
    public:
        /*!
         * animator
         * \param void
         * \return void
         */
        inline animator() {
            //
        }

        /*!
         * animator
         * \param void
         * \return void
         */
        inline ~animator() {
            //
        }

        std::function<void(mgr::entity_manager&)> animate;
};

} //  namespace cmp

} //  namespace wte

#endif
