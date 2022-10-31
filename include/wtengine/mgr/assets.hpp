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
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"

namespace wte::mgr {

/*!
 * \class assets
 * \brief Stores an index of assets.
 * \tparam ...Types Types of assets used in game code.
 */
template <typename... Types>
class assets final : private manager<assets<>> {
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
            return load_impl<T, 0, Types...>::load(label, obj);
        };

        /*!
         * \brief Load an asset using a new object.
         * \tparam T Asset type to add.
         * \param label Reference label for asset.
         * \param obj Asset to add.
         * \return True if loaded.  False if not.
         */
        template <typename T>
        inline static bool load(
            const std::string& label,
            const T& obj
        ) {
            return load_impl<T, 0, Types...>::load(label, make_asset(obj));
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
            return unload_impl<T, 0, Types...>::unload(label);
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
                return get_impl<T, 0, Types...>::get(label);
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

        /*
         * The template specializations below generate a tuple of maps
         * for each asset type found in the game code.
         *
         * First template does compile-time matching for asset load/unload/get implementations.
         * Second template is the actual specialization of each member.
         */
        template <typename T, size_t idx, typename U, typename... Ts>
        struct load_impl {
            inline static bool load(
                const std::string& label,
                const wte_asset<T>& obj
            ) {
                static_assert((sizeof ...(Ts)) > 0, "Asset template type error.");
                return load_impl<T, idx + 1, Ts&&...>::load(label, obj);
            }
        };

        template <typename T, size_t idx, typename... Ts>
        struct load_impl<T, idx, T, Ts...> {
            inline static bool load(
                const std::string& label,
                const wte_asset<T>& obj
            ) {
                auto ret =
                    std::get<idx>(assets::_assets).insert(std::make_pair(label, obj));
                return ret.second;
            }
        };

        template <typename T, size_t idx, typename U, typename... Ts>
        struct unload_impl {
            inline static bool unload(
                const std::string& label
            ) {
                static_assert((sizeof ...(Ts)) > 0, "Asset template type error.");
                return unload_impl<T, idx + 1, Ts&&...>::unload(label);
            }
        };

        template <typename T, size_t idx, typename... Ts>
        struct unload_impl<T, idx, T, Ts...> {
            inline static bool unload(
                const std::string& label
            ) {
                auto it = std::get<idx>(assets::_assets).find(label);
                if(it != std::get<idx>(assets::_assets).end()) {
                    std::get<idx>(assets::_assets).erase(it);
                    return true;
                }
                return false;
            }
        };

        template <typename T, size_t idx, typename U, typename... Ts>
        struct get_impl {
            inline static const wte_asset<T> get(
                const std::string& label
            ) {
                static_assert((sizeof ...(Ts)) > 0, "Asset template type error.");
                try {
                    return get_impl<T, idx + 1, Ts&&...>::get(label);
                } catch(...) { throw; }
            }
        };

        template <typename T, size_t idx, typename... Ts>
        struct get_impl<T, idx, T, Ts...> {
            inline static const wte_asset<T> get(
                const std::string& label
            ) {
                try {
                    return std::get<idx>(assets::_assets).at(label);
                } catch(...) { throw; }
            }
        };

        //  Store the asset map.
        inline static std::tuple<
            std::map<
                const std::string,
                wte_asset<Types>
            >
        ...> _assets;
};

template <> inline bool manager<assets<>>::initialized = false;

}  //  end namespace wte::mgr

#endif
