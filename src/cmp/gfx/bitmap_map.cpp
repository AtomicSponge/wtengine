/*!
 * WTEngine | File:  bitmap_map.cpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include <string>
#include <map>

#include <allegro5/allegro.h>

#include "wtengine/cmp/gfx/bitmap_map.hpp"
#include "wtengine/wte_global_defines.hpp"

namespace wte
{

namespace cmp
{

bitmap_map::~bitmap_map() {
    for(auto & it : bmp_map) al_destroy_bitmap(it.second);
    bmp_map.clear();
}

const bool bitmap_map::load_bitmap(const std::string& label,
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
}

const bool bitmap_map::delete_bitmap(const std::string& label) {
    auto it = bmp_map.find(label);
    if(it != bmp_map.end()) {
        al_destroy_bitmap(it->second);
        bmp_map.erase(it);
        return true;
    }
    return false;
}

void bitmap_map::draw_bitmap(
    const std::string& label,
    const float& dx, const float& dy,
    const int& flags
) {
    auto it = bmp_map.find(label);
    if(it != bmp_map.end()) al_draw_bitmap(it->second, dx, dy, flags);
}

void bitmap_map::draw_tinted_bitmap(
    const std::string& label,
    const ALLEGRO_COLOR& tint,
    const float& dx, const float& dy,
    const int& flags
) {
    auto it = bmp_map.find(label);
    if(it != bmp_map.end()) al_draw_tinted_bitmap(
        it->second, tint, dx, dy, flags
    );
}

void bitmap_map::draw_bitmap_region(
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
}

void bitmap_map::draw_tinted_bitmap_region(
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
}

void bitmap_map::draw_rotated_bitmap(
    const std::string& label,
    const float& cx, const float& cy,
    const float& dx, const float& dy,
    const float& angle, const int& flags
) {
    auto it = bmp_map.find(label);
    if(it != bmp_map.end()) al_draw_rotated_bitmap(
        it->second, cx, cy, dx, dy, angle, flags
    );
}

void bitmap_map::draw_tinted_rotated_bitmap(
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
}

void bitmap_map::draw_scaled_rotated_bitmap(
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
}

void bitmap_map::draw_tinted_scaled_rotated_bitmap(
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
}

void bitmap_map::draw_tinted_scaled_rotated_bitmap_region(
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
}

void bitmap_map::draw_scaled_bitmap(
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
}

void bitmap_map::draw_tinted_scaled_bitmap(
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
}

bitmap_map::bitmap_map() {
    bmp_map.clear();
}

} //  namespace cmp

} //  namespace wte
