/*!
 * WTEngine | File:  messages.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/messages.hpp"

namespace wte
{

namespace mgr
{

template <> bool messages::manager<messages>::initialized = false;

message_container messages::msg_queue = {};

/*
 *
 */
messages::messages() {
    msg_queue.clear();

    #if WTE_DEBUG_MODE
    //  If debug mode is enabled, create a new log file
    debug_log_file.open("wte_debug//wte_debug_messages.txt", std::ios::trunc);
    debug_log_file << "Logging messages..." << std::endl << std::endl;
    #endif
}

/*
 *
 */
messages::~messages() {
    msg_queue.clear();

    #if WTE_DEBUG_MODE
    //  Close log file if debugging is enabled
    debug_log_file.close();
    #endif
}

/*
 *
 */
void messages::clear(void) { msg_queue.clear(); }

/*
 *
 */
void messages::add_message(const message& msg) {
    msg_queue.insert(msg_queue.begin(), msg);
    if(msg.is_timed_event()) std::sort(msg_queue.begin(), msg_queue.end());
}

/*
 *
 */
//  Ignore message pruning if WTE_NO_PRUNE build flag is defined
void messages::prune(void) {
    for(auto it = msg_queue.begin(); it != msg_queue.end();) {
        //  End early if events are in the future.
        if(it->get_timer() > engine_time::check_time()) break;
        if(it->is_timed_event()) {
            #if WTE_DEBUG_MODE
            debug_log_file << "MESSAGE DELETED | ";
            debug_log_message(*it);
            #endif
            it = msg_queue.erase(it);
        }
        else it++;
    }
}

/*
 *
 */
const message_container messages::get_messages(const std::string& sys) {
    message_container temp_messages;

    for(auto it = msg_queue.begin(); it != msg_queue.end();) {
        //  End early if events are in the future
        if(it->get_timer() > engine_time::check_time()) break;

        if((it->get_timer() == -1 || it->get_timer() == engine_time::check_time()) && it->get_sys() == sys) {
            #if WTE_DEBUG_MODE
            //  Log the message if debug mode is on
            debug_log_message(*it);
            #endif
            temp_messages.push_back(*it);  //  Add the message to the temp vector to be returned.
            it = msg_queue.erase(it);  //  Erase the message once processed.
        } else it++;  //  Message not processed, iterate to next.
    }

    return temp_messages;
}

/*
 *
 */
void messages::load_file(const std::string& fname) {
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

        //  Read the message from file.
        read_message(*file, timer, sys, to, from, cmd, args);

        //  Add message to queue.  Ignore incomplete messages.
        if(sys != "" && cmd != "") msg_queue.push_back(message(timer, sys, to, from, cmd, args));
    }
    al_fclose(file);

    //  Sort the queue so timed events are in order first to last.
    std::sort(msg_queue.begin(), msg_queue.end());
}

/*
 *
 */
const bool messages::load_script(const std::string& fname) {
    //  Open data file - read binary mode.
    ALLEGRO_FILE* file;
    file = al_fopen(fname.c_str(), "rb");
    //  File not found, error.
    if(!file) {
        al_fclose(file);
        return false;
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

        //  Read the message from file.
        read_message(*file, timer, sys, to, from, cmd, args);

        //  Add the current time to the timer value.
        if(timer != -1) timer += engine_time::check_time();

        //  Add message to queue.  Ignore incomplete messages.  Sort while adding.
        if(sys != "" && cmd != "") add_message(message(timer, sys, to, from, cmd, args));
    }
    al_fclose(file);
    return true;
}

/*
 *
 */
void messages::read_message(ALLEGRO_FILE& file,
                            int64_t& timer,
                            std::string& sys,
                            std::string& to,
                            std::string& from,
                            std::string& cmd,
                            std::string& args) {
    //  Read in timer value.
    al_fread(&file, &timer, sizeof(int64_t));

    //  For each of the strings above, loop through and read
    //  each charecter, ending if we reach null termed or eof.
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == EOF) break;  //  End loop if eof.
        if(ch == '\0') break;  //  End loop if null terminated.
        sys += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == EOF) break;  //  End loop if eof.
        if(ch == '\0') break;  //  End loop if null terminated.
        to += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == EOF) break;  //  End loop if eof.
        if(ch == '\0') break;  //  End loop if null terminated.
        from += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == EOF) break;  //  End loop if eof.
        if(ch == '\0') break;  //  End loop if null terminated.
        cmd += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == EOF) break;  //  End loop if eof.
        if(ch == '\0') break;  //  End loop if null terminated.
        args += ch;
    }
}

#if WTE_DEBUG_MODE
/*
 *
 */
void messages::debug_log_message(const message& msg) {
    debug_log_file << "PROC AT:  " << engine_time::check_time() << " | ";
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

} //  namespace mgr

} //  namespace wte
