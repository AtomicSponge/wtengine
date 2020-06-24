/*!
 * \brief WTEngine | File:  sample.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Sample component.
 */

#ifndef WTE_CMP_SAMPLE_HPP
#define WTE_CMP_SAMPLE_HPP

#include <string>

#include "component.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

//! Sample component
/*!
 * Handle sample looping.
 */
class sample final : public component {
    public:
        /*!
         * Sample constructor.
         * \param sn Sample name to play.
         * \param si Sample instance name to use.
         * \return void
         */
        inline sample(const std::string sn, const std::string si) :
        instance_name(si), playing(false) {
            if(sn.find(".") == std::string::npos) sample_name = sn;
            else sample_name = sn.substr(0, sn.find("."));
        };

        /*!
         * Start sample loop.
         * \param messages Reference to message manager.
         */
        inline void start(mgr::message_manager& messages) {
            if(playing == false) {
                playing = true;
                messages.add_message(message("audio", "play_sample", sample_name + ";" + instance_name));
            }
        };

        /*!
         * Stop sample loop.
         * \param messages Reference to message manager.
         */
        inline void stop(mgr::message_manager& messages) {
            if(playing == true) {
                playing = false;
                messages.add_message(message("audio", "stop_sample", instance_name));
            }
        };

    private:
        std::string sample_name;
        std::string instance_name;
        bool playing;
};

} //  namespace cmp

} //  namespace wte

#endif
