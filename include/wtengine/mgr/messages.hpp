/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_MESSAGES_HPP)
#define WTE_MGR_MESSAGES_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
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
    static bool load_script(const std::string& fname);

  private:
    messages() = default;
    ~messages() = default;
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
    static void prune(void);
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

    static message_container _messages;   //  Vector of all messages to be processed

    static void message_log_start(void);
    static void message_log_stop(void);
    static void log(           //  Write a message to the debug log file
      const message& msg,
      const bool& deleted);
    static std::ofstream debug_log_file;  //  For message logging
};

}  //  end namespace wte::mgr

#endif
