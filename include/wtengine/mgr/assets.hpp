/*!
 * wtengine | File:  assets.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_MGR_ASSETS_HPP)
#define WTE_MGR_ASSETS_HPP

#include <string>
#include <tuple>
#include <map>
#include <exception>

#include <allegro5/allegro.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_asset.hpp"

namespace wte {
    class engine;
}

namespace wte::mgr {

template <typename T>
using asset_map = std::map<const std::string, wte_asset<T>>;

/*!
 * \class assets
 * \brief Stores an index of assets.
 * \tparam ...Types Types of assets used in game code.
 */
class assets final : private manager<assets> {
    friend class wte::engine;

    public:
        /*!
         * \brief Load an existing asset.
         * \tparam T Asset type to add.
         * \param label Reference label for asset.
         * \param obj Asset to add.
         * \return True if loaded.  False if not.
         */
        template <typename T>
        inline static bool load(
            const std::string& label,
            const wte_asset<T> obj
        ) {
            auto ret = _assets<T>.insert(std::make_pair(label, std::move(obj)));
            return ret.second;
        };

        /*!
         * \brief Unload an asset.
         * \tparam T Asset type to unload.
         * \param label Reference label for asset.
         * \return True if removed, false if not.
         */
        template <typename T>
        inline static bool unload(
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
         * \tparam T Asset type to get.
         * \param label Reference label for asset.
         * \return Shared pointer to asset.
         * \exception Asset not found.
         */
        template <typename T>
        inline static const wte_asset<T> get(
            const std::string& label
        ) {
            try {
                return _assets<T>.at(label);
            } catch(std::out_of_range& e) {
                std::string err_msg = "Could not get asset: " + label;
                throw engine_exception(err_msg, "Assets", 4);
            } catch(...) {
                std::string err_msg = "Error reading asset: " + label;
                throw engine_exception(err_msg, "Assets", 4);
            }
        };

    private:
        assets() = default;
        ~assets() = default;

        //  Engine calls this during de-init to clean up.
        inline static void clear_al_objects(void) {
            _assets<ALLEGRO_BITMAP>.clear();
            _assets<ALLEGRO_FONT>.clear();
            _assets<ALLEGRO_SAMPLE>.clear();
            _assets<ALLEGRO_AUDIO_STREAM>.clear();
        };

        //  Store each asset map in a tuple.
        template <typename T>
        inline static asset_map<T> _assets;
};

template <> inline bool manager<assets>::initialized = false;

}  //  end namespace wte::mgr

#endif
