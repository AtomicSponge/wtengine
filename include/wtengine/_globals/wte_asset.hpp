/*!
 * WTEngine | File:  wte_asset.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ASSET_HPP
#define WTE_ASSET_HPP

#include <memory>

namespace wte
{

/*!
 * \tparam T Asset type.
 */
template <typename T>
using wte_asset = std::shared_prt<T>;

}  //  end namespace wte

#endif
