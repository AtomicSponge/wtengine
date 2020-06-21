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
#include <map>

#include <allegro5/allegro.h>

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
                          ALLEGRO_COLOR c) :
        background_w(w), background_h(h), color(c),
        animator(l, [](entity e_id, mgr::entity_manager& world, int64_t engine_time) {
            al_set_target_bitmap(world.set_component<cmp::background>(e_id)->background_bitmap);
            al_clear_to_color(world.get_component<cmp::background>(e_id)->color);
        }) {
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            background_bitmap = al_create_bitmap(background_w, background_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
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
        background_w(w), background_h(h), animator(l, func) {
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            background_bitmap = al_create_bitmap(background_w, background_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
        };

        /*!
         * Background destructor.
         * \param void
         * \return void
         */
        inline ~background() {
            for(auto & it : bmp_map) al_destroy_bitmap(it.second);
            al_destroy_bitmap(background_bitmap);
        };

        /*!
         * \brief Reload the background bitmap.
         * Called when the screen is updated.
         * \param void
         * \return void
         */
        inline void reload_background_bitmap(void) {
            al_destroy_bitmap(background_bitmap);
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            background_bitmap = al_create_bitmap(background_w, background_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
        };

        /*!
         * \brief Load a bitmap.
         * Store in a map for reference to later.  Call this during entity creation.
         * \param fname Filename to load.
         * \param label Label for referencing bitmap.
         */
        inline bool load_file(std::string label, std::string fname) {
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {
                al_fclose(file);
                return false;
            }
            bmp_map.insert(std::make_pair(label, al_load_bitmap_f(file, fname.substr(fname.find("."),
                                                                        fname.length()).c_str())));
            al_fclose(file);
            return true;
        };

        ALLEGRO_BITMAP* background_bitmap;
        ALLEGRO_COLOR color;

        std::map<std::string, ALLEGRO_BITMAP*> bmp_map;

    private:
        int background_w, background_h;
};

} //  namespace cmp

} //  namespace wte

#endif
