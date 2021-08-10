/*!
 * WTEngine | File:  sprite.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_SPRITE_HPP
#define WTE_CMP_SPRITE_HPP

#include <string>
#include <utility>
#include <map>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "wtengine/cmp/gfx/animator.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class sprite
 * \brief Component for loading a sprite sheet and storing its animation frames.
 */
class sprite final : public animator {
    public:
        /*!
         * \brief Create a new Sprite component.
         * \param sw Sprite width.
         * \param sh Sprite height.
         * \param dox Horizontal draw offset in relation to entity location.
         * \param doy Verticle draw offset in relation to entity location.
         * \param rt Rate in engine timer ticks for animation.
         * \param l Layer for sorting.
         */
        sprite(std::shared_ptr<wte_asset> bmp,
               const float& sw, const float& sh,
               const float& dox, const float& doy,
               const std::size_t& rt, const std::size_t& l);

        /*!
         * \brief Sprite destructor.
         */
        ~sprite();

        /*!
         * \brief Add animation cycle.
         * \param name Name of cycle.
         * \param start Start cell of cycle.
         * \param stop End cell of cycle.
         * \return True if created, false if not.
         */
        const bool add_cycle(const std::string& name,
                             const std::size_t& start,
                             const std::size_t& stop);

        /*!
         * \brief Set the active cycle.
         * \param name Name of cycle to set.
         * \return True if set, false if not.
         */
        const bool set_cycle(const std::string& name);

        /*!
         * \brief Get the sprite width.
         * \return Sprite width.
         */
        const float get_sprite_width(void) const;

        /*!
         * \brief Get the sprite height.
         * \return Sprite height.
         */
        const float get_sprite_height(void) const;

        /*!
         * \brief Get the X draw offset.
         * \return X offset in pixels.
         */
        const float get_draw_offset_x(void) const;

        /*!
         * \brief Get the Y draw offset.
         * \return Y offset in pixels.
         */
        const float get_draw_offset_y(void) const;

        /*!
         * \brief Get the sheet's X location for sprite drawing.
         * \return Sprite sheet X location in pixels.
         */
        const float get_sprite_x(void) const;

        /*!
         * \brief Get the sheet's Y location for sprite drawing.
         * \return Sprite sheet Y location in pixels.
         */
        const float get_sprite_y(void) const;

        /*!
         * \brief Set the X scale factor.
         * \param sx New scale factor X value.
         */
        void set_scale_factor_x(const float& sx);

        /*!
         * \brief Set the Y scale factor.
         * \param sy New scale factor Y value.
         */
        void set_scale_factor_y(const float& sy);

        /*!
         * \brief Get the X scale factor.
         * \return scale_factor_x
         */
        const float get_scale_factor_x(void) const;

        /*!
         * \brief Get the Y scale factor.
         * \return scale_factor_y
         */
        const float get_scale_factor_y(void) const;

    private:
        //  Animation cycle index.
        std::map<std::string, std::pair<std::size_t, std::size_t>> cycles;

        float sprite_width, sprite_height;      //  Sprite width & height.
        float draw_offset_x, draw_offset_y;     //  Sprite draw offset x/y.
        float sprite_x, sprite_y;               //  Sprite location.
        float scale_factor_x, scale_factor_y;   //  Sprite x/y scale factor.
        int sheet_width, sheet_height;          //  Sprite sheet size (w/h).
        std::size_t start_frame, stop_frame;    //  Current start/stop frame postitions.
        std::size_t current_frame, rate;        //  Current frame counter and frame rate.
};

} //  namespace cmp

} //  namespace wte

#endif
