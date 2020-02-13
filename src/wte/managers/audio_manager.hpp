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

#include <string>
#include <deque>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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

  Mixer Main - All other mixers attach to this
  Mixer 1 - Play music
  Mixer 2 - Play sound effects
*/
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        //!  Audio Manager Constructor
        //!  Clears the internal audio deck
        inline audio_manager() {
            map_cmd_str_values["play_song"] = CMD_STR_PLAY_SONG;
            map_cmd_str_values["stop_song"] = CMD_STR_STOP_SONG;
            map_cmd_str_values["pause_song"] = CMD_STR_PAUSE_SONG;
            map_cmd_str_values["unpause_song"] = CMD_STR_UNPAUSE_SONG;
            audio_messages.clear();
            al_install_audio();
            al_init_acodec_addon();
        }

        //!  Audio Manager Destructor
        //!  Clears the internal audio deck
        inline ~audio_manager() {
            al_uninstall_audio();
            map_cmd_str_values.clear();
            audio_messages.clear();
        }

        //!  Take a vector of messages and pass them into the audio messages deck
        inline void transfer_messages(const message_container messages) {
            audio_messages.insert(audio_messages.end(),
                                std::make_move_iterator(messages.begin()),
                                std::make_move_iterator(messages.end()));
        }

    private:
        //!  Run the audio manager
        void run(void);

        //  Used for switching on audio messages:
        enum CMD_STR_VALUE {
            CMD_STR_PLAY_SONG,      CMD_STR_STOP_SONG,
            CMD_STR_PAUSE_SONG,     CMD_STR_UNPAUSE_SONG,
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        std::deque<message> audio_messages;
};

template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

/*!
  Audio playback
*/
inline void audio_manager::run(void) {
    ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_main = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    ALLEGRO_AUDIO_STREAM* stream = NULL;

    bool song_loaded = false, song_paused = false, loop_song = true;

    al_attach_mixer_to_voice(mixer_main, voice);
    al_attach_mixer_to_mixer(mixer_1, mixer_main);
    al_attach_mixer_to_mixer(mixer_2, mixer_main);

    al_set_default_mixer(mixer_2);
    //al_reserve_samples(4);

    while(is_running() == true) {
        /* ***** Check audio messages and process ***** */
        if(!audio_messages.empty()) {
            //  Switch over the audio message and process
            switch(map_cmd_str_values[audio_messages.front().get_cmd()]) {
                //audio_messages.front().get_args()
                //audio_messages.front().get_split_args()

                //  cmd:  play_song - Play a song
                case CMD_STR_PLAY_SONG:
                    //  If something's playing, stop it first
                    if(al_get_mixer_playing(mixer_1)) al_destroy_audio_stream(stream);
                    //  Load stream and play
                    stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!stream) break;  //  Didn't load audio, end
                    al_attach_audio_stream_to_mixer(stream, mixer_1);
                    song_loaded = true;
                    song_paused = false;
                    break;

                //  cmd:  stop_song - Stop current song from playing
                case CMD_STR_STOP_SONG:
                    al_destroy_audio_stream(stream);
                    song_loaded = false;
                    break;

                //  cmd:  pause_song - Pause song if one is playing
                case CMD_STR_PAUSE_SONG:
                    if(song_loaded && al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(stream, false);
                        song_paused = true;
                    }
                    break;

                //  cmd:  unpause_song - Unpause song if one is paused
                case CMD_STR_UNPAUSE_SONG:
                    if(song_loaded && !al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(stream, true);
                        song_paused = false;
                    }
                    break;

                //  cmd:  new_cmd - description
                //case CMD_STR_X:
                    //
                    //break;
            }
            //  Remove processed message from the deck
            audio_messages.pop_front();
        }  //  End if(!audio_messages.empty())

        /* ***** Audio manager maintenance ***** */
        //  Keep loaded song playing on loop
        if(song_loaded && loop_song && !song_paused && !al_get_mixer_playing(mixer_1)) {
            al_rewind_audio_stream(stream);
            al_set_audio_stream_playing(stream, true);
        }

    }  //  End while(is_running() == true)

    //  Cleanup local objects
    al_destroy_audio_stream(stream);

    al_destroy_mixer(mixer_1);
    al_destroy_mixer(mixer_2);
    al_destroy_mixer(mixer_main);
    al_destroy_voice(voice);
}

} //  namespace mgr

} //  end namespace wte

#endif
