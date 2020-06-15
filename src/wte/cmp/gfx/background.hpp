/*!
 * \brief WTEngine | File:  background.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Background component.
 */

#ifndef WTE_CMP_BACKGROUND_HPP
#define WTE_CMP_BACKGROUND_HPP

#include <string>

#include <allegro5/allegro.h>

#include "../../engine_cfg_map.hpp"
#include "animator.hpp"

namespace wte
{

namespace cmp
{

//! Background component
/*!
 * Store the background bitmap
 */
class background final : public animator {
    public:
        /*!
         * Static background, solid color.
         * \param w Background width.
         * \param h Background height.
         * \param c Allegro color.
         * \param l Background layer.
         * \return void
         */
        inline background(const int w,
                          const int h,
                          const std::size_t l,
                          ALLEGRO_COLOR c) : color(c),
        animator(l, [](entity e_id, mgr::entity_manager& world, int64_t engine_time) {
            al_set_target_bitmap(world.set_component<cmp::background>(e_id)->background_bitmap);
            al_clear_to_color(world.get_component<cmp::background>(e_id)->color);
        }) {
            background_bitmap = al_create_bitmap(w, h);
        };

        /*!
         * Background with custom animation.
         * \param w Background width.
         * \param h Background height.
         * \param l Background layer.
         * \param func Animcation function.
         * \return void
         */
        inline background(const int w,
                          const int h,
                          const std::size_t l,
                          void func(entity, mgr::entity_manager&, int64_t)) :
        animator(l, func) {
            background_bitmap = al_create_bitmap(w, h);
        };

        /*!
         * WIP - finish file loading
         */
        inline bool load_file(std::string fname) {
            ALLEGRO_BITMAP* temp_bmp;
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            temp_bmp = al_load_bitmap_f(file, NULL);
            al_fclose(file);
            al_destroy_bitmap(temp_bmp);
            return true;
        };

        /*!
         * Background destructor.
         * \param void
         * \return void
         */
        inline ~background() {
            al_destroy_bitmap(background_bitmap);
        };

        ALLEGRO_BITMAP* background_bitmap;
        ALLEGRO_COLOR color;
};

} //  namespace cmp

} //  namespace wte

#endif
