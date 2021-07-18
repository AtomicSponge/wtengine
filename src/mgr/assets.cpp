/*!
 * WTEngine | File:  assets.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/assets.hpp"

namespace wte
{

namespace mgr
{

template <> bool assets::manager<assets>::initialized = false;

std::map<std::string, std::pair<std::any, bool>> assets::_assets = {};
std::map<std::string, al_bitmap> assets::_bitmaps_backup = {};

/*
 *
 */
assets::assets() { _assets.clear(); }

/*
 *
 */
assets::~assets() { _assets.clear(); }

/*
 *
 */
template <typename T> const bool assets::load(const std::string& label, const T& data) {
    auto ret = _assets.insert(std::make_pair(label, std::make_pair<std::make_any<T>(data), true>));
    return ret.second;
}

/*
 *
 */
const bool assets::unload(const std::string& label) {
    auto it = _assets.find(label);
    if(it != _assets.end() && it->second.second) {
        _assets.erase(it);
        return true;
    }
    return false;
}

/*
 *
 */
template <typename T> T assets::get(const std::string& label) {
    try {
        if(_assets.at(label).second) return _assets.at(label).first;
        const std::string err_msg = "Asset is protected: " + label;
        throw wte_exception(err_msg.c_str());
    } catch(std::out_of_range& e) {
        const std::string err_msg = "Could not find asset: " + label;
        throw wte_exception(err_msg.c_str());
    }
}

/*
 *
 */
template <typename T> const bool assets::secret_load(const std::string& label, const T& data) {
    auto ret = _assets.insert(std::make_pair(label, std::make_pair<std::make_any<T>(data), false>));
    return ret.second;
}

/*
 *
 */
const bool assets::secret_unload(const std::string& label) {
    auto it = _assets.find(label);
    if(it != _assets.end()) {
        _assets.erase(it);
        return true;
    }
    return false;
}

/*
 *
 */
template <typename T> T assets::secret_get(const std::string& label) {
    try {
        return _assets.at(label);
    } catch(std::out_of_range& e) {
        const std::string err_msg = "Could not find asset: " + label;
        throw wte_exception(err_msg.c_str());
    }
}

/*
 *
 */
void assets::backup_bitmaps(void) {
    /*_bitmaps_backup.clear();
    for (auto it = _assets.begin(); it != _assets.end();) {
        if(it->second.second) {
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _bitmaps_backup.insert(std::make_pair(it->first, al_clone_bitmap(it->second.first)));
            //  Now delete the old item.
            al_destroy_bitmap(it->second.first);
            it = _assets.erase(it);
        }
        else it++;
    }*/
}

/*
 *
 */
void assets::reload_bitmaps(void) {
    /*for (auto it = _bitmaps_backup.begin(); it != _bitmaps_backup.end();) {
        al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
        _assets.insert(std::make_pair(it->first, std::make_pair(al_clone_bitmap(it->second), true)));
        //  Now delete the old backup item.
        al_destroy_bitmap(it->second);
        it = _bitmaps_backup.erase(it);
    }
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _bitmaps_backup.clear();*/
}

} //  namespace mgr

} //  namespace wte
