/*!
 * WTEngine | File:  background.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_BACKGROUND_HPP
#define WTE_CMP_BACKGROUND_HPP

#include <string>
#include <map>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/cmp/gfx/animator.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class background
 * \brief Component for storing a Background image and defining its animation process.
 */
class background final : public animator {
    public:
        /*!
         * \brief Create a static Background component, solid color.
         * \param w Background width.
         * \param h Background height.
         * \param c Allegro color.
         * \param l Background layer.
         */
        background(wte_asset<al_bitmap> bmp, const std::size_t& l, const ALLEGRO_COLOR& c);

        /*!
         * \brief Create a Background component with custom animation.
         * \param w Background width.
         * \param h Background height.
         * \param l Background layer.
         * \param func Animcation function.
         */
        background(wte_asset<al_bitmap> bmp, const std::size_t& l,
                   const std::function<void(const entity_id&)>& func);

        /*!
         * \brief Background destructor.
         */
        ~background();

        /*!
         * \brief Get the saved color.
         * \return Allegro color object.
         */
        ALLEGRO_COLOR get_color(void) const;

    private:
        ALLEGRO_COLOR color;    //  Background color for solid bg.
};

} //  namespace cmp

} //  namespace wte

#endif
