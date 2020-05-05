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

#include "../component.hpp"
#include "../../mgr/entity_manager.hpp"

namespace wte
{

namespace cmp
{

//! Animator component
/*!
 * Animator
 */
class animator : public component {
    public:
        /*!
         * animator
         * \param void
         * \return void
         */
        inline animator() {}

        /*!
         * animator
         * \param void
         * \return void
         */
        inline animator(void func(entity, mgr::entity_manager&, int64_t)) : animate(func) {}

        /*!
         * animator
         * \param void
         * \return void
         */
        inline ~animator() {}

        inline void run(entity eid, mgr::entity_manager& world, int64_t engine_time) {
            animate(eid, world, engine_time);
        }

    protected:
        std::function<void(entity, mgr::entity_manager&, int64_t)> animate;
};

} //  namespace cmp

} //  namespace wte

#endif
