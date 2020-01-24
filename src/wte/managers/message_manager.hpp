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

#include "..\wte_globals.hpp"
#include "..\message.hpp"
#include "manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

typedef std::vector<msg::message> message_container;
typedef std::vector<msg::message>::iterator message_iterator;
typedef std::vector<msg::message>::const_iterator message_citerator;

namespace mgr
{

//! message_manager class
/*!
  Store a collection of message objects in a vector for processing
*/
class message_manager final : public manager<message_manager> {
    public:
        message_manager();                                /*!< Message queue constructor */
        ~message_manager();                               /*!< Message queue destructor */

        void new_data_file(std::string);                /*!< Load a new data file into the message queue */
        void set_time(int64_t);                         /*!< Set the internal timer value */
        void add_message(msg::message);                      /*!< Add a message to the queue */
        void clear_queue(void);                         /*!< Clear the message queue */
        const message_container get_messages(const std::string);    /*!< Get messages based on their command */

    private:
        int64_t current_time;                           /*!< Store timer for message processing */
        message_container msg_queue;                    /*!< Vector of all messages to be processed */

        #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        void debug_log_message(message, int64_t);       /*!< Member to log processed messages to a file */
        #endif
};

template <> inline bool message_manager::manager<message_manager>::initialized = false;

//! Message queue constructor
/*!
  Clear any existing queue and start logging if debugging is enabled
*/
inline message_manager::message_manager() {

    msg_queue.clear();

    //  If debug mode is enabled, create a new log file
    #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        std::ofstream debug_log_file;
        debug_log_file.open("wte_debug\\wte_debug_message_manager.txt", std::ios::trunc);
        debug_log_file << "Logging messages";
        debug_log_file << std::endl << std::endl;
        debug_log_file.close();
    #endif
}

//! Message queue destructor
/*!
  Delete message queue object
*/
inline message_manager::~message_manager() {
    msg_queue.clear();
}

//! Debug message logging
/*!
  Write a message to the debug log file if debugging is enabled
*/
#if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
inline void message_manager::debug_log_message(message msg, int64_t current_time) {
    std::ofstream debug_log_file;
    debug_log_file.open("wte_debug\\wte_debug_message_manager.txt", std::ios::app);
    debug_log_file << "PROC AT:  " << current_time << " | ";
    debug_log_file << "TIMER:  " << msg.get_timer() << " | ";
    debug_log_file << "SYS:  " << msg.get_sys() << " | ";
    debug_log_file << "TO:  " << msg.get_to() << " | ";
    debug_log_file << "FROM:  " << msg.get_from() << " | ";
    debug_log_file << "CMD:  " << msg.get_cmd() << " | ";
    debug_log_file << "ARGS:  " << msg.get_args() << std::endl;
    debug_log_file.close();
}
#endif

//! Reset the message queue with a new data file
/*!
  Events are placed in order according to the timer value
*/
inline void message_manager::new_data_file(std::string file) {
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
        msg_queue.push_back(msg::message(timer, sys, cmd, args));
    }
    data_file.close();

    //  Sort the queue so timed events are in order first to last
    std::sort(msg_queue.begin(), msg_queue.end());
}

//! Set the internal timer
/*!
  Called by the main engine loop
*/
inline void message_manager::set_time(int64_t t) { current_time = t; }

//! Add a message to the queue
/*!
  Adds a message object to the start of the msg_queue vector
  Then sorts if it's a timed event
*/
inline void message_manager::add_message(msg::message msg) {
    msg_queue.insert(msg_queue.begin(), msg);
    if(msg.is_timed_event()) std::sort(msg_queue.begin(), msg_queue.end());
}

//! Clear the queue
/*!
  Wipe the existing message queue
*/
inline void message_manager::clear_queue(void) { msg_queue.clear(); }

//! Get current messages based on the command field
/*!
  Once events in the future are reached, break early
*/
inline const message_container message_manager::get_messages(const std::string sys) {
    message_container temp_messages;

    //  Return empty vector if the queue is empty
    if(msg_queue.empty()) return {};

    for(message_iterator it = msg_queue.begin(); it != msg_queue.end();) {
        //  End early if events are in the future
        if(it->get_timer() > current_time) break;

        if((it->get_timer() == current_time || it->get_timer() == -1) && it->get_sys() == sys) {
            //  Log the message if debug mode is on
            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
                debug_log_message(*it, current_time);
            #endif
            temp_messages.push_back(*it); //  Add the message to the temp vector to be returned
            it = msg_queue.erase(it); //  Erase the message once processed
        } else it++;
    }

    return temp_messages;
}

} //  namespace mgr

} //  namespace wte

#endif
