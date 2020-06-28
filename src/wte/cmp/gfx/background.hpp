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
        animator(l, [](entity_id e_id, mgr::entity_manager& world, int64_t engine_time) {
            world.set_component<cmp::background>(e_id)->set_drawing();
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
                          void func(entity_id, mgr::entity_manager&, int64_t)) :
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
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            ALLEGRO_BITMAP* temp_bmp = al_clone_bitmap(background_bitmap);
            al_destroy_bitmap(background_bitmap);
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            background_bitmap = al_create_bitmap(background_w, background_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            al_set_target_bitmap(background_bitmap);
            al_draw_bitmap(temp_bmp, 0.0f, 0.0f, 0);
            al_destroy_bitmap(temp_bmp);
        };

        /*!
         * Set drawing to the internal bitmap.
         * \param void
         * \return void
         */
        inline void set_drawing(void) {
            al_set_target_bitmap(background_bitmap);
        };

        /*!
         * \brief Load a bitmap.
         * Store in a map for reference to later.  Call this during entity creation.
         * \param fname Filename to load.
         * \param label Label for referencing bitmap.
         */
        inline bool load_bitmap(const std::string& label, const std::string& fname) {
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {
                al_fclose(file);
                return false;
            }
            ALLEGRO_BITMAP* temp_bitmap =
                al_load_bitmap_f(file, fname.substr(fname.find("."), fname.length()).c_str());
            al_fclose(file);

            if(!temp_bitmap) return false;

            #if WTE_USE_MAGIC_PINK
            al_convert_mask_to_alpha(temp_bitmap, WTE_MAGIC_PINK);
            #endif

            bmp_map.insert(std::make_pair(label, al_clone_bitmap(temp_bitmap)));

            al_destroy_bitmap(temp_bitmap);
            return true;
        };

        /*!
         * \brief Draw a loaded bitmap.
         * Draws a bitmap from the reference map.  Call during animation.
         * \param labal Label of saved bitmap.
         * \param x X location of the background in pixels.
         * \param y Y location of the background in pixels.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_bitmap(const std::string& label, const float& x, const float& y, const int& flags) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_bitmap(it->second, x, y, flags);
        };

        ALLEGRO_BITMAP* background_bitmap;
        ALLEGRO_COLOR color;

    private:
        std::map<std::string, ALLEGRO_BITMAP*> bmp_map;

        int background_w, background_h;
};

} //  namespace cmp

} //  namespace wte

#endif
