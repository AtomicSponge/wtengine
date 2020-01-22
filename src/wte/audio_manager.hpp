/*
  WTEngine
  By:  Matthew Evans
  File:  audio_manager.hpp

  See LICENSE.txt for copyright information

  Handle playback of audio in its own thread
  Waits for messages to be loaded into the audio deck
*/

#ifndef WTE_AUDIO_MANAGER_HPP
#define WTE_AUDIO_MANAGER_HPP

#include <deque>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "wte_globals.hpp"
#include "make_thread.hpp"
#include "message.hpp"

namespace wte
{

//!  Container for storing audio messages
typedef std::deque<msg::message> audio_deck;

//!  Global message queue for audio processing
extern audio_deck audio_messages;
inline audio_deck audio_messages;

class audio_manager final : public make_thread {
    private:
        void run(void);
};

inline void audio_manager::run(void) {
    while(is_running() == true) {
        //  Check audio messages and process
        if(!audio_messages.empty()) {
            //std::cout << "Audio event:  " << audio_messages.front().get_args() << std::endl;
            audio_messages.pop_front(); //  Remove processed message from the deck
        }
    }
}

} //  end namespace wte

#endif
