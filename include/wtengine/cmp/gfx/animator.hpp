/*!
 * WTEngine | File:  animator.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_ANIMATOR_HPP
#define WTE_CMP_ANIMATOR_HPP

#include <functional>
#include <memory>

#include <allegro5/allegro.h>

#include "wtengine/cmp/component.hpp"

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/world.hpp"

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
         * \param a Object to compare to.
         * \return True if <, false if >
         */
        const bool operator<(const animator& a) const {
            return layer < a.layer;
        };

        /*!
         * \brief Run animaton function wrapper.
         * \param e_id ID of the entity calling the wrapper.
         */
        void run(const entity_id& e_id);

        /*!
         * \brief Set drawing to the internal bitmap.
         */
        void set_drawing(void);

        /*!
         * \brief Return the internal bitmap.
         * \return The internal bitmap.
         */
        ALLEGRO_BITMAP* get_bitmap(void) const;

        /*!
         * \brief Set a tint color.
         * \param c Allegro color.
         */
        void set_tint(const ALLEGRO_COLOR& c);

        /*!
         * \brief Get the tint color.
         * \return Allegro color.
         */
        const ALLEGRO_COLOR get_tint(void) const;

        /*!
         * \brief Clear tint color.
         */
        void clear_tint(void);

        /*!
         * \brief Check if the bitmap should be drawn tinted.
         * \return True if tint set, false if tint cleared.
         */
        const bool draw_tinted(void) const;

        /*!
         * \brief Check if visible.
         * \return True if visible, false if not.
         */
        const bool is_visible(void) const;

        /*!
         * \brief Show the entity.
         */
        void show(void);

        /*!
         * \brief Hide the entity.
         */
        void hide(void);

    protected:
        /*!
         * \brief Create a new component based on the Animator.
         * \param bmp Bitmap asset to use.
         * \param l Layer position.
         * \param func Animation function.
         */
        animator(
            std::shared_ptr<wte_asset> bmp,
            const std::size_t& l,
            const std::function<void(const entity_id&)>& func
        );

        //!  Stores the bitmap used by the animator.
        std::shared_ptr<wte_asset> internal_bitmap;

    private:
        std::size_t layer;          //  Layer position.
        bool tint_set;              //  Flag to set tint.
        bool visible;               //  Flag to set visibility.
        ALLEGRO_COLOR tint_color;   //  Color of tint.

        //  Animation function.
        std::function<void(const entity_id&)> animate;
};

} //  namespace cmp

} //  namespace wte

#endif
