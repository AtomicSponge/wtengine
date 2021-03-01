/*!
 * WTEngine | File:  bitmap_map.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_BITMAP_MAP_HPP
#define WTE_CMP_BITMAP_MAP_HPP

#include <string>
#include <map>

#include <allegro5/allegro.h>

#include "wtengine/wte_global_defines.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class bitmap_map
 * \brief Stores a map of bitmaps for drawing onto the component.
 * 
 * Extend this to add functionality to a component.
 * Also includes wrappers to the Allegro bitmap drawing functions.
 */
class bitmap_map {
    public:
        /*!
         * \brief Bitmap map destructor.
         */
        inline virtual ~bitmap_map() {
            for(auto & it : bmp_map) al_destroy_bitmap(it.second);
            bmp_map.clear();
        };

        /*!
         * \brief Load a bitmap.
         * 
         * Store in a map for reference to later.  Call this during entity creation.
         * 
         * \param fname Filename to load.
         * \param label Label for referencing bitmap.
         * \return True if loaded, false if not.
         */
        inline const bool load_bitmap(const std::string& label,
                                      const std::string& fname) {
            //  Load the file.
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {  //  File not found, fail.
                al_fclose(file);
                return false;
            }

            //  Load bitmap into a temp pointer.
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            ALLEGRO_BITMAP* temp_bitmap =
                al_load_bitmap_f(file, fname.substr(fname.find("."), fname.length()).c_str());
            al_fclose(file);

            if(!temp_bitmap) return false;  //  Bitmap not loaded, fail.

            #if WTE_USE_MAGIC_PINK
            //  Apply transparency if magic pink is enabled.
            al_convert_mask_to_alpha(temp_bitmap, WTE_MAGIC_PINK);
            #endif

            //  Store the bitmap.
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            auto ret = bmp_map.insert(std::make_pair(label, al_clone_bitmap(temp_bitmap)));
            al_destroy_bitmap(temp_bitmap);

            return ret.second;
        };

        /*!
         * \brief Delete a bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \return True if removed, false if not.
         */
        inline const bool delete_bitmap(const std::string& label) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) {
                al_destroy_bitmap(it->second);
                bmp_map.erase(it);
                return true;
            }
            return false;
        };

        /*!
         * \brief Draw a bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_bitmap(
            const std::string& label,
            const float& dx, const float& dy,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_bitmap(it->second, dx, dy, flags);
        };

        /*!
         * \brief Draw a tinted bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param tint Allegro Color of tint.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_bitmap(
            const std::string& label,
            const ALLEGRO_COLOR& tint,
            const float& dx, const float& dy,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_bitmap(
                it->second, tint, dx, dy, flags
            );
        };

        /*!
         * \brief Draw a region of a bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param sx Source X.
         * \param sy Source Y.
         * \param sw Source width.
         * \param sh Source height.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_bitmap_region(
            const std::string& label,
            const float& sx, const float& sy,
            const float& sw, const float& sh,
            const float& dx, const float& dy,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_bitmap_region(
                it->second, sx, sy, sw, sh, dx, dy, flags
            );
        };

        /*!
         * \brief Draw a tinted region of a bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param tint Allegro Color of tint.
         * \param sx Source X.
         * \param sy Source Y.
         * \param sw Source width.
         * \param sh Source height.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_bitmap_region(
            const std::string& label,
            const ALLEGRO_COLOR& tint,
            const float& sx, const float& sy,
            const float& sw, const float& sh,
            const float& dx, const float& dy,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_bitmap_region(
                it->second, tint, sx, sy, sw, sh, dx, dy, flags
            );
        };

        /*!
         * \brief Draw a rotated bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param cx Center X of bitmap for rotation.
         * \param cy Center Y of bitmap for rotation.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param angle Angle of rotation in radians.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_rotated_bitmap(
            const std::string& label,
            const float& cx, const float& cy,
            const float& dx, const float& dy,
            const float& angle, const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_rotated_bitmap(
                it->second, cx, cy, dx, dy, angle, flags
            );
        };

        /*!
         * \brief Draw a tinted rotated bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param tint Allegro Color of tint.
         * \param cx Center X of bitmap for rotation.
         * \param cy Center Y of bitmap for rotation.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param angle Angle of rotation in radians.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_rotated_bitmap(
            const std::string& label,
            const ALLEGRO_COLOR& tint,
            const float& cx, const float& cy,
            const float& dx, const float& dy,
            const float& angle, const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_rotated_bitmap(
                it->second, tint, cx, cy, dx, dy, angle, flags
            );
        };

        /*!
         * \brief Draw a scaled rotated bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param cx Center X of bitmap for rotation.
         * \param cy Center Y of bitmap for rotation.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param xscale X scale factor.
         * \param yscale Y scale factor.
         * \param angle Angle of rotation in radians.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_scaled_rotated_bitmap(
            const std::string& label,
            const float& cx, const float& cy,
            const float& dx, const float& dy,
            const float& xscale, const float& yscale,
            const float& angle, const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_scaled_rotated_bitmap(
                it->second, cx, cy, dx, dy, xscale, yscale, angle, flags
            );
        };

        /*!
         * \brief Draw a tinted scaled rotated bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param tint Allegro Color of tint.
         * \param cx Center X of bitmap for rotation.
         * \param cy Center Y of bitmap for rotation.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param xscale X scale factor.
         * \param yscale Y scale factor.
         * \param angle Angle of rotation in radians.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_scaled_rotated_bitmap(
            const std::string& label,
            const ALLEGRO_COLOR& tint,
            const float& cx, const float& cy,
            const float& dx, const float& dy,
            const float& xscale, const float& yscale,
            const float& angle, const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_scaled_rotated_bitmap(
                it->second, tint, cx, cy, dx, dy, xscale, yscale, angle, flags
            );
        };

        /*!
         * \brief Draw a tinted scaled rotated bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param sx Source X.
         * \param sy Source Y.
         * \param sw Source width.
         * \param sh Source height.
         * \param tint Allegro Color of tint.
         * \param cx Center X of bitmap for rotation.
         * \param cy Center Y of bitmap for rotation.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param xscale X scale factor.
         * \param yscale Y scale factor.
         * \param angle Angle of rotation in radians.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_scaled_rotated_bitmap_region(
            const std::string& label,
            const float& sx, const float& sy,
            const float& sw, const float& sh,
            const ALLEGRO_COLOR& tint,
            const float& cx, const float& cy,
            const float& dx, const float& dy,
            const float& xscale, const float& yscale,
            const float& angle, const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_scaled_rotated_bitmap_region(
                it->second, sx, sy, sw, sh, tint, cx, cy, dx, dy, xscale, yscale, angle, flags
            );
        };

        /*!
         * \brief Draw a scaled bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param sx Source X.
         * \param sy Source Y.
         * \param sw Source width.
         * \param sh Source height.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param dw Destination width.
         * \param dh Destination height.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_scaled_bitmap(
            const std::string& label,
            const float& sx, const float& sy,
            const float& sw, const float& sh,
            const float& dx, const float& dy,
            const float& dw, const float& dh,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_scaled_bitmap(
                it->second, sx, sy, sw, sh, dx, dy, dw, dh, flags
            );
        };

        /*!
         * \brief Draw a tinted scaled bitmap from the map.
         * 
         * \param label Label of saved bitmap.
         * \param tint Allegro Color of tint.
         * \param sx Source X.
         * \param sy Source Y.
         * \param sw Source width.
         * \param sh Source height.
         * \param dx X location of the background to draw to.
         * \param dy Y location of the background to draw to.
         * \param dw Destination width.
         * \param dh Destination height.
         * \param flags Allegro Bitmap flags.
         */
        inline void draw_tinted_scaled_bitmap(
            const std::string& label,
            const ALLEGRO_COLOR& tint,
            const float& sx, const float& sy,
            const float& sw, const float& sh,
            const float& dx, const float& dy,
            const float& dw, const float& dh,
            const int& flags
        ) {
            auto it = bmp_map.find(label);
            if(it != bmp_map.end()) al_draw_tinted_scaled_bitmap(
                it->second, tint, sx, sy, sw, sh, dx, dy, dw, dh, flags
            );
        };

    protected:
        /*!
         * \brief Bitmap map constructor.
         */
        inline bitmap_map() {
            bmp_map.clear();
        };

    private:
        std::map<std::string, ALLEGRO_BITMAP*> bmp_map;
};

} //  namespace cmp

} //  namespace wte

#endif
