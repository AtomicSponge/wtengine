/*!
 * WTEngine | File:  sprite.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_SPRITE_HPP
#define WTE_CMP_SPRITE_HPP

#include <string>
#include <utility>
#include <map>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "animator.hpp"
#include "../../mgr/entity_manager.hpp"
#include "../../wte_global_defines.hpp"

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
         * \brief Sprite constructor.
         * 
         * \param sw Sprite width.
         * \param sh Sprite height.
         * \param dox Horizontal draw offset in relation to entity location.
         * \param doy Verticle draw offset in relation to entity location.
         * \param rt Rate in engine timer ticks for animation.
         * \param l Layer for sorting.
         */
        sprite(const float sw, const float sh,
                      const float dox, const float doy,
                      const std::size_t rt, const std::size_t l) :
        animator(l,
            [](const entity_id& e_id, mgr::entity_manager& world, const int64_t& engine_time) {
                //  Define sprite animation process.
                if(engine_time % world.get_component<sprite>(e_id)->rate == 0) {
                    //  Increment frame.
                    world.set_component<sprite>(e_id)->current_frame++;
                    //  Loop frame.
                    if(world.get_component<sprite>(e_id)->current_frame > world.get_component<sprite>(e_id)->stop_frame) {
                        world.set_component<sprite>(e_id)->current_frame = world.get_component<sprite>(e_id)->start_frame;
                    }
                    //  Calculate the X position in the sprite sheet.
                    world.set_component<sprite>(e_id)->sprite_x = (float)
                        ((int)(world.get_component<sprite>(e_id)->current_frame * world.get_component<sprite>(e_id)->sprite_width +
                        world.get_component<sprite>(e_id)->sheet_width) % world.get_component<sprite>(e_id)->sheet_width);
                    //  Calculate the Y position in the sprite sheet.
                    world.set_component<sprite>(e_id)->sprite_y = (float)
                        ((int)((world.get_component<sprite>(e_id)->current_frame * world.get_component<sprite>(e_id)->sprite_width) /
                        world.get_component<sprite>(e_id)->sheet_width) * world.get_component<sprite>(e_id)->sprite_height);
                }
            }
        ),
        sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
        sprite_x(0.0f), sprite_y(0.0f), scale_factor_x(1.0f), scale_factor_y(1.0f),
        start_frame(0), stop_frame(0), current_frame(0), rate(rt)
        {
            internal_bitmap = NULL;
            if(rate == 0) rate = 1;
        };

        /*!
         * \brief Sprite destructor.
         */
        ~sprite() {
            al_destroy_bitmap(internal_bitmap);
        };

        /*!
         * \brief Load a sprite sheet.
         * 
         * Will use magic pink (rgb 255,0,255) for transparency if WTE_NO_MAGIC_PINK
         * is not defined.
         * 
         * \param fname Filename of sprite sheet.
         */
        void load_sprite(const std::string& fname) {
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) throw std::runtime_error("Couldn't find sprite file:  " + fname);

            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            internal_bitmap = al_load_bitmap_f(file, fname.substr(fname.find("."),
                                                     fname.length()).c_str());
            al_fclose(file);
            if(!internal_bitmap) throw std::runtime_error("Couldn't load sprite:  " + fname);

            #if WTE_USE_MAGIC_PINK
            al_convert_mask_to_alpha(internal_bitmap, WTE_MAGIC_PINK);
            #endif

            sheet_width = al_get_bitmap_width(internal_bitmap);
            sheet_height = al_get_bitmap_height(internal_bitmap);
        };

        /*!
         * \brief Add animation cycle.
         * 
         * \param name Name of cycle.
         * \param start Start cell of cycle.
         * \param stop End cell of cycle.
         * \return True if created, false if not.
         */
        const bool add_cycle(const std::string& name,
                                    const std::size_t& start,
                                    const std::size_t& stop) {
            auto ret = cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
            return ret.second;
        };

        /*!
         * \brief Set the active cycle.
         * 
         * \param name Name of cycle to set.
         * \return True if set, false if not.
         */
        const bool set_cycle(const std::string& name) {
            auto it = cycles.find(name);
            if(it != cycles.end()) {
                start_frame = it->second.first;
                stop_frame = it->second.second;
                return true;
            } else return false;
        };

        /*!
         * \brief Get the sprite width.
         * 
         * \return Sprite width.
         */
        const float get_sprite_width(void) const {
            return sprite_width;
        };

        /*!
         * \brief Get the sprite height.
         * 
         * \return Sprite height.
         */
        const float get_sprite_height(void) const {
            return sprite_height;
        };

        /*!
         * \brief Get the X draw offset.
         * 
         * \return draw_offset_x
         */
        const float get_draw_offset_x(void) const {
            return draw_offset_x;
        };

        /*!
         * \brief Get the Y draw offset.
         * 
         * \return draw_offset_y
         */
        const float get_draw_offset_y(void) const {
            return draw_offset_y;
        };

        /*!
         * \brief Get the X location for sprite drawing.
         * 
         * \return sprite_x
         */
        const float get_sprite_x(void) const {
            return sprite_x;
        };

        /*!
         * \brief Get the Y location for sprite drawing.
         * 
         * \return sprite_y
         */
        const float get_sprite_y(void) const {
            return sprite_y;
        };

        /*!
         * \brief Set the X scale factor.
         * 
         * \param sx New scale factor X value.
         */
        void set_scale_factor_x(const float& sx) {
            scale_factor_x = sx;
        };

        /*!
         * \brief Set the Y scale factor.
         * 
         * \param sy New scale factor Y value.
         */
        void set_scale_factor_y(const float& sy) {
            scale_factor_y = sy;
        };

        /*!
         * \brief Get the X scale factor.
         * 
         * \return scale_factor_x
         */
        const float get_scale_factor_x(void) const {
            return scale_factor_x;
        };

        /*!
         * \brief Get the Y scale factor.
         * 
         * \return scale_factor_y
         */
        const float get_scale_factor_y(void) const {
            return scale_factor_y;
        };

    private:
        std::map<std::string, std::pair<std::size_t, std::size_t>> cycles;

        float sprite_width, sprite_height;
        float draw_offset_x, draw_offset_y;
        float sprite_x, sprite_y;
        float scale_factor_x, scale_factor_y;
        int sheet_width, sheet_height;
        std::size_t start_frame, stop_frame;
        std::size_t current_frame, rate;
};

} //  namespace cmp

} //  namespace wte

#endif
