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
         * \brief 
         */
        template <typename T, typename... Args> inline static const bool load(const std::string& label, Args... args) {
            auto ret = _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), true)));
            return ret.second;
        };

        /*!
         * \brief 
         */
        static const bool unload(const std::string& label);

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
        assets();

        /*!
         * \brief 
         */
        ~assets();

        /*!
         * \brief 
         */
        template <typename T, typename... Args> inline static const bool secret_load(const std::string& label, Args... args) {
            auto ret = _assets.insert(std::make_pair(label, std::make_pair(std::make_shared<T>(args...), false)));
            return ret.second;
        };

        /*!
         * \brief 
         */
        static const bool secret_unload(const std::string& label);

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
        static void backup_bitmaps(void);

        /*!
         * \brief 
         */
        static void reload_bitmaps(void);

        static std::map<std::string, std::pair<std::shared_ptr<wte_asset>, bool>> _assets;
        static std::map<std::string, ALLEGRO_BITMAP*> _bitmaps_backup;
};

} //  namespace mgr

} //  namespace wte

#endif
