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
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/_globals/wte_exception.hpp"

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
         * \brief Load an asset.
         * \tparam T Asset type to add.
         * \tparam Args Parameters to asset constructor.
         * \param label Reference label for asset.
         * \param args Parameters to asset constructor.
         * \return True if loaded.  False if not.
         */
        template <typename T, typename... Args>
        inline static const bool load(
            const std::string& label, Args... args
        ) {
            auto ret =
                _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), true)));
            return ret.second;
        };

        /*!
         * \brief Unload an asset.
         * \param label Reference label for asset.
         * \return True if removed, false if not.
         */
        static const bool unload(
            const std::string& label
        );

        /*!
         * \brief Get an asset by reference label.
         * \param label Reference label for asset.
         * \tparam T Asset type to get.
         * \return Pointer to asset.
         * \exception Asset is protected.
         * \exception Asset not found.
         */
        template <typename T>
        inline static const std::shared_ptr<T> get(
            const std::string& label
        ) {
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
        assets();
        ~assets();

        /*!
         * \brief Load an asset.
         *
         * This creates a private asset only accessable by engine internals.
         *
         * \tparam T Asset type to add.
         * \tparam Args Parameters to asset constructor.
         * \param label Reference label for asset.
         * \param args Parameters to asset constructor.
         * \return True if loaded.  False if not.
         */
        template <typename T, typename... Args>
        inline static const bool secret_load(
            const std::string& label,
            Args... args
        ) {
            auto ret = _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), false)));
            return ret.second;
        };

        /*!
         * \brief Unload an asset.
         *
         * Allows removal of private assets.
         *
         * \param label Reference label for asset.
         * \return True if removed, false if not.
         */
        static const bool secret_unload(
            const std::string& label
        );

        /*!
         * \brief Get an asset by reference label.
         *
         * This allows access to private assets created ny secret_load.
         *
         * \param label Reference label for asset.
         * \tparam T Asset type to get.
         * \return Pointer to asset.
         * \exception Asset not found.
         */
        template <typename T>
        inline static const std::shared_ptr<T> secret_get(
            const std::string& label
        ) {
            try {
                return std::static_pointer_cast<T>(_assets.at(label).first);
            } catch(std::out_of_range& e) {
                const std::string err_msg = "Could not find asset: " + label;
                throw wte_exception(err_msg.c_str());
            }
        };

        /*
         * Backup temp bitmaps
         */
        static void backup_bitmaps(void);

        /*!
         * Restore temp bitmaps
         */
        static void reload_bitmaps(void);

        //  Store the asset map.
        static std::map<
            std::string,
            std::pair<
                std::shared_ptr<wte_asset>,
                bool
        >> _assets;

        //  Map for bitmap backup process.
        static std::map<
            std::string,
            ALLEGRO_BITMAP*
        > _bitmaps_backup;
};

} //  namespace mgr

} //  namespace wte

#endif
