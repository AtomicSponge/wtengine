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

#include "component.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class sample_loop
 * \brief Handle sample looping.
 */
class sample_loop final : public component {
    public:
        /*!
         * Sample loop constructor.
         */
        inline sample_loop() {
            instance_map.clear();
        };

        /*!
         * Sample loop destructor.
         */
        inline ~sample_loop() {
            instance_map.clear();
        };

        /*!
         * Add handle.
         * \param sn Sample name reference.
         * \param si Sample instance reference.
         * \return True if added, false if not.
         */
        inline const bool add_handle(const std::string& sn, const std::string& si) {
            if(sn == "all") return false;
            if(si == "once") return false;
            std::string sample_name;
            if(sn.find(".") == std::string::npos) sample_name = sn;
            else sample_name = sn.substr(0, sn.find("."));
            auto ret = instance_map.insert(std::make_pair(si, std::make_pair(sample_name, false)));
            return ret.second;
        };

        /*!
         * Start sample loop.
         * \param messages Reference to message manager.
         * \param si Sample instance reference.
         */
        inline void start(mgr::message_manager& messages, const std::string& si) {
            auto ref = instance_map.find(si);
            if(ref != instance_map.end()) {
                if(ref->second.second == false) {  //  If not playing.
                    ref->second.second = true;  //  Set is playing.
                    messages.add_message(message("audio", "play_sample", ref->second.first + ";" + si));
                }
            }
        };

        /*!
         * Stop sample loop.
         * \param messages Reference to message manager.
         * \param si Sample instance reference.
         */
        inline void stop(mgr::message_manager& messages, const std::string& si) {
            auto ref = instance_map.find(si);
            if(ref != instance_map.end()) {
                if(ref->second.second == true) {  //  If is playing.
                    ref->second.second = false;  //  Set not playing.
                    messages.add_message(message("audio", "stop_sample", si));
                }
            }
        };

    private:
        //  Key:  Instance name | sample name, is playing
        std::map<std::string, std::pair<std::string, bool>> instance_map;
};

} //  namespace cmp

} //  namespace wte

#endif
