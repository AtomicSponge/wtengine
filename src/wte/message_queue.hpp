/*
  WTEngine
  By:  Matthew Evans
  File:  message_queue.hpp

  See LICENSE.txt for copyright information

  Message queue object
  Stores a vector of messages and processes them
*/

#ifndef WTE_MSG_MESSAGE_QUEUE_HPP
#define WTE_MSG_MESSAGE_QUEUE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "wte_globals.hpp"
#include "message.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace msg
{

typedef std::vector<message> message_container;
typedef std::vector<message>::iterator message_iterator;
typedef std::vector<message>::const_iterator message_citerator;

//! message_queue class
/*!
  Store a collection of message objects in a vector for processing
*/
class message_queue {
    public:
        message_queue();                                /*!< Message queue constructor */
        ~message_queue();                               /*!< Message queue destructor */

        message_queue(const message_queue&) = delete;
        void operator=(message_queue const&) = delete;

        bool new_data_file(std::string);                /*!< Load a new data file into the message queue */
        void set_time(int64_t);                         /*!< Set the internal timer value */
        void add_message(message);                      /*!< Add a message to the queue */
        void clear_queue(void);                         /*!< Clear the message queue */
        const message_container get_messages(const std::string);    /*!< Get messages based on their command */

    private:
        int64_t current_time;                           /*!< Store timer for message processing */
        message_container msg_queue;                    /*!< Vector of all messages to be processed */

        static bool initialized;

        #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        void debug_log_message(message, int64_t);       /*!< Member to log processed messages to a file */
        #endif
};

inline bool message_queue::initialized = false;

//! Message queue constructor
/*!
  Clear any existing queue and start logging if debugging is enabled
*/
inline message_queue::message_queue() {
    if(initialized == true) throw std::runtime_error("Message Queue already running!");
    initialized = true;

    msg_queue.clear();

    //  If debug mode is enabled, create a new log file
    #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        std::ofstream debug_log_file;
        debug_log_file.open("wte_debug\\wte_debug_message_queue.txt", std::ios::trunc);
        debug_log_file << "Logging messages";
        debug_log_file << std::endl << std::endl;
        debug_log_file.close();
    #endif
}

//! Message queue destructor
/*!
  Delete message queue object
*/
inline message_queue::~message_queue() {
    msg_queue.clear();

    initialized = false;
}

//! Debug message logging
/*!
  Write a message to the debug log file if debugging is enabled
*/
#if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
inline void message_queue::debug_log_message(message msg, int64_t current_time) {
    std::ofstream debug_log_file;
    debug_log_file.open("wte_debug\\wte_debug_message_queue.txt", std::ios::app);
    debug_log_file << "TIMER:  " << current_time << " | ";
    debug_log_file << "CMD:  " << msg.get_cmd() << " | ";
    debug_log_file << "FROM:  " << msg.get_from() << " | ";
    debug_log_file << "TO:  " << msg.get_to() << " | ";
    debug_log_file << "ARGS:  " << msg.get_args() << std::endl;
    debug_log_file.close();
}
#endif

//! Reset the message queue with a new data file
/*!
  Events are placed in order according to the timer value
*/
inline bool message_queue::new_data_file(std::string file) {
    std::ifstream data_file;
    int64_t timer;
    std::string cmd;
    std::string from;
    std::string to;
    std::string args;

    msg_queue.clear();

    //  Open data file - input binary mode
    data_file.open(file, std::fstream::in | std::fstream::binary);
    if(!data_file.good()) return false; //  Error loading data file

    //  Loop through the entire data file loading into the queue
    while(true) {
        data_file.read(reinterpret_cast<char *>(&timer), sizeof(int64_t));
        std::getline(data_file, cmd, '\0');
        std::getline(data_file, from, '\0');
        std::getline(data_file, to, '\0');
        std::getline(data_file, args, '\0');

        if(data_file.eof()) break;
        msg_queue.push_back(message(timer, cmd, from, to, args));
    }
    data_file.close();

    //  Sort the queue so timed events are in order first to last
    std::sort(msg_queue.begin(), msg_queue.end());
    
    return true;
}

//! Set the internal timer
/*!
  Called by the main engine loop
*/
inline void message_queue::set_time(int64_t t) { current_time = t; }

//! Add a message to the queue
/*!
  Adds a message object to the msg_queue vector, then sorts if it's a timed event
*/
inline void message_queue::add_message(message msg) {
    msg_queue.insert(msg_queue.begin(), msg);
    if(msg.is_timed_event()) std::sort(msg_queue.begin(), msg_queue.end());
}

//! Clear the queue
/*!
  Wipe the existing message queue
*/
inline void message_queue::clear_queue(void) { msg_queue.clear(); }

//! Get current messages based on the command field
/*!
  Once events in the future are reached, break early
*/
inline const message_container message_queue::get_messages(const std::string cmd) {
    message_container temp_messages;

    //  Return empty vector if the queue is empty
    if(msg_queue.empty()) return {};

    for(message_iterator it = msg_queue.begin(); it != msg_queue.end(); it++) {
        //  End early if events are in the future
        if(it->get_timer() > current_time) break;

        if((it->get_timer() == current_time || it->get_timer() == -1) && it->get_cmd() == cmd) {
            //  Log the message if debug mode is on
            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
                debug_log_message(*it, current_time);
            #endif
            temp_messages.push_back(*it); //  Add the message to the temp vector to be returned
            it = msg_queue.erase(it); //  Erase the message once processed
        }
    }

    return temp_messages;
}

} //  namespace msg

} //  namespace wte

#endif
