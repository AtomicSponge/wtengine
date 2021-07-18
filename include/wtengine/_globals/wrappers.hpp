/*!
 * WTEngine | File:  wrappers.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_WRAPPERS_HPP
#define WTE_WRAPPERS_HPP

#include <string>
#include <cassert>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "wtengine/_globals/_defines.hpp"

namespace wte
{

/*!
 *
 */
class al_bitmap {
    public:
        /*!
         *
         */
        inline al_bitmap() : nopreserve(false) {
            _al_bitmap = NULL;
        };

        /*!
         *
         */
        inline al_bitmap(const bool& p) : nopreserve(p) {
            _al_bitmap = NULL;
        };

        /*!
         *
         */
        inline al_bitmap(const int& w, const int& h) : nopreserve(false) {
            assert(w > 0 && h > 0);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*!
         *
         */
        inline al_bitmap(const int& w, const int& h, const bool& p) : nopreserve(p) {
            assert(w > 0 && h > 0);
            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*!
         *
         */
        inline al_bitmap(const std::string& fname) : nopreserve(false) {
            load(fname);
        };

        /*!
         *
         */
        inline al_bitmap(const std::string& fname, const bool& p) : nopreserve(p) {
            load(fname);
        };

        inline al_bitmap(const al_bitmap& b) {
            if(b.nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_clone_bitmap(b._al_bitmap);
            nopreserve = b.nopreserve;
        };

        /*!
         *
         */
        inline ~al_bitmap() {
            al_destroy_bitmap(_al_bitmap);
        };

        /*!
         *
         */
        /*inline al_bitmap operator=(const al_bitmap& b) {
            al_bitmap temp;
            if(b.nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            temp._al_bitmap = al_clone_bitmap(b._al_bitmap);
            temp.nopreserve = b.nopreserve;
            return temp;
        };*/

        /*inline ALLEGRO_BITMAP* operator*() {
            return _al_bitmap;
        };*/

        /*!
         *
         */
        inline ALLEGRO_BITMAP* bitmap(void) {
            return _al_bitmap;
        };

        /*!
         *
         */
        inline const bool isconverted(void) const {
            return !nopreserve;
        }

        /*!
         *
         */
        inline const bool load(const std::string& fname) {
            //  Load the file.
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {  //  File not found, fail.
                al_fclose(file);
                return false;
            }

            //  Load bitmap into a temp pointer.
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

    private:
        ALLEGRO_BITMAP* _al_bitmap;
        bool nopreserve;
};

class al_font {
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
        inline ~al_font() {
            al_destroy_font(_al_font);
        };

        inline ALLEGRO_FONT* operator*() {
            return _al_font;
        };

        /*!
         *
         */
        inline ALLEGRO_FONT* font(void) {
            return _al_font;
        };

    private:
        ALLEGRO_FONT* _al_font;
};

}  //  end namespace wte

#endif
