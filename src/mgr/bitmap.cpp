/*!
 * WTEngine | File:  bitmap.cpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/bitmap.hpp"

namespace wte
{

namespace mgr
{

template <> bool bitmap::manager<bitmap>::initialized = false;

/*
 *
 */
bitmap::bitmap() { _bitmaps.clear(); }

/*
 *
 */
bitmap::~bitmap() {
    for(auto & it : _bitmaps) al_destroy_bitmap(it.second.first);
    _bitmaps.clear();
}

/*
 *
 */
const bool bitmap::load(const std::string& fname, const std::string& label) {
    //  Load the file.
    ALLEGRO_FILE* file;
    file = al_fopen(fname.c_str(), "rb");
    if(!file) {  //  File not found, fail.
        alert::set("problem was here");
        al_fclose(file);
        return false;
    }

    //  Load bitmap into a temp pointer.
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
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
    auto ret = _bitmaps.insert(std::make_pair(label, std::make_pair(al_clone_bitmap(temp_bitmap), false)));
    al_destroy_bitmap(temp_bitmap);

    return ret.second;
}

/*
 *
 */
const bool bitmap::unload(const std::string& label) {
    auto it = _bitmaps.find(label);
    if(it != _bitmaps.end()) {
        al_destroy_bitmap(it->second.first);
        _bitmaps.erase(it);
        return true;
    }
    return false;
}

/*
 *
 */
ALLEGRO_BITMAP* bitmap::get(const std::string& label) {
    try {
        return _bitmaps.at(label).first;
    } catch(std::out_of_range& e) {
        std::string err_msg = "Could not find bitmap: " + label;
        throw wte_exception(err_msg.c_str());
    }
}

/*
 *
 */
const bool bitmap::create_bitmap(const std::string& label, const int w, const int h) {
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap(w, h);
    if(!temp_bitmap) return false;
    auto ret = _bitmaps.insert(std::make_pair(label, std::make_pair(al_clone_bitmap(temp_bitmap), true)));
    al_destroy_bitmap(temp_bitmap);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    if (!ret.second) return false;
    return true;
}

/*
 *
 */
void bitmap::reload(void) {
    for(auto & it : _bitmaps) {
        if(it.second.second) {
            int _width = al_get_bitmap_width(it.second.first);
            int _height = al_get_bitmap_height(it.second.first);
            al_destroy_bitmap(it.second.first);
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap(_width, _height);
            it.second.first = al_clone_bitmap(temp_bitmap);
            al_destroy_bitmap(temp_bitmap);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
        }
    }
}

} //  namespace mgr

} //  namespace wte
