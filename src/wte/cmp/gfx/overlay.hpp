/*!
 * \brief WTEngine | File:  overlay.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Overlay component.
 */

#ifndef WTE_CMP_OVERLAY_HPP
#define WTE_CMP_OVERLAY_HPP

#include <string>
#include <map>

#include <allegro5/allegro.h>

#include "animator.hpp"

namespace wte
{

namespace cmp
{

//! Overlay component
/*!
 * Store the overlay bitmap
 */
class overlay final : public animator {
    public:
        /*!
         * Overlay constructor.
         * \param w Overlay width in pixels.
         * \param h Overlay height in pixels.
         * \param x Horizontal location of the overlay in pixels.
         * \param y Verticle location of the overlay in pixels.
         * \param l Layer for sorting.
         * \param func Function to define what is displayed in the overlay.
         * \return void
         */
        inline overlay(int w, int h, float x, float y,
                       std::size_t l, void func(entity, mgr::entity_manager&, int64_t)) :
        overlay_w(w), overlay_h(h), pos_x(x), pos_y(y), animator(l, func) {
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            overlay_bitmap = al_create_bitmap(overlay_w, overlay_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            overlay_font = NULL;
        };

        /*!
         * Overlay destructor.
         * \param void
         * \return void
         */
        inline ~overlay() {
            for(auto & it : bmp_map) al_destroy_bitmap(it.second);
            al_destroy_bitmap(overlay_bitmap);
            al_destroy_font(overlay_font);
        };

        /*!
         * \brief Reload the overlay bitmap.
         * Called when the screen is updated.
         * \param void
         * \return void
         */
        inline void reload_overlay_bitmap(void) {
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            ALLEGRO_BITMAP* temp_bmp = al_clone_bitmap(overlay_bitmap);
            al_destroy_bitmap(overlay_bitmap);
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            overlay_bitmap = al_create_bitmap(overlay_w, overlay_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            al_set_target_bitmap(overlay_bitmap);
            al_draw_bitmap(temp_bmp, 0.0f, 0.0f, 0);
            al_destroy_bitmap(temp_bmp);
        };

        /*!
         * Set the font used by the overlay.
         * \param font Allegro font object to be used.
         * \return void
         */
        inline void set_font(ALLEGRO_FONT* font) { overlay_font = font; };

        /*!
         * Set text on the overlay.
         * \param txt Text to be displayed.
         * \param color Allegro color object.
         * \param x Horizontal location of the text.
         * \param y Verticle location of the text.
         * \param f Text flags for drawing - see Allegro docs on al_draw_text.
         * \return void
         */
        inline void set_text(const std::string txt, const ALLEGRO_COLOR color,
                             const float x, const float y, const int f) {
            al_draw_text(overlay_font, color, x, y, f, txt.c_str());
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

        ALLEGRO_BITMAP* overlay_bitmap;
        
        std::map<std::string, ALLEGRO_BITMAP*> bmp_map;

        float pos_x, pos_y;

    private:
        ALLEGRO_FONT* overlay_font;

        int overlay_w, overlay_h;
};

} //  namespace cmp

} //  namespace wte

#endif
