/*!
 * WTEngine | File:  wrappers.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
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
 * \class al_bitmap
 * \brief Provides a wrapper to Allegro bitmaps.
 */
class al_bitmap final {
    public:
        /*!
         * \brief Creates a null bitmap that preserves its data.
         */
        inline al_bitmap() : _al_bitmap(NULL), nopreserve(false) {};

        /*!
         * \brief Set bitmap preservation.
         *
         * Set the nopreserve flag to true to create a bitmap that does not preserve its data
         * when the screen is reset.  This is to increase performance when drawing.
         * See Allegro docs on graphics routines for more info.
         *
         * \param p Preservation flag.
         */
        inline al_bitmap(
            const bool& p
        ) : _al_bitmap(NULL), nopreserve(p) {};

        /*!
         * \brief Creates a bitmap of w x h that preserves its data.
         * \param w Width in pixels.
         * \param h Hieght in pixels.
         */
        inline al_bitmap(
            const int& w,
            const int& h
        ) : _al_bitmap(NULL), nopreserve(false) {
            assert(w > 0 && h > 0);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*!
         * \brief Creates a bitmap of w x h and set the preservation flag.
         * \param w Width in pixels.
         * \param h Hieght in pixels.
         * \param p Preservation flag.
         */
        inline al_bitmap(
            const int& w,
            const int& h,
            const bool& p
        ) : _al_bitmap(NULL), nopreserve(p) {
            assert(w > 0 && h > 0);
            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_create_bitmap(w, h);
        };

        /*
         * having an issue with these
         */
        //inline al_bitmap(const std::string& fname) : _al_bitmap(NULL), nopreserve(false) { load(fname); };

        /*
         * having an issue with these
         */
        //inline al_bitmap(const std::string& fname, const bool& p) : _al_bitmap(NULL), nopreserve(p) { load(fname); };

        /*!
         * \brief Return a pointer to the internal Allegro bitmap.
         * \return Allegro bitmap pointer.
         */
        inline ~al_bitmap() {};

        inline ALLEGRO_BITMAP* operator*() { return _al_bitmap; };

        /*!
         * \brief Return a pointer to the internal Allegro bitmap.
         * \return Allegro bitmap pointer.
         */
        inline ALLEGRO_BITMAP* bitmap(void) { return _al_bitmap; };

        /*!
         * \brief Check if the bitmap should be converted when the screen is updated.
         * \return True if it should be converted, false if not.
         */
        inline const bool isconverted(void) const { return nopreserve; };

        /*!
         * \brief Load a bitmap from file.
         * \param fname Filename to load.
         * \return True if loaded, false on error.
         */
        inline const bool load(
            const std::string& fname
        ) {
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
         * \brief Set the bitmap from an existing one.
         * \param temp_bmp Bitmap to copy.
         */
        inline void set(
            ALLEGRO_BITMAP* temp_bmp
        ) {
            if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _al_bitmap = al_clone_bitmap(temp_bmp);
        };

        /*!
         * \brief Get bitmap width.
         * \return Bitmap width.
         */
        inline const int get_width(void) const { return al_get_bitmap_width(_al_bitmap); };

        /*!
         * \brief Get bitmap height.
         * \return Bitmap height.
         */
        inline const int get_height(void) const { return al_get_bitmap_height(_al_bitmap); };

    private:
        ALLEGRO_BITMAP* _al_bitmap; //  Internal Allegro bitmap.
        bool nopreserve;            //  Preservation flag.
};

/*!
 * \class al_font
 * \brief Provides a wrapper to Allegro fonts.
 */
class al_font final {
    public:
        /*!
         * \brief Create a font asset using Allegro's default font.
         */
        inline al_font() { _al_font = al_create_builtin_font(); };

        /*!
         * \brief Create a font asset from file.
         * \param fname Font file name.
         * \param size Font size.  See Allegro docs on fonts.
         * \param flags Font flags.  See Allegro docs on fonts.
         */
        inline al_font(
            const std::string& fname,
            const int& size,
            const int& flags
        ) { _al_font = al_load_font(fname.c_str(), size, flags); };

        /*!
         * \brief Delete font asset.
         */
        inline ~al_font() {};

        /*!
         * \brief Get font asset.
         * \return Pointer to font asset.
         */
        inline ALLEGRO_FONT* operator*() { return _al_font; };

        /*!
         * \brief Get font asset.
         * \return Pointer to font asset.
         */
        inline ALLEGRO_FONT* font(void) { return _al_font; };

    private:
        ALLEGRO_FONT* _al_font;
};

}  //  end namespace wte

#endif
