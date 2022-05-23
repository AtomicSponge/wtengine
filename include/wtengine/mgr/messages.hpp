/*!
 * WTEngine | File:  messages.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_MGR_MESSAGES_HPP
#define WTE_MGR_MESSAGES_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/cmp/dispatcher.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte {
    class engine;
}

namespace wte::mgr {

/*!
 * \class messages
 * \brief Store a collection of message objects in a vector for processing.
 */
class messages final : private manager<messages> {
    friend class wte::engine;
    friend class systems;

    public:
        /*!
         * \brief Adds a message object to the start of the msg_queue vector.
         * 
         * Then sorts if it's a timed event.
         * 
         * \param msg Message to add.
         */
        static void add(const message& msg);

        /*!
         * \brief Load additional data into the message queue.
         * 
         * Can be called by a system message to load additional game data.
         * Note the timer value used for scripts is adjusted by the game timer.
         * 
         * \param fname Filename to load.
         * \return True if loaded, false if not.
         */
        static const bool load_script(const std::string& fname);

    private:
        inline messages() {
            if constexpr (build_options.debug_mode) {
                debug_log_file.open("wte_debug//messages.txt", std::ios::trunc);
                debug_log_file << "Logging messages..." << std::endl << std::endl;
            }
        };
        inline ~messages() { if constexpr (build_options.debug_mode) debug_log_file.close(); };
        //  Clear the message queue.
        static void clear(void);
        /*
         * Process dispatcher components. 
         * Get messages for the entities and pass to each.
         * Keeps checking for responces and will process as well.
         */
        static void dispatch(void);
        /*
         * Load a new data file into the message queue.
         * This is called when a new game is created.
         * Events are placed in order according to the timer value.
         */
        static void load_file(const std::string& fname);
        /*
         * Get messages based on their command.
         * Once events in the future are reached, break early.
         */
        static const message_container get(const std::string& sys);
        //  Deletes timed messages that were not processed.
        inline static void prune(void) {
            for(auto it = _messages.begin(); it != _messages.end();) {
                //  End early if events are in the future.
                if(it->get_timer() > engine_time::check()) break;
                if constexpr (build_options.debug_mode) {
                    debug_log_file << "MESSAGE DELETED | ";
                    log(*it);
                }
                it = _messages.erase(it);
            }
        };
        //  Read a message from file.
        static void read(
            ALLEGRO_FILE& file,
            int64_t& timer,
            std::string& sys,
            std::string& to,
            std::string& from,
            std::string& cmd,
            std::string& args
        );
        //  Write a message to the debug log file if debugging is enabled.
        inline static void log(const message& msg) {
            if constexpr (build_options.debug_mode) {
                debug_log_file << "PROC AT:  " << engine_time::check() << " | ";
                debug_log_file << "TIMER:  " << msg.get_timer() << " | ";
                debug_log_file << "SYS:  " << msg.get_sys() << " | ";
                if((msg.get_to() != "") || (msg.get_from() != "")) {
                    debug_log_file << "TO:  " << msg.get_to() << " | ";
                    debug_log_file << "FROM:  " << msg.get_from() << " | ";
                }
                debug_log_file << "CMD:  " << msg.get_cmd() << " | ";
                debug_log_file << "ARGS:  ";
                msg_args arglist = msg.get_args();
                for(auto i = arglist.begin(); i != arglist.end(); i++) {
                    debug_log_file << *i;
                    if(std::next(i, 1) != arglist.end()) debug_log_file << ";";
                }
                debug_log_file << std::endl;
            }
        };
        static std::ofstream debug_log_file;  //  For message logging
        //  Vector of all messages to be processed
        static message_container _messages;
};

}  //  end namespace wte::mgr

#endif
