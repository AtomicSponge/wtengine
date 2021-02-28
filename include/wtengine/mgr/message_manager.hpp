/*!
 * WTEngine | File:  message_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_MESSAGE_MANAGER_HPP
#define WTE_MGR_MESSAGE_MANAGER_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/mgr/engine_time.hpp"
#include "wtengine/message.hpp"
#include "wtengine/wte_global_defines.hpp"

#if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
#include <fstream>
#endif

namespace wte
{

namespace mgr
{

/*!
 * \class message_manager
 * \brief Store a collection of message objects in a vector for processing.
 */
class message_manager final : private manager<message_manager>, private engine_time {
    public:
        /*!
         * \brief Message queue constructor.
         * 
         * Clear any existing queue and start logging if debugging is enabled.
         */
        message_manager();

        /*!
         * \brief Message queue destructor.
         * 
         * Delete message queue object and close log file if debugging is enabled.
         */
        ~message_manager();

        /*!
         * \brief Clear the message queue.
         */
        void clear(void);

        /*!
         * \brief Adds a message object to the start of the msg_queue vector.
         * 
         * Then sorts if it's a timed event.
         * 
         * \param msg Message to add.
         */
        void add_message(const message& msg);

        //  Ignore message pruning if WTE_NO_PRUNE build flag is defined
        #ifndef WTE_NO_PRUNE
        /*!
         * Deletes timed messages that were not processed.
         */
        void prune(void);
        #endif

        /*!
         * \brief Get messages based on their command.
         * 
         * Once events in the future are reached, break early.
         * 
         * \param sys Manager/system to get messages for.
         * \return Vector of messages.
         */
        const message_container get_messages(const std::string& sys);

        /*!
         * \brief Load a new data file into the message queue.
         * 
         * This is called when a new game is created.
         * Events are placed in order according to the timer value.
         * 
         * \param fname Filename to load.
         */
        void load_file(const std::string& fname);

        /*!
         * \brief Load additional data into the message queue.
         * 
         * This is called by a system message to load additional game data.
         * Note the timer value used for scripts is adjusted by the game timer.
         * 
         * \param fname Filename to load.
         * \return True if loaded, false if not.
         */
        const bool load_script(const std::string& fname);

    private:
        /*!
         * \brief Read a message from file.
         * 
         * \param file Allegro file to read from.
         * \param timer Timer value to write to.
         * \param sys System value to write to.
         * \param to Entity To value to write to.
         * \param from Entity From value to write to.
         * \param cmd Command value to write to.
         * \param args Argument value to write to.
         */
        void read_message(ALLEGRO_FILE& file,
                                 int64_t& timer,
                                 std::string& sys,
                                 std::string& to,
                                 std::string& from,
                                 std::string& cmd,
                                 std::string& args);

        /*!
         * \brief Log processed messages to a file.
         * 
         * Write a message to the debug log file if debugging is enabled.
         * 
         * \param msg Message to write.
         */
        #if WTE_DEBUG_MODE == 2 || WTE_DEBUG_MODE == 9
        void debug_log_message(const message& msg);

        std::ofstream debug_log_file;
        #endif

        //  Vector of all messages to be processed
        message_container msg_queue;
};

} //  namespace mgr

} //  namespace wte

#endif
