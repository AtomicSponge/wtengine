/*!
 * \brief WTEngine | File:  sample_loop.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Sample loop component.
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

//! Sample loop component
/*!
 * Handle sample looping.
 */
class sample_loop final : public component {
    public:
        /*!
         * Sample loop constructor.
         * \param void
         * \return void
         */
        inline sample_loop() {
            instance_map.clear();
        };

        /*!
         * Sample loop destructor.
         * \param void
         * \return void
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
        inline const bool add_handle(const std::string sn, const std::string si) {
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
         * \return void
         */
        inline void start(mgr::message_manager& messages, const std::string si) {
            if(instance_map.find(si) != instance_map.end()) {
                if(instance_map.find(si)->second.second == false) {
                    instance_map.find(si)->second.second = true;
                    messages.add_message(message("audio", "play_sample", instance_map.find(si)->second.first + ";" + si));
                }
            }
        };

        /*!
         * Stop sample loop.
         * \param messages Reference to message manager.
         * \param si Sample instance reference.
         * \return void
         */
        inline void stop(mgr::message_manager& messages, const std::string si) {
            if(instance_map.find(si) != instance_map.end()) {
                if(instance_map.find(si)->second.second == true) {
                    instance_map.find(si)->second.second = false;
                    messages.add_message(message("audio", "stop_sample", si));
                }
            }
        };

    private:
        std::map<std::string, std::pair<std::string, bool>> instance_map;
};

} //  namespace cmp

} //  namespace wte

#endif
