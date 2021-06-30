/*!
 * WTEngine | File:  messages.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_MESSAGES_HPP
#define WTE_MGR_MESSAGES_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/message.hpp"
#include "wtengine/global_defines.hpp"

#if WTE_DEBUG_MODE
#include <fstream>
#endif

namespace wte
{

namespace mgr
{

/*!
 * \class messages
 * \brief Store a collection of message objects in a vector for processing.
 */
class messages final : private manager<messages> {
    friend class interface;
    friend class systems;
    friend class menu;

    public:
        /*!
         * \brief Adds a message object to the start of the msg_queue vector.
         * 
         * Then sorts if it's a timed event.
         * 
         * \param msg Message to add.
         */
        static void add_message(const message& msg);

        /*!
         * \brief Load additional data into the message queue.
         * 
         * This is called by a system message to load additional game data.
         * Note the timer value used for scripts is adjusted by the game timer.
         * 
         * \param fname Filename to load.
         * \return True if loaded, false if not.
         */
        static const bool load_script(const std::string& fname);

    private:
        /*!
         * \brief Message queue constructor.
         * 
         * Clear any existing queue and start logging if debugging is enabled.
         */
        messages();

        /*!
         * \brief Message queue destructor.
         * 
         * Delete message queue object and close log file if debugging is enabled.
         */
        ~messages();

        /*!
         * \brief Clear the message queue.
         */
        static void clear(void);

        /*!
         * \brief Load a new data file into the message queue.
         * 
         * This is called when a new game is created.
         * Events are placed in order according to the timer value.
         * 
         * \param fname Filename to load.
         */
        static void load_file(const std::string& fname);

        /*!
         * \brief Get messages based on their command.
         * 
         * Once events in the future are reached, break early.
         * 
         * \param sys Manager/system to get messages for.
         * \return Vector of messages.
         */
        static const message_container get_messages(const std::string& sys);

        /*!
         * Deletes timed messages that were not processed.
         */
        static void prune(void);

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
        static void read_message(ALLEGRO_FILE& file,
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
        #if WTE_DEBUG_MODE
        static void debug_log_message(const message& msg);

        static std::ofstream debug_log_file;
        #endif

        //  Vector of all messages to be processed
        inline static message_container msg_queue = {};
};

} //  namespace mgr

} //  namespace wte

#endif
