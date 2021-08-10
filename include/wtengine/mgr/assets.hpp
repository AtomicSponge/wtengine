/*!
 * WTEngine | File:  assets.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_ASSETS_HPP
#define WTE_MGR_ASSETS_HPP

#include <string>
#include <map>
#include <utility>
#include <memory>

#include <allegro5/allegro.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

class engine;

namespace mgr
{

/*!
 * \class assets
 * \brief Stores an index of assets.
 */
class assets final : private manager<assets> {
    friend class wte::engine;

    public:
        /*!
         * \brief Load an asset.
         * \tparam T Asset type to add.
         * \tparam Args Parameters to asset constructor.
         * \param label Reference label for asset.
         * \param args Parameters to asset constructor.
         * \return True if loaded.  False if not.
         */
        template <typename T>
        inline static const bool load(
            const std::string& label,
            const T& asset
        ) {
            auto ret =
                _assets<T>.insert(std::make_pair(label, wte_asset<T>(asset)));
            return ret.second;
        };

        /*!
         * \brief Unload an asset.
         * \param label Reference label for asset.
         * \return True if removed, false if not.
         */
        template <typename T>
        inline static const bool unload(
            const std::string& label
        ) {
            auto it = _assets<T>.find(label);
            if(it != _assets<T>.end()) {
                _assets<T>.erase(it);
                return true;
            }
            return false;
        };

        /*!
         * \brief Get an asset by reference label.
         * \param label Reference label for asset.
         * \tparam T Asset type to get.
         * \return Pointer to asset.
         * \exception Asset not found.
         */
        template <typename T>
        inline static const wte_asset<T> get(
            const std::string& label
        ) {
            try {
                auto res = _assets<T>.at(label);
                return res;
            } catch(std::out_of_range& e) {
                const std::string err_msg = "Could not find asset: " + label;
                throw wte_exception(err_msg.c_str(), "assets", engine_time::check_time());
            }
        };

    private:
        assets();
        ~assets();

        /*
         * Backup temp bitmaps
         */
        static void backup_bitmaps(void);

        /*!
         * Restore temp bitmaps
         */
        static void reload_bitmaps(void);

        //  Store the asset map.
        template <typename T>
        inline static std::map<
            std::string,
            wte_asset<T>
        > _assets = {};

        //  Map for bitmap backup process.
        static std::map<
            std::string,
            ALLEGRO_BITMAP*
        > _bitmaps_backup;
};

} //  namespace mgr

} //  namespace wte

#endif
