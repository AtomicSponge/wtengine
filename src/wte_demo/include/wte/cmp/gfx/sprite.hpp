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

namespace wte
{

namespace cmp
{

//! Sprite component
/*!
 * WIP
 */
class sprite final : public animator {
    public:
        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline sprite(const float sw, const float sh, const float dox, const float doy, const std::size_t sp, const std::size_t l) :
        sprite_width(sw - 1.0), sprite_height(sh - 1.0), draw_offset_x(dox), draw_offset_y(doy),
        sprite_x(0), sprite_y(0), start_frame(0), stop_frame(0), current_frame(0), speed(sp),
        animator(l, [](entity e_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define sprite animation process.
            if(engine_time % world.get_component<sprite>(e_id)->speed == 0) {
                world.set_component<sprite>(e_id)->current_frame++;
            }
            if(world.get_component<sprite>(e_id)->current_frame > world.get_component<sprite>(e_id)->stop_frame) {
                world.set_component<sprite>(e_id)->current_frame = world.get_component<sprite>(e_id)->start_frame;
            }
            world.set_component<sprite>(e_id)->sprite_x = 0;
            world.set_component<sprite>(e_id)->sprite_y = 0;
            //world.set_component<sprite>(e_id)->sprite_x = world.get_component<sprite>(e_id)->current_frame * world.get_component<sprite>(e_id)->sprite_width;
        }) {
            sprite_bitmap = NULL;
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline ~sprite() {
            al_destroy_bitmap(sprite_bitmap);
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void load_sprite(const std::string fname) {
            sprite_bitmap = al_load_bitmap(fname.c_str());
            if(sprite_bitmap == NULL) throw std::runtime_error("Error loading sprite file:  " + fname);
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void add_cycle(const std::string name, const std::size_t start, const std::size_t stop) {
            cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
        };

        /*!
         * Sprite
         * \param void
         * \return void
         */
        inline void set_cycle(const std::string name) {
            auto it = cycles.find(name);
            if(it != cycles.end()) {
                start_frame = it->second.first;
                stop_frame = it->second.second;
            }
        };

        ALLEGRO_BITMAP* sprite_bitmap;

        float sprite_width, sprite_height;
        float draw_offset_x, draw_offset_y;
        float sprite_x, sprite_y;
        std::size_t current_frame, speed;
        std::size_t start_frame, stop_frame;

    private:
        std::map<std::string, std::pair<std::size_t, std::size_t>> cycles;
};

} //  namespace cmp

} //  namespace wte

#endif
