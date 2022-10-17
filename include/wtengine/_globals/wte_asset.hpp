/*!
 * wtengine | File:  wte_asset.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_ASSET_HPP)
#define WTE_ASSET_HPP

#include <memory>

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
template <typename T>
inline static const wte_asset<T> make_asset(const T& obj) {
    return std::make_shared<T>(obj);
};

}  //  end namespace wte

#endif
