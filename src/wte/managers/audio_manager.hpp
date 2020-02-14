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
  Mixer 3 - Voice
  Mixer 4 - Play ambiance
*/
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        //!  Audio Manager Constructor.
        //!  Clears the internal audio deck.
        inline audio_manager() {
            map_cmd_str_values["music_loop"] = CMD_STR_MUSIC_LOOP;
            map_cmd_str_values["play_music"] = CMD_STR_PLAY_MUSIC;
            map_cmd_str_values["stop_music"] = CMD_STR_STOP_MUSIC;
            map_cmd_str_values["pause_music"] = CMD_STR_PAUSE_MUSIC;
            map_cmd_str_values["unpause_music"] = CMD_STR_UNPAUSE_MUSIC;

            map_cmd_str_values["play_voice"] = CMD_STR_PLAY_VOICE;
            map_cmd_str_values["stop_voice"] = CMD_STR_STOP_VOICE;
            map_cmd_str_values["pause_voice"] = CMD_STR_PAUSE_VOICE;
            map_cmd_str_values["unpause_voice"] = CMD_STR_UNPAUSE_VOICE;

            map_cmd_str_values["ambiance_loop"] = CMD_STR_AMBIANCE_LOOP;
            map_cmd_str_values["play_ambiance"] = CMD_STR_PLAY_AMBIANCE;
            map_cmd_str_values["stop_ambiance"] = CMD_STR_STOP_AMBIANCE;
            map_cmd_str_values["pause_ambiance"] = CMD_STR_PAUSE_AMBIANCE;
            map_cmd_str_values["unpause_ambiance"] = CMD_STR_UNPAUSE_AMBIANCE;
            audio_messages.clear();
            al_install_audio();
            al_init_acodec_addon();
        }

        //!  Audio Manager Destructor.
        //!  Clears the internal audio deck.
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
        //!  Run the audio manager.
        void run(void);

        //  Used for switching on audio messages:
        enum CMD_STR_VALUE {
            CMD_STR_MUSIC_LOOP,
            CMD_STR_PLAY_MUSIC,      CMD_STR_STOP_MUSIC,
            CMD_STR_PAUSE_MUSIC,     CMD_STR_UNPAUSE_MUSIC,

            CMD_STR_PLAY_VOICE,      CMD_STR_STOP_VOICE,
            CMD_STR_PAUSE_VOICE,     CMD_STR_UNPAUSE_VOICE,

            CMD_STR_AMBIANCE_LOOP,
            CMD_STR_PLAY_AMBIANCE,      CMD_STR_STOP_AMBIANCE,
            CMD_STR_PAUSE_AMBIANCE,     CMD_STR_UNPAUSE_AMBIANCE
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        std::deque<message> audio_messages;
};

template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

/*!
  Waits for messages to be loaded into the internal queue then processes.
  On startup, creates multiple mixer objects to play sound through, then allows playback
  control via messages.
*/
inline void audio_manager::run(void) {
    ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_main = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_3 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer_4 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    ALLEGRO_AUDIO_STREAM* music_stream = NULL;
    ALLEGRO_AUDIO_STREAM* ambiance_stream = NULL;
    ALLEGRO_AUDIO_STREAM* voice_stream = NULL;

    //  Music & ambiance looping on by default
    bool music_loaded = false, music_paused = false, loop_music = true;
    bool ambiance_loaded = false, ambiance_paused = false, loop_ambiance = true;
    bool voice_loaded = false, voice_paused = false;

    al_attach_mixer_to_voice(mixer_main, voice);
    al_attach_mixer_to_mixer(mixer_1, mixer_main);
    al_attach_mixer_to_mixer(mixer_2, mixer_main);
    al_attach_mixer_to_mixer(mixer_3, mixer_main);
    al_attach_mixer_to_mixer(mixer_4, mixer_main);

    //al_set_default_mixer(mixer_2);
    //al_reserve_samples(8);

    while(is_running() == true) {
        /* ***** Check audio messages and process ***** */
        if(!audio_messages.empty()) {
            //  Switch over the audio message and process
            switch(map_cmd_str_values[audio_messages.front().get_cmd()]) {
                //audio_messages.front().get_args()
                //audio_messages.front().get_split_args()

                /* ***  Music controls  *** */
                //  cmd:  music_loop - arg:  enable/disable - Turn music looping on or off
                case CMD_STR_MUSIC_LOOP:
                    if(audio_messages.front().get_args() == "enable") loop_music = true;
                    if(audio_messages.front().get_args() == "disable") loop_music = false;
                    break;

                //  cmd:  play_music - Load a file and play in a stream
                case CMD_STR_PLAY_MUSIC:
                    //  If something's playing, stop it first
                    if(al_get_mixer_playing(mixer_1)) al_destroy_audio_stream(music_stream);
                    //  Load stream and play
                    music_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!music_stream) break;  //  Didn't load audio, end
                    al_attach_audio_stream_to_mixer(music_stream, mixer_1);
                    music_loaded = true;
                    music_paused = false;
                    break;

                //  cmd:  stop_music - Stop current music from playing
                case CMD_STR_STOP_MUSIC:
                    al_destroy_audio_stream(music_stream);
                    music_loaded = false;
                    break;

                //  cmd:  pause_music - Pause music if it is playing
                case CMD_STR_PAUSE_MUSIC:
                    if(music_loaded && al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(music_stream, false);
                        music_paused = true;
                    }
                    break;

                //  cmd:  unpause_music - Unpause music if it is paused
                case CMD_STR_UNPAUSE_MUSIC:
                    if(music_loaded && !al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(music_stream, true);
                        music_paused = false;
                    }
                    break;

                /* ***  Voice controls  *** */
                //  cmd:  play_voice - Load a file and play in a stream
                case CMD_STR_PLAY_VOICE:
                    //  If something's playing, stop it first
                    if(al_get_mixer_playing(mixer_3)) al_destroy_audio_stream(voice_stream);
                    //  Load stream and play
                    voice_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!voice_stream) break;  //  Didn't load audio, end
                    al_attach_audio_stream_to_mixer(voice_stream, mixer_3);
                    voice_loaded = true;
                    voice_paused = false;
                    break;

                //  cmd:  stop_voice - Stop current voice from playing
                case CMD_STR_STOP_VOICE:
                    al_destroy_audio_stream(voice_stream);
                    voice_loaded = false;
                    break;

                //  cmd:  pause_voice - Pause voice if it is playing
                case CMD_STR_PAUSE_VOICE:
                    if(voice_loaded && al_get_mixer_playing(mixer_3)) {
                        al_set_audio_stream_playing(voice_stream, false);
                        voice_paused = true;
                    }
                    break;

                //  cmd:  unpause_voice - Unpause voice if it is paused
                case CMD_STR_UNPAUSE_VOICE:
                    if(voice_loaded && !al_get_mixer_playing(mixer_3)) {
                        al_set_audio_stream_playing(voice_stream, true);
                        voice_paused = false;
                    }
                    break;

                /* ***  Ambiance controls  *** */
                //  cmd:  ambiance_loop - arg:  enable/disable - Turn music looping on or off
                case CMD_STR_AMBIANCE_LOOP:
                    if(audio_messages.front().get_args() == "enable") loop_ambiance = true;
                    if(audio_messages.front().get_args() == "disable") loop_ambiance = false;
                    break;

                //  cmd:  play_ambiance - Load a file and play in a stream
                case CMD_STR_PLAY_AMBIANCE:
                    //  If something's playing, stop it first
                    if(al_get_mixer_playing(mixer_4)) al_destroy_audio_stream(ambiance_stream);
                    //  Load stream and play
                    ambiance_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!ambiance_stream) break;  //  Didn't load audio, end
                    al_attach_audio_stream_to_mixer(ambiance_stream, mixer_4);
                    ambiance_loaded = true;
                    ambiance_paused = false;
                    break;

                //  cmd:  stop_ambiance - Stop current ambiance from playing
                case CMD_STR_STOP_AMBIANCE:
                    al_destroy_audio_stream(ambiance_stream);
                    ambiance_loaded = false;
                    break;

                //  cmd:  pause_ambiance - Pause ambiance if it is playing
                case CMD_STR_PAUSE_AMBIANCE:
                    if(ambiance_loaded && al_get_mixer_playing(mixer_4)) {
                        al_set_audio_stream_playing(ambiance_stream, false);
                        ambiance_paused = true;
                    }
                    break;

                //  cmd:  unpause_ambiance - Unpause ambiance if it is paused
                case CMD_STR_UNPAUSE_AMBIANCE:
                    if(ambiance_loaded && !al_get_mixer_playing(mixer_4)) {
                        al_set_audio_stream_playing(ambiance_stream, true);
                        ambiance_paused = false;
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
        //  Keep loaded music playing on loop
        if(music_loaded && loop_music && !music_paused && !al_get_mixer_playing(mixer_1)) {
            al_rewind_audio_stream(music_stream);
            al_set_audio_stream_playing(music_stream, true);
        }
        //  Keep loaded ambiance playing on loop
        if(ambiance_loaded && loop_ambiance && !ambiance_paused && !al_get_mixer_playing(mixer_4)) {
            al_rewind_audio_stream(ambiance_stream);
            al_set_audio_stream_playing(ambiance_stream, true);
        }

    }  //  End while(is_running() == true)

    //  Cleanup local objects
    al_destroy_audio_stream(music_stream);
    al_destroy_audio_stream(ambiance_stream);
    al_destroy_audio_stream(voice_stream);

    al_destroy_mixer(mixer_1);
    al_destroy_mixer(mixer_2);
    al_destroy_mixer(mixer_3);
    al_destroy_mixer(mixer_4);
    al_destroy_mixer(mixer_main);

    al_destroy_voice(voice);
}

} //  namespace mgr

} //  end namespace wte

#endif
