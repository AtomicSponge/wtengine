/*!
 * WTEngine | File:  bitmap.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_BITMAP_HPP
#define WTE_MGR_BITMAP_HPP

#include <string>
#include <map>
#include <utility>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/manager.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class bitmap
 * \brief wip
 */
class bitmap final : private manager<bitmap> {
    friend class interface;

    public:
        /*!
         * \brief 
         */
        static const bool load(const std::string& fname, const std::string& label);

        /*!
         * \brief 
         */
        static const bool unload(const std::string& label);

        /*!
         * \brief 
         */
        static ALLEGRO_BITMAP* get(const std::string& label);

        /*!
         * \brief 
         */
        static const bool create_bitmap(const std::string& label, const int w, const int h);

    private:
        /*!
         * \brief constructor.
         */
        bitmap();

        /*!
         * \brief destructor.
         */
        ~bitmap();

        /*!
         * \brief 
         */
        static void reload(void);

        inline static std::map<std::string, std::pair<ALLEGRO_BITMAP*, bool>> _bitmaps = {};
};

} //  namespace mgr

} //  namespace wte

#endif
