/*!
 * \brief WTEngine | File:  message_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Message queue object.
 * Stores a vector of messages and processes them.
 */

#ifndef WTE_MGR_MESSAGE_MANAGER_HPP
#define WTE_MGR_MESSAGE_MANAGER_HPP

#if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
#include <fstream>
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "manager.hpp"
#include "engine_time.hpp"
#include "../_globals/engine_flags.hpp"
#include "../message.hpp"
#include "../wte_global_defines.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class Message Manager class
 * Store a collection of message objects in a vector for processing.
 */
class message_manager final : public manager<message_manager>, private engine_time {
    public:
        /*!
         * \brief Message queue constructor
         * Clear any existing queue and start logging if debugging is enabled.
         * \param void
         * \return void
         */
        inline message_manager() {
            msg_queue.clear();

            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
            //  If debug mode is enabled, create a new log file
            debug_log_file.open("wte_debug//wte_debug_message_manager.txt", std::ios::trunc);
            debug_log_file << "Logging messages..." << std::endl << std::endl;
            #endif
        };

        /*!
         * Message queue destructor
         * Delete message queue object and close log file if debugging is enabled.
         * \param void
         * \return void
         */
        inline ~message_manager() {
            msg_queue.clear();

            #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
            //  Close log file if debugging is enabled
            debug_log_file.close();
            #endif
        };

        /*!
         * Adds a message object to the start of the msg_queue vector
         * Then sorts if it's a timed event
         * \param msg Message to add.
         * \return void
         */
        inline void add_message(const message msg) {
            msg_queue.insert(msg_queue.begin(), msg);
            if(msg.is_timed_event()) std::sort(msg_queue.begin(), msg_queue.end());
        };

        //  Ignore message pruning if WTE_NO_PRUNE build flag is defined
        #ifndef WTE_NO_PRUNE
        /*!
         * Deletes timed messages that were not processed.
         * \param void
         * \return void
         */
        inline void prune(void) {
            for(auto it = msg_queue.begin(); it != msg_queue.end();) {
                //  End early if events are in the future.
                if(it->get_timer() > check_time()) break;
                if(it->is_timed_event()) {
                    it = msg_queue.erase(it);
                    #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
                    debug_log_file << "MESSAGE DELETED | ";
                    debug_log_file << "TIMER:  " << it->get_timer() << " | ";
                    debug_log_file << "SYS:  " << it->get_sys() << " | ";
                    if((it->get_to() != "") || (it->get_from() != "")) {
                        debug_log_file << "TO:  " << it->get_to() << " | ";
                        debug_log_file << "FROM:  " << it->get_from() << " | ";
                    }
                    debug_log_file << "CMD:  " << it->get_cmd() << " | ";
                    debug_log_file << "ARGS:  ";
                    msg_arg_list arglist = it->get_arglist();
                    for(auto i = arglist.begin(); i != arglist.end(); i++) {
                        debug_log_file << *i;
                        if(std::next(i, 1) != arglist.end()) debug_log_file << ";";
                    }
                    debug_log_file << std::endl;
                    #endif
                }
                else it++;
            }
        };
        #endif

        /*!
         * Clear the message queue.
         * \param void
         * \return void
         */
        inline void clear(void) { msg_queue.clear(); };

        /*!
         * \brief Get messages based on their command.
         * Once events in the future are reached, break early.
         * \param sys Manager/system to get messages for.
         * \return Vector of messages.
         */
        inline const message_container get_messages(const std::string sys) {
            message_container temp_messages;

            for(auto it = msg_queue.begin(); it != msg_queue.end();) {
                //  End early if events are in the future
                if(it->get_timer() > check_time()) break;

                if((it->get_timer() == -1 || it->get_timer() == check_time()) && it->get_sys() == sys) {
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
        
        /*!
         * \brief Load a new data file into the message queue.
         * Events are placed in order according to the timer value.
         * \param fname Filename to load.
         * \return void
         */
        inline void load_file(const std::string fname) {
            msg_queue.clear();
            //  Open data file - read binary mode.
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            //  File not found, error.
            if(!file) {
                al_fclose(file);
                throw std::runtime_error("Error loading game data!");
            }

            //  Loop through the entire data file loading into the queue.
            while(true) {
                if(al_feof(file)) break;  //  End loop if eof.

                int64_t timer = 0;
                std::string sys = "";
                std::string to = "";
                std::string from = "";
                std::string cmd = "";
                std::string args = "";

                //  Read in timer value.
                al_fread(file, &timer, sizeof(int64_t));

                //  For each of the strings above, loop through and read
                //  each charecter, ending if we reach null termed or eof.
                while(true) {
                    const char ch = al_fgetc(file);
                    if(ch == EOF) break;  //  End loop if eof.
                    if(ch == '\0') break;  //  End loop if null terminated.
                    sys += ch;
                }
                while(true) {
                    const char ch = al_fgetc(file);
                    if(ch == EOF) break;  //  End loop if eof.
                    if(ch == '\0') break;  //  End loop if null terminated.
                    to += ch;
                }
                while(true) {
                    const char ch = al_fgetc(file);
                    if(ch == EOF) break;  //  End loop if eof.
                    if(ch == '\0') break;  //  End loop if null terminated.
                    from += ch;
                }
                while(true) {
                    const char ch = al_fgetc(file);
                    if(ch == EOF) break;  //  End loop if eof.
                    if(ch == '\0') break;  //  End loop if null terminated.
                    cmd += ch;
                }
                while(true) {
                    const char ch = al_fgetc(file);
                    if(ch == EOF) break;  //  End loop if eof.
                    if(ch == '\0') break;  //  End loop if null terminated.
                    args += ch;
                }

                //  Add message to queue.  Ignore incomplete messages.
                if(sys != "" && cmd != "") msg_queue.push_back(message(timer, sys, to, from, cmd, args));
            }
            al_fclose(file);

            //  Sort the queue so timed events are in order first to last.
            std::sort(msg_queue.begin(), msg_queue.end());
        };

    private:
        //  Vector of all messages to be processed
        message_container msg_queue;

        #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        std::ofstream debug_log_file;
        //!  Log processed messages to a file
        //!  Write a message to the debug log file if debugging is enabled
        inline void debug_log_message(const message msg) {
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
};

template <> inline bool message_manager::manager<message_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
