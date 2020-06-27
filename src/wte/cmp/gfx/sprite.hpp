/*!
 * \brief WTEngine | File:  sprite.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Sprite component.
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

//! Sprite component
/*!
 * Sprite
 */
class sprite final : public animator {
    public:
        /*!
         * Sprite constructor.
         * \param sw Sprite width.
         * \param sh Sprite height.
         * \param dox Horizontal draw offset in relation to entity location.
         * \param doy Verticle draw offset in relation to entity location.
         * \param rt Rate in engine timer ticks for animation.
         * \param l Layer for sorting.
         * \return void
         */
        inline sprite(const float sw, const float sh, const float dox, const float doy, const std::size_t rt, const std::size_t l) :
        sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
        sprite_x(0.0f), sprite_y(0.0f), start_frame(0), stop_frame(0), current_frame(0), rate(rt),
        scale_factor_x(1.0f), scale_factor_y(1.0f),
        animator(l,
            [](entity_id e_id, mgr::entity_manager& world, int64_t engine_time) {
                //  Define sprite animation process.
                if(engine_time % world.get_component<sprite>(e_id)->rate == 0) {
                    world.set_component<sprite>(e_id)->current_frame++;
                    if(world.get_component<sprite>(e_id)->current_frame > world.get_component<sprite>(e_id)->stop_frame) {
                        world.set_component<sprite>(e_id)->current_frame = world.get_component<sprite>(e_id)->start_frame;
                    }
                    world.set_component<sprite>(e_id)->sprite_x = (float)
                        ((int)(world.get_component<sprite>(e_id)->current_frame * world.get_component<sprite>(e_id)->sprite_width +
                        world.get_component<sprite>(e_id)->sheet_width) % world.get_component<sprite>(e_id)->sheet_width);
                    world.set_component<sprite>(e_id)->sprite_y = (float)
                        ((int)((world.get_component<sprite>(e_id)->current_frame * world.get_component<sprite>(e_id)->sprite_width) /
                        world.get_component<sprite>(e_id)->sheet_width) * world.get_component<sprite>(e_id)->sprite_height);
                }
            }
        ) {
            sprite_bitmap = NULL;
            if(rate == 0) rate = 1;
        };

        /*!
         * Sprite destructor.
         * \param void
         * \return void
         */
        inline ~sprite() {
            al_destroy_bitmap(sprite_bitmap);
        };

        /*!
         * \brief Load a sprite sheet.
         * Will use magic pink (rgb 255,0,255) for transparency if WTE_NO_MAGIC_PINK
         * is not defined.
         * \param fname Filename of sprite sheet.
         * \return void
         */
        inline void load_sprite(const std::string fname) {
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            sprite_bitmap = al_load_bitmap_f(file, fname.substr(fname.find("."),
                                                   fname.length()).c_str());
            al_fclose(file);
            if(!sprite_bitmap) throw std::runtime_error("Error loading sprite file:  " + fname);
            #if WTE_USE_MAGIC_PINK
            al_convert_mask_to_alpha(sprite_bitmap, WTE_MAGIC_PINK);
            #endif
            sheet_width = al_get_bitmap_width(sprite_bitmap);
            sheet_height = al_get_bitmap_height(sprite_bitmap);
        };

        /*!
         * Add animation cycle.
         * \param name Name of cycle.
         * \param start Start cell of cycle.
         * \param stop End cell of cycle.
         * \return void
         */
        inline const bool add_cycle(const std::string name, const std::size_t start, const std::size_t stop) {
            auto ret = cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
            return ret.second;
        };

        /*!
         * Set the active cycle.
         * \param name Name of cycle to set.
         * \return void
         */
        inline const bool set_cycle(const std::string name) {
            auto it = cycles.find(name);
            if(it != cycles.end()) {
                start_frame = it->second.first;
                stop_frame = it->second.second;
                return true;
            } else return false;
        };

        ALLEGRO_BITMAP* sprite_bitmap;

        float sprite_width, sprite_height;
        float draw_offset_x, draw_offset_y;
        float sprite_x, sprite_y;
        float scale_factor_x, scale_factor_y;
        int sheet_width, sheet_height;
        std::size_t current_frame, rate;
        std::size_t start_frame, stop_frame;

    private:
        std::map<std::string, std::pair<std::size_t, std::size_t>> cycles;
};

} //  namespace cmp

} //  namespace wte

#endif
