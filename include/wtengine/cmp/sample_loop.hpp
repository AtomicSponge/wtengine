/*!
 * WTEngine | File:  sample_loop.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_SAMPLE_LOOP_HPP
#define WTE_CMP_SAMPLE_LOOP_HPP

#include <string>
#include <map>
#include <utility>

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class sample_loop
 * \brief Handle playback sample looping.
 */
class sample_loop final : public component {
    public:
        /*!
         * \brief Sample loop constructor.
         */
        sample_loop();

        /*!
         * \brief Sample loop destructor.
         */
        ~sample_loop();

        /*!
         * \brief Add handle.
         * 
         * \param sn Sample name reference.
         * \param si Sample instance reference.
         * \return True if added, false if not.
         */
        const bool add_handle(const std::string& sn, const std::string& si);

        /*!
         * \brief Start sample loop.
         * 
         * \param messages Reference to message manager.
         * \param si Sample instance reference.
         */
        void start(mgr::message_manager& messages, const std::string& si);

        /*!
         * \brief Stop sample loop.
         * 
         * \param messages Reference to message manager.
         * \param si Sample instance reference.
         */
        void stop(mgr::message_manager& messages, const std::string& si);

    private:
        //  Key:  Instance name | sample name, is playing
        std::map<std::string, std::pair<std::string, bool>> instance_map;
};

} //  namespace cmp

} //  namespace wte

#endif
