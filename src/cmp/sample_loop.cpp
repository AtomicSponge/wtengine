/*!
 * WTEngine | File:  sample_loop.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/sample_loop.hpp"

namespace wte
{

namespace cmp
{

sample_loop::sample_loop() {
    instance_map.clear();
}

sample_loop::~sample_loop() {
    instance_map.clear();
}

const bool sample_loop::add_handle(const std::string& sn, const std::string& si) {
    if(sn == "all") return false;
    if(si == "once") return false;
    std::string sample_name;
    if(sn.find(".") == std::string::npos) sample_name = sn;
    else sample_name = sn.substr(0, sn.find("."));
    auto ret = instance_map.insert(std::make_pair(si, std::make_pair(sample_name, false)));
    return ret.second;
}

void sample_loop::start(mgr::message_manager& messages, const std::string& si) {
    auto ref = instance_map.find(si);
    if(ref != instance_map.end()) {
        if(ref->second.second == false) {  //  If not playing.
            ref->second.second = true;  //  Set is playing.
            messages.add_message(message("audio", "play_sample", ref->second.first + ";" + si));
        }
    }
}

void sample_loop::stop(mgr::message_manager& messages, const std::string& si) {
    auto ref = instance_map.find(si);
    if(ref != instance_map.end()) {
        if(ref->second.second == true) {  //  If is playing.
            ref->second.second = false;  //  Set not playing.
            messages.add_message(message("audio", "stop_sample", si));
        }
    }
}

} //  namespace cmp

} //  namespace wte
