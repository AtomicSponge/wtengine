/*!
 * wtengine | File:  wrappers.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/_globals/wrappers.hpp"

namespace wte {

/*
 *
 */
al_bitmap::al_bitmap() : _al_bitmap(NULL), nopreserve(false) {}

/*
 *
 */
al_bitmap::al_bitmap(const bool& p) : _al_bitmap(NULL), nopreserve(p) {}

/*
 *
 */
al_bitmap::al_bitmap(
    const int& w,
    const int& h
) : _al_bitmap(NULL), nopreserve(false) {
    assert(w > 0 && h > 0);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _al_bitmap = al_create_bitmap(w, h);
}

/*
 *
 */
al_bitmap::al_bitmap(
    const int& w,
    const int& h,
    const bool& p
) : _al_bitmap(NULL), nopreserve(p) {
    assert(w > 0 && h > 0);
    if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _al_bitmap = al_create_bitmap(w, h);
}

/*
 *
 */
const bool al_bitmap::isconverted(void) const { return nopreserve; }

/*
 *
 */
const bool al_bitmap::load(const std::string& fname) {
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
    return true;
}

/*
 *
 */
void al_bitmap::clone(ALLEGRO_BITMAP* temp_bmp) {
    if(nopreserve) al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    else al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _al_bitmap = al_clone_bitmap(temp_bmp);
}

/*
 *
 */
void al_bitmap::destroy(void) { al_destroy_bitmap(_al_bitmap); }

/*
 *
 */
const int al_bitmap::get_width(void) const { return al_get_bitmap_width(_al_bitmap); }

/*
 *
 */
const int al_bitmap::get_height(void) const { return al_get_bitmap_height(_al_bitmap); }

/*
 *
 */
al_font::al_font() {
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _al_font = al_create_builtin_font();
}

/*
 *
 */
al_font::al_font(
    const std::string& fname,
    const int& size,
    const int& flags
) {
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _al_font = al_load_font(fname.c_str(), size, flags);
}

/*
 *
 */
al_sample::al_sample(const std::string& fname) {
    _al_sample = al_load_sample(fname.c_str());
}

/*
 *
 */
al_audio::al_audio(const std::string& fname) {
    _al_audio = al_load_audio_stream(fname.c_str(), 4, 2048);
    al_set_audio_stream_playing(_al_audio, false);
}

/*
 *
 */
void al_audio::set_speed(const float& speed) { al_set_audio_stream_speed(_al_audio, speed); }

/*
 *
 */
const unsigned int al_audio::get_frequency(void) const { return al_get_audio_stream_frequency(_al_audio); }

/*
 *
 */
const double al_audio::get_length(void) const { return al_get_audio_stream_length_secs(_al_audio); }

/*
 *
 */
const float al_audio::get_speed(void) const { return al_get_audio_stream_speed(_al_audio); }

/*
 *
 */
const bool al_audio::is_playing(void) const { return al_get_audio_stream_playing(_al_audio); }

}  //  end namespace wte
