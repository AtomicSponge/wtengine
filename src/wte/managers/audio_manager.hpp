/*
  WTEngine
  By:  Matthew Evans
  File:  audio_manager.hpp

  See LICENSE.txt for copyright information

  Handle playback of audio in its own thread
  Waits for messages to be loaded into the audio deck
*/

#ifndef WTE_MGR_AUDIO_MANAGER_HPP
#define WTE_MGR_AUDIO_MANAGER_HPP

#include <deque>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "manager.hpp"
#include "message_manager.hpp"
#include "..\make_thread.hpp"
#include "..\message.hpp"

namespace wte
{

namespace mgr
{

//!
/*!
*/
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        inline audio_manager() { audio_messages.clear(); }
        inline ~audio_manager() { audio_messages.clear(); }

        void transfer_messages(message_container);

    private:
        void run(void);

        std::deque<msg::message> audio_messages;
};

template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

//!
/*!
*/
inline void audio_manager::transfer_messages(message_container messages) {
    audio_messages.insert(audio_messages.end(),
                          std::make_move_iterator(messages.begin()),
                          std::make_move_iterator(messages.end()));
}

//!
/*!
*/
inline void audio_manager::run(void) {
    while(is_running() == true) {
        //  Check audio messages and process
        if(!audio_messages.empty()) {
            //std::cout << "Audio event:  " << audio_messages.front().get_args() << std::endl;
            audio_messages.pop_front(); //  Remove processed message from the deck
        }
    }
}

} //  namespace mgr

} //  end namespace wte

#endif
