/*!
 * WTEngine | File:  assets.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/assets.hpp"

namespace wte
{

namespace mgr
{

template <> bool assets::manager<assets>::initialized = false;

//std::map<std::string, ALLEGRO_BITMAP*> assets::_bitmaps_backup = {};

/*
 *
 */
/*void assets::backup_bitmaps(void) {
    _bitmaps_backup.clear();
    for (auto & it : _assets) {
        if(std::dynamic_pointer_cast<al_bitmap>(it.second)) {
            if(std::static_pointer_cast<al_bitmap>(it.second)->isconverted()) {
                //  Make a conversion safe copy in the backup map.
                al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
                _bitmaps_backup.insert(std::make_pair(it.first, al_clone_bitmap(**std::static_pointer_cast<al_bitmap>(it.second.first))));
                //  Now delete the old item.
                al_destroy_bitmap(**std::static_pointer_cast<al_bitmap>(it.second));
            }
        }
    }
}*/

/*
 *
 */
/*void assets::reload_bitmaps(void) {
    for (auto & it : _bitmaps_backup) {
        //  Restore bitmap.
        try {
            std::static_pointer_cast<al_bitmap>(_assets.at(it.first).first)->set(it.second);
        } catch(...) {}
        //  Now delete the old backup bitmap.
        al_destroy_bitmap(it.second);
    }
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    _bitmaps_backup.clear();
}*/

} //  namespace mgr

} //  namespace wte
