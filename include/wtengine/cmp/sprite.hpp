/*!
 * wtengine | File:  sprite.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_CMP_SPRITE_HPP
#define WTE_CMP_SPRITE_HPP

#include <string>
#include <utility>
#include <map>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "wtengine/cmp/gfx.hpp"

#include "wtengine/_debug/wte_exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte::mgr::gfx {
    class renderer;
}

namespace wte::cmp::gfx {

/*!
 * \class sprite
 * \brief Component for loading a sprite sheet and storing its animation frames.
 */
class sprite final : public gfx {
    friend class mgr::gfx::renderer;

    public:
        /*!
         * \brief Create a new Sprite component.
         * \param l Layer for sorting.
         * \param sw Sprite width.
         * \param sh Sprite height.
         * \param dox Horizontal draw offset in relation to entity location.
         * \param doy Verticle draw offset in relation to entity location.
         * \param rt Rate in engine timer ticks for animation.
         */
        sprite(
            wte_asset<al_bitmap> bmp,
            const std::size_t& l,
            const float& sw,
            const float& sh,
            const float& dox,
            const float& doy,
            const std::size_t& rt
        );

        sprite() = delete;    //!<  Delete default constructor.
        ~sprite() = default;  //!<  Default destructor.

        /*!
         * \brief Add animation cycle.
         * \param name Name of cycle.
         * \param start Start cell of cycle.
         * \param stop End cell of cycle.
         * \return True if created, false if not.
         */
        const bool add_cycle(
            const std::string& name,
            const std::size_t& start,
            const std::size_t& stop
        );

        /*!
         * \brief Set the active cycle.
         * \param name Name of cycle to set.
         * \return True if set, false if not.
         */
        const bool set_cycle(const std::string& name);

    private:
        //  Animation cycle index.
        std::map<
            const std::string,
            const std::pair<const std::size_t, const std::size_t>
        > cycles;

        float sprite_width, sprite_height;      //  Sprite width & height.
        float draw_offset_x, draw_offset_y;     //  Sprite draw offset x/y.
        float sprite_x, sprite_y;               //  Sprite location.
        int sheet_width, sheet_height;          //  Sprite sheet size (w/h).
        std::size_t start_frame, stop_frame;    //  Current start/stop frame postitions.
        std::size_t current_frame, rate;        //  Current frame counter and frame rate.
};

}  //  end namespace wte::cmp

#endif
