/*!
 * WTEngine | File:  assets.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 *
 * WIP
 */

#ifndef WTE_MGR_ASSETS_HPP
#define WTE_MGR_ASSETS_HPP

#include <string>
#include <map>
#include <utility>
#include <memory>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/manager.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class assets
 * \brief Stores an index of assets.
 */
class assets final : private manager<assets> {
    friend class interface;
    friend class renderer;
    friend class menus;

    public:
        /*!
         * \brief 
         */
        template <typename T, typename... Args> inline static const bool load(const std::string& label, Args... args) {
            auto ret = _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), true)));
            return ret.second;
            return false;
        };

        /*!
         * \brief 
         */
        inline static const bool unload(const std::string& label) {
            auto it = _assets.find(label);
            if(it != _assets.end() && it->second.second) {
                _assets.erase(it);
                return true;
            }
            return false;
        };

        /*!
         * \brief 
         */
        template <typename T> inline static const std::shared_ptr<T> get(const std::string& label) {
            try {
                auto res = _assets.at(label);
                if(res.second) return std::static_pointer_cast<T>(res.first);
                const std::string err_msg = "Asset is protected: " + label;
                throw wte_exception(err_msg.c_str());
            } catch(std::out_of_range& e) {
                const std::string err_msg = "Could not find asset: " + label;
                throw wte_exception(err_msg.c_str());
            }
        };

    private:
        /*!
         * \brief 
         */
        inline assets() { _assets.clear(); };

        /*!
         * \brief 
         */
        inline ~assets() { _assets.clear(); };

        /*!
         * \brief 
         */
        template <typename T, typename... Args> inline static const bool secret_load(const std::string& label, Args... args) {
            auto ret = _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), false)));
            return ret.second;
            return false;
        };

        /*!
         * \brief 
         */
        inline static const bool secret_unload(const std::string& label) {
            auto it = _assets.find(label);
            if(it != _assets.end()) {
                _assets.erase(it);
                return true;
            }
            return false;
        };

        /*!
         * \brief 
         */
        template <typename T> inline static const std::shared_ptr<T> secret_get(const std::string& label) {
            try {
                return std::static_pointer_cast<T>(_assets.at(label).first);
            } catch(std::out_of_range& e) {
                const std::string err_msg = "Could not find asset: " + label;
                throw wte_exception(err_msg.c_str());
            }
        };

        /*!
         * \brief 
         */
        inline static void backup_bitmaps(void) {
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
        };

        /*!
         * \brief 
         */
        inline static void reload_bitmaps(void) {
            /*for (auto it = _bitmaps_backup.begin(); it != _bitmaps_backup.end();) {
                al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
                _assets.insert(std::make_pair(it->first, std::make_pair(al_clone_bitmap(it->second), true)));
                //  Now delete the old backup item.
                al_destroy_bitmap(it->second);
                it = _bitmaps_backup.erase(it);
            }
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            _bitmaps_backup.clear();*/
        };

        inline static std::map<std::string, std::pair<std::shared_ptr<wte_asset>, bool>> _assets = {};
        inline static std::map<std::string, al_bitmap> _bitmaps_backup = {};
};

template <> inline bool assets::manager<assets>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
