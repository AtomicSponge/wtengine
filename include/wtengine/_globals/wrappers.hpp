/*!
 * WTEngine | File:  wrappers.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 *
 * WIP
 */

#ifndef WTE_WRAPPERS_HPP
#define WTE_WRAPPERS_HPP

#include <string>
#include <cassert>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/wte_asset.hpp"

namespace wte
{

/*!
 *
 */
class al_bitmap : public wte_asset {
    friend class assets;

    public:
        /*!
         * \brief
         */
        inline al_bitmap() : _al_bitmap(NULL), nopreserve(false) {};

        /*!
         * \brief
         */
        inline al_bitmap(const bool& p) : _al_bitmap(NULL), nopreserve(p) {};

        /*!
         * \brief
         */
        inline al_bitmap(const int& w, const int& h) : _al_bitmap(NULL), nopreserve(false) {
            assert(w > 0 && h > 0);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*!
         * \brief
         */
        inline al_bitmap(const int& w, const int& h, const bool& p) : _al_bitmap(NULL), nopreserve(p) {
            assert(w > 0 && h > 0);
            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*!
         * \brief
         */
        //inline al_bitmap(const std::string& fname) : _al_bitmap(NULL), nopreserve(false) { load(fname); };

        /*!
         * \brief
         */
        //inline al_bitmap(const std::string& fname, const bool& p) : _al_bitmap(NULL), nopreserve(p) { load(fname); };

        /*!
         * \brief
         */
        inline ~al_bitmap() { al_destroy_bitmap(_al_bitmap); };

        inline ALLEGRO_BITMAP* operator*() { return _al_bitmap; };

        /*!
         * \brief
         */
        inline ALLEGRO_BITMAP* bitmap(void) { return _al_bitmap; };

        /*!
         * \brief
         */
        inline const bool isconverted(void) const { return nopreserve; };

        /*!
         * \brief
         */
        inline const bool load(const std::string& fname) {
            //  Load the file.
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {  //  File not found, fail.
                al_fclose(file);
                return false;
            }

            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap =
                al_load_bitmap_f(file, fname.substr(fname.find("."), fname.length()).c_str());
            al_fclose(file);

            if(!_al_bitmap) return false;  //  Bitmap not loaded, fail.

            #if WTE_USE_MAGIC_PINK
            //  Apply transparency if magic pink is enabled.
            al_convert_mask_to_alpha(_al_bitmap, WTE_MAGIC_PINK);
            #endif

            return true;
        };

        /*!
         * \brief
         */
        inline void set(ALLEGRO_BITMAP* temp_bmp) {
            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_clone_bitmap(temp_bmp);
        };

        /*!
         * \brief
         */
        inline const int get_width(void) const { return al_get_bitmap_width(_al_bitmap); };

        /*!
         * \brief
         */
        inline const int get_height(void) const { return al_get_bitmap_height(_al_bitmap); };

    private:
        ALLEGRO_BITMAP* _al_bitmap;
        bool nopreserve;
};

class al_font : public wte_asset {
    public:
        /*!
         *
         */
        inline al_font() {
            _al_font = NULL;
        };

        /*!
         *
         */
        inline al_font(const std::string& fname, const int& size, const int& flags) {
            if(fname == "default") _al_font = al_create_builtin_font();
            else _al_font = al_load_font(fname.c_str(), size, flags);
        };

        /*!
         *
         */
        inline ~al_font() { al_destroy_font(_al_font); };

        inline ALLEGRO_FONT* operator*() { return _al_font; };

        /*!
         *
         */
        inline ALLEGRO_FONT* font(void) { return _al_font; };

    private:
        ALLEGRO_FONT* _al_font;
};

}  //  end namespace wte

#endif
