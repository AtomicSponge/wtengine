/*!
 * wtengine | File:  messages.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/messages.hpp"

namespace wte::mgr {

template <> bool manager<messages>::initialized = false;

message_container messages::_messages;
std::ofstream messages::debug_log_file;

/*
 *
 */
messages::messages() {
    if constexpr (build_options.debug_mode) {
        std::time_t t = std::time(nullptr);
        std::ostringstream date_stream;
        date_stream << std::put_time(std::localtime(&t), "%d-%m-%Y_%H-%M-%S");
        std::string date = date_stream.str();
        std::cout << "Logging messages to:  messages_" + date + ".log.txt";
        debug_log_file.open("messages_" + date + ".log.txt", std::ios::trunc);
        debug_log_file << "Logging messages..." << std::endl << std::endl;
    }
}

/*
 *
 */
messages::~messages() { if constexpr (build_options.debug_mode) debug_log_file.close(); }

/*
 *
 */
void messages::clear(void) { _messages.clear(); }

/*
 *
 */
void messages::prune(void) {
    for(auto it = _messages.begin(); it != _messages.end();) {
        //  End early if events are in the future.
        if(it->get_timer() > engine_time::check()) break;
        if constexpr (build_options.debug_mode) {
            debug_log_file << "MESSAGE DELETED | ";
            log(*it);
        }
        it = _messages.erase(it);
    }
}

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
void messages::log(const message& msg) {
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
}

/*
 *
 */
void messages::dispatch(void) {
    component_container<cmp::dispatcher> dispatch_components =
        mgr::world::set_components<cmp::dispatcher>();

    while(true) {  //  Infinite loop to verify all current messages are processed.
        message_container temp_msgs = get("entities");
        if(temp_msgs.empty()) break;  //  No messages, end while(true) loop.

        //  For all messages, check each dispatch component.
        for(auto& m_it: temp_msgs) { for(auto& c_it: dispatch_components) {
            try {
                if(m_it.get_to() == mgr::world::get_name(c_it.first)) {
                    c_it.second->handle_msg(c_it.first, m_it);
                    break;  //  Found, stop checking dispatch components.
                }
            } catch(const std::exception& e) {
                throw e;
                break;
            } catch(...) { break; }
        }}  //  End double for
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
            log(*it);
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
        throw engine_error("Can not load game data!");
    }

    //  Loop through the entire data file loading into the queue.
    while(true) {
        if(al_feof(file)) break;  //  End loop if eof.

        int64_t timer = -1;
        std::string sys, to, from, cmd, args;

        try{
            //  Read the message from file.
            read(*file, timer, sys, to, from, cmd, args);
            //  Add message to queue.  Ignore incomplete messages.
            if(sys != "" && cmd != "") _messages.push_back(message(timer, sys, to, from, cmd, args));
        } catch(const std::exception& e) {
            throw engine_error("Unable to read game data file!");
        }
    }
    al_fclose(file);

    //  Sort the queue so timed events are in order first to last.
    std::sort(_messages.begin(), _messages.end());
}

/*
 *
 */
bool messages::load_script(const std::string& fname) {
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

        try {
            //  Read the message from file.
            read(*file, timer, sys, to, from, cmd, args);
            //  Add the current time to the timer value.
            if(timer != -1) timer += engine_time::check();
            //  Add message to queue.  Ignore incomplete messages.  Sort while adding.
            if(sys != "" && cmd != "") add(message(timer, sys, to, from, cmd, args));
        } catch(const std::exception& e) { throw e; }
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
        if(ch == '\0') break;  //  End loop if null terminated.
        if(ch == EOF) {
            throw engine_exception("Bad message file format!", "Messages", 2);
            return;
        }
        sys += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == '\0') break;  //  End loop if null terminated.
        if(ch == EOF) {
            throw engine_exception("Bad message file format!", "Messages", 2);
            return;
        }
        to += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == '\0') break;  //  End loop if null terminated.
        if(ch == EOF) {
            throw engine_exception("Bad message file format!", "Messages", 2);
            return;
        }
        from += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == '\0') break;  //  End loop if null terminated.
        if(ch == EOF) {
            throw engine_exception("Bad message file format!", "Messages", 2);
            return;
        }
        cmd += ch;
    }
    while(true) {
        const char ch = al_fgetc(&file);
        if(ch == '\0') break;  //  End loop if null terminated.
        if(ch == EOF) {
            throw engine_exception("Bad message file format!", "Messages", 2);
            return;
        }
        args += ch;
    }
}

}  //  end namespace wte::mgr
