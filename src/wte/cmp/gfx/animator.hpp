/*!
 * WTEngine | File:  animator.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
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
 * \class animator
 * \brief Interface class for components with an animation process.
 */
class animator : public component {
    public:
        /*!
         * \brief Animator destructor.
         */
        inline ~animator() {};

        /*!
         * \brief Overload < operator to sort by layer value.
         * 
         * \param a Object to compare to.
         * \return True if <, false if >
         */
        inline const bool operator<(const animator& a) const {
            return layer < a.layer;
        };

        /*!
         * \brief Run animaton function wrapper.
         * 
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param engine_time Current value of the main timer.
         */
        inline void run(const entity_id& e_id, mgr::entity_manager& world, const int64_t& engine_time) {
            animate(e_id, world, engine_time);
        };

        /*!
         * \brief Set drawing to the internal bitmap.
         */
        inline void set_drawing(void) {
            al_set_target_bitmap(internal_bitmap);
        };

        /*!
         * \brief Return the internal bitmap.
         * 
         * \return The internal bitmap.
         */
        inline ALLEGRO_BITMAP& get_bitmap(void) const {
            return *internal_bitmap;
        };

        /*!
         * \brief Set a tint color.
         * 
         * \param c Allegro color.
         */
        inline void set_tint(const ALLEGRO_COLOR& c) {
            tint_color = c;
            tint_set = true;
        };

        /*!
         * \brief Get the tint color.
         * 
         * \return Allegro color.
         */
        inline const ALLEGRO_COLOR get_tint(void) const {
            return tint_color;
        };

        /*!
         * \brief Clear tint color.
         */
        inline void clear_tint(void) {
            tint_set = false;
        };

        /*!
         * \brief Check if the bitmap should be drawn tinted.
         * 
         * \return True if tint set, false if tint cleared.
         */
        inline const bool draw_tinted(void) const {
            return tint_set;
        };

    protected:
        /*!
         * \brief Animator constructor.
         * 
         * \param l Layer for sorting.
         * \param func Animation function.
         */
        inline animator(std::size_t l, void func(const entity_id&, mgr::entity_manager&, const int64_t&)) :
        layer(l), tint_set(false), animate(func) {};

        //!  Stores the bitmap used by the animator.
        ALLEGRO_BITMAP* internal_bitmap;

    private:
        ALLEGRO_COLOR tint_color;
        bool tint_set;

        std::function<void(const entity_id&, mgr::entity_manager&, const int64_t&)> animate;

        std::size_t layer;
};

} //  namespace cmp

} //  namespace wte

#endif
