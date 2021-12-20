/*!
 * WTEngine | File:  messages.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/mgr/manager.hpp"

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
    friend class menus;

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
         * Load a new data file into the message queue.
         * This is called when a new game is created.
         * Events are placed in order according to the timer value.
         */
        static void load_file(const std::string& fname);
        /*
         * Get messages based on their command.
         * Once events in the future are reached, break early.
         */
        inline static const message_container get(const std::string& sys) {
            if constexpr (build_options.debug_mode) return _get_debug(sys); else return _get(sys);
        };
        static const message_container _get(const std::string& sys);        //  Normal get
        static const message_container _get_debug(const std::string& sys);  //  Debug get
        //  Deletes timed messages that were not processed.
        inline static void prune(void) {
            if constexpr (build_options.debug_mode) _prune_debug(); else _prune();
        };
        static void _prune(void);        //  Normal prune
        static void _prune_debug(void);  //  Debug prune
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
        static void log(const message& msg);
        static std::ofstream debug_log_file;  //  For message logging
        //  Vector of all messages to be processed
        static message_container _messages;
};

}  //  end namespace wte::mgr

#endif
