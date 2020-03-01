/*
  WTEngine
  By:  Matthew Evans
  File:  message_manager.hpp

  See LICENSE.txt for copyright information

  Message queue object
  Stores a vector of messages and processes them
*/

#ifndef WTE_MGR_MESSAGE_MANAGER_HPP
#define WTE_MGR_MESSAGE_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "manager.hpp"
#include "engine_time.hpp"

#include "..\wte_global_defines.hpp"
#include "..\message.hpp"
#include "entity_manager.hpp"

namespace wte
{

typedef std::vector<message> message_container;
typedef std::vector<message>::iterator message_iterator;
typedef std::vector<message>::const_iterator message_citerator;

namespace mgr
{

//! message_manager class
/*!
  Store a collection of message objects in a vector for processing
*/
class message_manager final : public manager<message_manager>, private engine_time {
    public:
        //!  Message queue constructor
        //!  Clear any existing queue and start logging if debugging is enabled
        inline message_manager() {
            msg_queue.clear();

            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
            //  If debug mode is enabled, create a new log file
            debug_log_file.open("wte_debug\\wte_debug_message_manager.txt", std::ios::trunc);
            debug_log_file << "Logging messages..." << std::endl << std::endl;
            #endif
        };

        //!  Message queue destructor
        //!  Delete message queue object and close log file if debugging is enabled
        inline ~message_manager() {
            msg_queue.clear();

            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
            //  Close log file if debugging is enabled
            debug_log_file.close();
            #endif
        };

        //!  Adds a message object to the start of the msg_queue vector
        //!  Then sorts if it's a timed event
        inline void add_message(const message msg) {
            msg_queue.insert(msg_queue.begin(), msg);
            if(msg.is_timed_event()) std::sort(msg_queue.begin(), msg_queue.end());
        };

        //!  Load a new data file into the message queue
        void new_data_file(const std::string);
        //!  Clear the message queue
        inline void clear_queue(void) { msg_queue.clear(); };
        //!  Get messages based on their command
        const message_container get_messages(const std::string);

    private:
        //  Vector of all messages to be processed
        message_container msg_queue;

        #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        std::ofstream debug_log_file;
        //!  Log processed messages to a file
        void debug_log_message(const message);
        #endif
};

template <> inline bool message_manager::manager<message_manager>::initialized = false;

#if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
/*!
  Write a message to the debug log file if debugging is enabled
*/
inline void message_manager::debug_log_message(const message msg) {
    debug_log_file << "PROC AT:  " << check_time() << " | ";
    debug_log_file << "TIMER:  " << msg.get_timer() << " | ";
    debug_log_file << "SYS:  " << msg.get_sys() << " | ";
    if((msg.get_to() != "") || (msg.get_from() != "")) {
        debug_log_file << "TO:  " << msg.get_to() << " | ";
        debug_log_file << "FROM:  " << msg.get_from() << " | ";
    }
    debug_log_file << "CMD:  " << msg.get_cmd() << " | ";
    debug_log_file << "ARGS:  ";
    msg_arg_list arglist = msg.get_arglist();
    for(auto i = arglist.begin(); i != arglist.end(); i++) {
        debug_log_file << *i;
        if(std::next(i, 1) != arglist.end()) debug_log_file << ";";
    }
    debug_log_file << std::endl;
}
#endif

/*!
  Events are placed in order according to the timer value
*/
inline void message_manager::new_data_file(const std::string file) {
    std::ifstream data_file;
    int64_t timer;
    std::string sys;
    std::string cmd;
    std::string args;

    msg_queue.clear();

    //  Open data file - input binary mode
    data_file.open(file, std::fstream::in | std::fstream::binary);
    if(!data_file.good()) throw std::runtime_error("Error reading game data file!");

    //  Loop through the entire data file loading into the queue
    while(true) {
        data_file.read(reinterpret_cast<char *>(&timer), sizeof(int64_t));
        std::getline(data_file, sys, '\0');
        std::getline(data_file, cmd, '\0');
        std::getline(data_file, args, '\0');

        if(data_file.eof()) break;
        msg_queue.push_back(message(timer, sys, cmd, args));
    }
    data_file.close();

    //  Sort the queue so timed events are in order first to last
    std::sort(msg_queue.begin(), msg_queue.end());
}

/*!
  Once events in the future are reached, break early
*/
inline const message_container message_manager::get_messages(const std::string sys) {
    message_container temp_messages;

    //  Return empty vector if the queue is empty
    if(msg_queue.empty()) return {};

    for(message_iterator it = msg_queue.begin(); it != msg_queue.end();) {
        //  End early if events are in the future
        if(it->get_timer() > check_time()) break;

        if((it->get_timer() == check_time() || it->get_timer() == -1) && it->get_sys() == sys) {
            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
            //  Log the message if debug mode is on
            debug_log_message(*it);
            #endif
            temp_messages.push_back(*it);  //  Add the message to the temp vector to be returned.
            it = msg_queue.erase(it);  //  Erase the message once processed.
        } else it++;  //  Message not processed, iterate to next.
    }

    return temp_messages;
}

} //  namespace mgr

} //  namespace wte

#endif
