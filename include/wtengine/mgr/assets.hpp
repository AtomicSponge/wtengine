/*!
 * WTEngine | File:  assets.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_ASSETS_HPP
#define WTE_MGR_ASSETS_HPP

#include <string>
#include <map>
#include <any>
#include <utility>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wrappers.hpp"
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
        template <typename T> static const bool load(const std::string& label, const T& data);

        /*!
         * \brief 
         */
        static const bool unload(const std::string& label);

        /*!
         * \brief 
         */
        template <typename T> static T get(const std::string& label);

    private:
        /*!
         * \brief constructor.
         */
        assets();

        /*!
         * \brief destructor.
         */
        ~assets();

        /*!
         * \brief 
         */
        template <typename T> static const bool secret_load(const std::string& label, const T& data);

        /*!
         * \brief 
         */
        static const bool secret_unload(const std::string& label);

        /*!
         * \brief 
         */
        template <typename T> static T secret_get(const std::string& label);

        /*!
         * \brief 
         */
        static void backup_bitmaps(void);

        /*!
         * \brief 
         */
        static void reload_bitmaps(void);

        static std::map<std::string, std::pair<std::any, bool>> _assets;
        static std::map<std::string, al_bitmap> _bitmaps_backup;
};

} //  namespace mgr

} //  namespace wte

#endif
