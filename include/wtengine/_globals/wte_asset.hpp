/*!
 * wtengine | File:  wte_asset.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_ASSET_HPP)
#define WTE_ASSET_HPP

#include <string>
#include <memory>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>

namespace wte {

/*!
 * \tparam T Asset type.
 */
template <typename T>
using wte_asset = std::shared_ptr<T>;

/*!
 * \brief Create an asset (shared) pointer.
 * \tparam T Asset type.
 * \param obj Object to create asset from.
 * \return A new asset (shared pointer).
 */
template <typename ALLEGRO_BITMAP>
inline static const wte_asset<ALLEGRO_BITMAP> make_asset(const int& w, const int& h) {
    std::shared_ptr<ALLEGRO_BITMAP> temp_ptr(al_create_bitmap(w, h), al_destroy_bitmap);
    return temp_ptr;
};

template <typename ALLEGRO_FONT>
inline static const wte_asset<ALLEGRO_FONT> make_asset() {
    std::shared_ptr<ALLEGRO_FONT> temp_ptr(al_create_builtin_font(), al_destroy_font);
    return temp_ptr;
};

template <typename T>
inline static const wte_asset<T> make_asset(
    const std::string& fname, const int& dx, const int& dy
) {
    if constexpr (std::is_same_v<T, ALLEGRO_FONT>) {
        std::shared_ptr<ALLEGRO_FONT> temp_ptr(al_load_font(fname.c_str(), dx, dy), al_destroy_font);
        return temp_ptr;
    }

    if constexpr (std::is_same_v<T, ALLEGRO_AUDIO_STREAM>) {
        std::shared_ptr<T> temp_ptr(al_load_audio_stream(fname.c_str(), dx, dy), al_destroy_audio_stream);
        al_set_audio_stream_playing(temp_ptr.get(), false);
        return temp_ptr;
    }
};

template <typename T>
inline static const wte_asset<T> make_asset(const std::string& fname) {
    if constexpr (std::is_same_v<T, ALLEGRO_BITMAP>) {
        std::shared_ptr<ALLEGRO_BITMAP> temp_ptr(al_load_bitmap(fname.c_str()), al_destroy_bitmap);
        return temp_ptr;
    }

    if constexpr (std::is_same_v<T, ALLEGRO_SAMPLE>) {
        std::shared_ptr<T> temp_ptr(al_load_sample(fname.c_str()), al_destroy_sample);
        return temp_ptr;
    }

    if constexpr (std::is_same_v<T, ALLEGRO_AUDIO_STREAM>) {
        std::shared_ptr<T> temp_ptr(al_load_audio_stream(fname.c_str(), 4, 2048), al_destroy_audio_stream);
        al_set_audio_stream_playing(temp_ptr.get(), false);
        return temp_ptr;
    }
};

}  //  end namespace wte

#endif
