/*!
 * WTEngine | File:  messages.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/messages.hpp"

namespace wte::mgr {

template <> bool messages::manager<messages>::initialized = false;

message_container messages::_messages;
std::ofstream messages::debug_log_file;

/*
 *
 */
void messages::clear(void) { _messages.clear(); }

/*
 *
 */
void messages::add(const message& msg) {
    _messages.insert(_messages.begin(), msg);
    if(msg.is_timed_event()) std::sort(_messages.begin(), _messages.end());
}

/*
 *
 */
void messages::_prune(void) {
    for(auto it = _messages.begin(); it != _messages.end();) {
        //  End early if events are in the future.
        if(it->get_timer() > engine_time::check()) break;
        if(it->is_timed_event()) it = _messages.erase(it);
        else it++;
    }
}

/*
 *
 */
void messages::_prune_debug(void) {
    if(!build_options.debug_mode) return;
    for(auto it = _messages.begin(); it != _messages.end();) {
        //  End early if events are in the future.
        if(it->get_timer() > engine_time::check()) break;
        if(it->is_timed_event()) {
            debug_log_file << "MESSAGE DELETED | ";
            log(*it);
            it = _messages.erase(it);
        }
        else it++;
    }
}

/*
 *
 */
const message_container messages::get(const std::string& sys) {
    message_container temp_messages;

    for(auto it = _messages.begin(); it != _messages.end();) {
        //  End early if events are in the future
        if(it->get_timer() > engine_time::check()) break;

        if((it->get_timer() == -1 || it->get_timer() == engine_time::check()) && it->get_sys() == sys) {
            //  Log the message if debug mode is on
            if constexpr (build_options.debug_mode) log(*it);

            temp_messages.push_back(*it);  //  Add the message to the temp vector to be returned.
            it = _messages.erase(it);  //  Erase the message once processed.
        } else it++;  //  Message not processed, iterate to next.
    }

    return temp_messages;
}

/*
 *
 */
void messages::load_file(const std::string& fname) {
    _messages.clear();
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

        int64_t timer = -1;
        std::string sys, to, from, cmd, args;

        //  Read the message from file.
        read(*file, timer, sys, to, from, cmd, args);

        //  Add message to queue.  Ignore incomplete messages.
        if(sys != "" && cmd != "") _messages.push_back(message(timer, sys, to, from, cmd, args));
    }
    al_fclose(file);

    //  Sort the queue so timed events are in order first to last.
    std::sort(_messages.begin(), _messages.end());
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

        int64_t timer = -1;
        std::string sys, to, from, cmd, args;

        //  Read the message from file.
        read(*file, timer, sys, to, from, cmd, args);

        //  Add the current time to the timer value.
        if(timer != -1) timer += engine_time::check();

        //  Add message to queue.  Ignore incomplete messages.  Sort while adding.
        if(sys != "" && cmd != "") add(message(timer, sys, to, from, cmd, args));
    }
    al_fclose(file);
    return true;
}

/*
 *
 */
void messages::read(
    ALLEGRO_FILE& file,
    int64_t& timer,
    std::string& sys,
    std::string& to,
    std::string& from,
    std::string& cmd,
    std::string& args
) {
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

/*
 *
 */
void messages::log(const message& msg) {
    if(!build_options.debug_mode) return;
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

}  //  end namespace wte::mgr
