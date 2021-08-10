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
 * \class wte_asset
 * \brief Interface class for creating an asset.
 */
template <class T> class wte_asset {
    public:
        /*!
         * \brief
         * \param args
         */
        template <typename... Args>
        inline wte_asset(Args... args) {
            internal_data = std::make_shared<T>(args...);
        };

        inline ~wte_asset() {};

        /*!
         * \brief
         * \param a
         */
        inline wte_asset(const wte_asset<T>& a) { internal_data = a.internal_data; };

        /*!
         * \brief
         * \param a
         */
        inline void operator=(wte_asset<T> const& a) { internal_data = a.internal_data; };

        /*!
         * \brief
         * \return
         */
        inline T* get() const noexcept { return internal_data.get(); };
        
        /*!
         * \brief
         * \return
         */
        inline T& operator*() const noexcept { return *internal_data; };

        /*!
         * \brief
         * \return
         */
        inline T* operator->() const noexcept { return *internal_data; };

    private:
        std::shared_ptr<T> internal_data;
};

}  //  end namespace wte

#endif
