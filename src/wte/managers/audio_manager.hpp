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
#include "make_thread.hpp"

#include "message_manager.hpp"

namespace wte
{

namespace mgr
{

//!  Audio Manager
/*!
  Handles audio messages in a thread
*/
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        //!  Clears the internal audio deck
        inline audio_manager() { audio_messages.clear(); }
        //!  Clears the internal audio deck
        inline ~audio_manager() { audio_messages.clear(); }

        //!  Copy a container of messages to the internal audio deck
        void transfer_messages(message_container);

    private:
        //!  Run the audio manager
        void run(void);

        std::deque<message> audio_messages;
};

template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

/*!
*/
inline void audio_manager::transfer_messages(message_container messages) {
    audio_messages.insert(audio_messages.end(),
                          std::make_move_iterator(messages.begin()),
                          std::make_move_iterator(messages.end()));
}

/*!
*/
inline void audio_manager::run(void) {
    ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    while(is_running() == true) {
        //  Check audio messages and process
        if(!audio_messages.empty()) {
            //std::cout << "Audio event:  " << audio_messages.front().get_args() << std::endl;
            audio_messages.pop_front(); //  Remove processed message from the deck
        }
    }

    al_destroy_mixer(mixer_1);
    al_destroy_mixer(mixer_2);
    al_destroy_voice(voice);
}

} //  namespace mgr

} //  end namespace wte

#endif
