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

#include <allegro5/allegro.h>

#include <functional>

#include "../component.hpp"
#include "../../mgr/entity_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Animator component
 * Interface class for components with an animation process.
 */
class animator : public component {
    public:
        /*!
         * Animator destructor.
         * \param void
         * \return void
         */
        inline ~animator() {};

        /*!
         * Overload < operator to sort by layer value.
         * \param a Object to compare to.
         * \return True if <, false if >
         */
        inline const bool operator<(const animator& a) const {
            return layer < a.layer;
        };

        /*!
         * Run animaton function wrapper.
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param engine_time Current value of the main timer.
         * \return void
         */
        inline void run(const entity_id e_id, mgr::entity_manager& world, const int64_t engine_time) {
            animate(e_id, world, engine_time);
        };

        /*!
         * Set drawing to the internal bitmap.
         * \param void
         * \return void
         */
        inline void set_drawing(void) {
            al_set_target_bitmap(internal_bitmap);
        };

        /*!
         * Return the internal bitmap.
         * \param void
         * \return The internal bitmap.
         */
        inline ALLEGRO_BITMAP& get_bitmap(void) const {
            return *internal_bitmap;
        };

    protected:
        /*!
         * Animator constructor.
         * \param l Layer for sorting.
         * \param func Animation function.
         * \return void
         */
        inline animator(std::size_t l, void func(const entity_id, mgr::entity_manager&, const int64_t)) :
        layer(l), animate(func) {};

        ALLEGRO_BITMAP* internal_bitmap;

        std::function<void(const entity_id, mgr::entity_manager&, const int64_t)> animate;

        std::size_t layer;
};

} //  namespace cmp

} //  namespace wte

#endif
