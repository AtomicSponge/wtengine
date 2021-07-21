/*!
 * WTEngine | File:  wte_asset.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ASSET_HPP
#define WTE_ASSET_HPP

/*!
 * \class wte_asset
 * \brief asset
 */
namespace wte
{

class wte_asset {
    public:
        /*!
         * \brief
         */
        inline virtual ~wte_asset() {};

        //!  Remove copy constructor.
        inline wte_asset(const wte_asset&) = delete;
        //!  Remove assignment operator.
        inline void operator=(wte_asset const&) = delete;

    protected:
        /*!
         * \brief
         */
        inline wte_asset() {};
};

} //  end namespace wte

#endif
