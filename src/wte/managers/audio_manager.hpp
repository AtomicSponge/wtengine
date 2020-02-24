/*
  WTEngine
  By:  Matthew Evans
  File:  audio_manager.hpp

  See LICENSE.txt for copyright information

  Handle playback of audio in its own thread
  Waits for messages to be loaded into the audio deck

  NOTE:  NEEDS TESTING!!!
*/

#ifndef WTE_MGR_AUDIO_MANAGER_HPP
#define WTE_MGR_AUDIO_MANAGER_HPP

#define WTE_MAX_SAMPLES 8

#include <string>
#include <deque>
#include <stdexcept>

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
  Handles audio messages in a thread.
  Initializes the Allegro audio and Allegro codec addons durring construction.
  Messages get passed from the main engine loop via transfer_messages().
  The Audio Manager is then implemented as a thread, creating local Allegro objects.
  As messages are placed in the deck, take them from the top and process.

  The manager creates the following mixers:
  Mixer Main - All other mixers attach to this.
  Mixer 1 - Play music - Load a file and play in a loop.  Looping can be disabled.
  Mixer 2 - Play samples - Has set number of samples that can be loaded in.
  Mixer 3 - Play voice - Load a file and play once.
  Mixer 4 - Play ambiance - Load a file and play in a loop.  Looping can be disabled.
*/
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        //!  Configures the Allegro audio addons.
        //!  Clears the internal audio deck and maps the audio commands.
        inline audio_manager() {
            if(!al_install_audio()) throw std::runtime_error("Failed to load audio!");
            if(!al_init_acodec_addon()) throw std::runtime_error("Failed to load Allegro audio addon!");

            //  Map the audio commands.
            //  Mixer 1
            map_cmd_str_values["music_loop"] = CMD_STR_MUSIC_LOOP;
            map_cmd_str_values["play_music"] = CMD_STR_PLAY_MUSIC;
            map_cmd_str_values["stop_music"] = CMD_STR_STOP_MUSIC;
            map_cmd_str_values["pause_music"] = CMD_STR_PAUSE_MUSIC;
            map_cmd_str_values["unpause_music"] = CMD_STR_UNPAUSE_MUSIC;
            //  Mixer 2
            map_cmd_str_values["load_sample"] = CMD_STR_LOAD_SAMPLE;
            map_cmd_str_values["unload_sample"] = CMD_STR_UNLOAD_SAMPLE;
            map_cmd_str_values["play_sample"] = CMD_STR_PLAY_SAMPLE;
            map_cmd_str_values["stop_sample"] = CMD_STR_STOP_SAMPLE;
            //  Mixer 3
            map_cmd_str_values["play_voice"] = CMD_STR_PLAY_VOICE;
            map_cmd_str_values["stop_voice"] = CMD_STR_STOP_VOICE;
            map_cmd_str_values["pause_voice"] = CMD_STR_PAUSE_VOICE;
            map_cmd_str_values["unpause_voice"] = CMD_STR_UNPAUSE_VOICE;
            //  Mixer 4
            map_cmd_str_values["ambiance_loop"] = CMD_STR_AMBIANCE_LOOP;
            map_cmd_str_values["play_ambiance"] = CMD_STR_PLAY_AMBIANCE;
            map_cmd_str_values["stop_ambiance"] = CMD_STR_STOP_AMBIANCE;
            map_cmd_str_values["pause_ambiance"] = CMD_STR_PAUSE_AMBIANCE;
            map_cmd_str_values["unpause_ambiance"] = CMD_STR_UNPAUSE_AMBIANCE;

            audio_messages.clear();
        }

        //!  Uninstalls Allegro audio addons.
        //!  Clears the internal audio deck and audio command map.
        inline ~audio_manager() {
            al_uninstall_audio();
            map_cmd_str_values.clear();
            audio_messages.clear();
        }

        //!  Take a vector of messages and pass them into the audio messages deck.
        inline void transfer_messages(const message_container messages) {
            audio_messages.insert(audio_messages.end(),
                                  std::make_move_iterator(messages.begin()),
                                  std::make_move_iterator(messages.end()));
        }

    private:
        //!  Run the audio manager in a thread.
        void run(void);

        //  Used for switching on audio messages:
        enum CMD_STR_VALUE {
            //  Mixer 1
            CMD_STR_MUSIC_LOOP,
            CMD_STR_PLAY_MUSIC,      CMD_STR_STOP_MUSIC,
            CMD_STR_PAUSE_MUSIC,     CMD_STR_UNPAUSE_MUSIC,
            //  Mixer 2
            CMD_STR_LOAD_SAMPLE,     CMD_STR_UNLOAD_SAMPLE,
            CMD_STR_PLAY_SAMPLE,     CMD_STR_STOP_SAMPLE,
            //  Mixer 3
            CMD_STR_PLAY_VOICE,      CMD_STR_STOP_VOICE,
            CMD_STR_PAUSE_VOICE,     CMD_STR_UNPAUSE_VOICE,
            //  Mixer 4
            CMD_STR_AMBIANCE_LOOP,
            CMD_STR_PLAY_AMBIANCE,      CMD_STR_STOP_AMBIANCE,
            CMD_STR_PAUSE_AMBIANCE,     CMD_STR_UNPAUSE_AMBIANCE
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        //  Deck of audio messages to be processed by the manager.
        std::deque<message> audio_messages;
};

//  Used to restrict class to a single instance.
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

    struct al_samples {
        ALLEGRO_SAMPLE* sample;
    } AL_SAMPLES[WTE_MAX_SAMPLES];

    struct al_sample_instances {
        ALLEGRO_SAMPLE_INSTANCE* instance;
    } AL_SAMPLE_INSTANCES[WTE_MAX_SAMPLES];

    int pos = 0;

    //  Flags for checking various states.
    bool music_loaded = false, music_paused = false;
    bool ambiance_loaded = false, ambiance_paused = false;
    bool voice_loaded = false, voice_paused = false;

    bool sample_loaded[WTE_MAX_SAMPLES], sample_playing[WTE_MAX_SAMPLES];

    //  Set sample flags all to false.
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) sample_loaded[pos] = false;
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) sample_playing[pos] = false;

    //  Set up the mixers.
    al_attach_mixer_to_voice(mixer_main, voice);
    al_attach_mixer_to_mixer(mixer_1, mixer_main);
    al_attach_mixer_to_mixer(mixer_2, mixer_main);
    al_attach_mixer_to_mixer(mixer_3, mixer_main);
    al_attach_mixer_to_mixer(mixer_4, mixer_main);

    //  Create sample instances and attach to mixer 2.
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) AL_SAMPLES[pos].sample = NULL;
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++)
        AL_SAMPLE_INSTANCES[pos].instance = al_create_sample_instance(AL_SAMPLES[pos].sample);
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++)
        al_attach_sample_instance_to_mixer(AL_SAMPLE_INSTANCES[pos].instance, mixer_2);

    al_set_default_mixer(mixer_main);

    //al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
    //al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
    //al_set_audio_stream_playmode(voice_stream, ALLEGRO_PLAYMODE_ONCE);

    //  Reset pos
    pos = 0;

    while(is_running() == true) {
        /* ***** CHECK AUDIO MESSAGES AND PROCESS ***** */
        if(!audio_messages.empty()) {
            //  Switch over the audio message and process.
            switch(map_cmd_str_values[audio_messages.front().get_cmd()]) {
                /* ***  Mixer 1 - Music controls  *** */
                //  cmd:  music_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_MUSIC_LOOP:
                    if(!music_stream) break;
                    if(audio_messages.front().get_args() == "enable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_args() == "disable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;

                //  cmd:  play_music - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_MUSIC:
                    //  If something's playing, stop it first.
                    if(al_get_mixer_playing(mixer_1)) al_destroy_audio_stream(music_stream);
                    //  Load stream and play.
                    music_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!music_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(music_stream, mixer_1);
                    music_loaded = true;
                    music_paused = false;
                    break;

                //  cmd:  stop_music - Stop current music from playing.
                case CMD_STR_STOP_MUSIC:
                    al_destroy_audio_stream(music_stream);
                    music_loaded = false;
                    break;

                //  cmd:  pause_music - Pause music if it is playing.
                case CMD_STR_PAUSE_MUSIC:
                    if(music_loaded && al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(music_stream, false);
                        music_paused = true;
                    }
                    break;

                //  cmd:  unpause_music - Unpause music if it is paused.
                case CMD_STR_UNPAUSE_MUSIC:
                    if(music_loaded && !al_get_mixer_playing(mixer_1)) {
                        al_set_audio_stream_playing(music_stream, true);
                        music_paused = false;
                    }
                    break;

                /* ***  Mixer 2 - Sample controls  *** */
                //  cmd:  load_sample - args:  sample_num ; file - Load a sample.
                case CMD_STR_LOAD_SAMPLE:
                    pos = std::stoi(audio_messages.front().get_split_args()[0]);
                    if(pos < 0 || pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(sample_loaded[pos]) al_destroy_sample(AL_SAMPLES[pos].sample);
                    sample_loaded[pos] = false;
                    sample_playing[pos] = false;
                    AL_SAMPLES[pos].sample = al_load_sample(("data\\" + audio_messages.front().get_split_args()[1]).c_str());
                    if(!AL_SAMPLES[pos].sample) break;  //  Failed to load sample, end.
                    //al_set_sample(AL_SAMPLE_INSTANCES[pos].instance, AL_SAMPLES[pos].sample);
                    sample_loaded[pos] = true;
                    break;

                //  cmd:  unload_sample - arg:  sample_num - Unload sample if one is loaded.
                //  If arg == "all" unload all samples.
                case CMD_STR_UNLOAD_SAMPLE:
                    //  Unload all samples.
                    if(audio_messages.front().get_args() == "all") {
                        for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) {
                            al_destroy_sample(AL_SAMPLES[pos].sample);
                            sample_loaded[pos] = false;
                            sample_playing[pos] = false;
                        }
                        break;
                    }
                    //  Unload a sample by position.
                    pos = std::stoi(audio_messages.front().get_args());
                    if(pos < 0 || pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(!sample_loaded[pos]) break;  //  Sample not loaded, end.
                    al_destroy_sample(AL_SAMPLES[pos].sample);
                    sample_loaded[pos] = false;
                    sample_playing[pos] = false;
                    break;

                //  cmd:  play_sample - arg:  sample_num - Start playing loaded sample.
                case CMD_STR_PLAY_SAMPLE:
                    pos = std::stoi(audio_messages.front().get_args());
                    if(pos < 0 || pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(!sample_loaded[pos]) break;  //  Sample not loaded, end.
                    if(sample_playing[pos]) break;  //  Sample already playing, end.
                    al_play_sample_instance(AL_SAMPLE_INSTANCES[pos].instance);
                    sample_playing[pos] = true;
                    break;

                //  cmd:  stop_sample - arg:  sample_num - Stop playing loaded sample.
                case CMD_STR_STOP_SAMPLE:
                    pos = std::stoi(audio_messages.front().get_args());
                    if(pos < 0 || pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(!sample_loaded[pos]) break;  //  Sample not loaded, end.
                    if(!sample_playing[pos]) break;  //  Sample not playing, end.
                    al_stop_sample_instance(AL_SAMPLE_INSTANCES[pos].instance);
                    sample_playing[pos] = false;
                    break;

                /* ***  Mixer 3 - Voice controls  *** */
                //  cmd:  play_voice - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_VOICE:
                    //  If something's playing, stop it first.
                    if(al_get_mixer_playing(mixer_3)) al_destroy_audio_stream(voice_stream);
                    //  Load stream and play.
                    voice_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!voice_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(voice_stream, ALLEGRO_PLAYMODE_ONCE);
                    al_attach_audio_stream_to_mixer(voice_stream, mixer_3);
                    voice_loaded = true;
                    voice_paused = false;
                    break;

                //  cmd:  stop_voice - Stop current voice from playing.
                case CMD_STR_STOP_VOICE:
                    al_destroy_audio_stream(voice_stream);
                    voice_loaded = false;
                    break;

                //  cmd:  pause_voice - Pause voice if it is playing.
                case CMD_STR_PAUSE_VOICE:
                    if(voice_loaded && al_get_mixer_playing(mixer_3)) {
                        al_set_audio_stream_playing(voice_stream, false);
                        voice_paused = true;
                    }
                    break;

                //  cmd:  unpause_voice - Unpause voice if it is paused.
                case CMD_STR_UNPAUSE_VOICE:
                    if(voice_loaded && !al_get_mixer_playing(mixer_3)) {
                        al_set_audio_stream_playing(voice_stream, true);
                        voice_paused = false;
                    }
                    break;

                /* ***  Mixer 4 - Ambiance controls  *** */
                //  cmd:  ambiance_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_AMBIANCE_LOOP:
                    if(!ambiance_stream) break;
                    if(audio_messages.front().get_args() == "enable") al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_args() == "disable") al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;

                //  cmd:  play_ambiance - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_AMBIANCE:
                    //  If something's playing, stop it first.
                    if(al_get_mixer_playing(mixer_4)) al_destroy_audio_stream(ambiance_stream);
                    //  Load stream and play.
                    ambiance_stream = al_load_audio_stream(("data\\" + audio_messages.front().get_args()).c_str(), 4, 2048);
                    if(!ambiance_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(ambiance_stream, mixer_4);
                    ambiance_loaded = true;
                    ambiance_paused = false;
                    break;

                //  cmd:  stop_ambiance - Stop current ambiance from playing.
                case CMD_STR_STOP_AMBIANCE:
                    al_destroy_audio_stream(ambiance_stream);
                    ambiance_loaded = false;
                    break;

                //  cmd:  pause_ambiance - Pause ambiance if it is playing.
                case CMD_STR_PAUSE_AMBIANCE:
                    if(ambiance_loaded && al_get_mixer_playing(mixer_4)) {
                        al_set_audio_stream_playing(ambiance_stream, false);
                        ambiance_paused = true;
                    }
                    break;

                //  cmd:  unpause_ambiance - Unpause ambiance if it is paused.
                case CMD_STR_UNPAUSE_AMBIANCE:
                    if(ambiance_loaded && !al_get_mixer_playing(mixer_4)) {
                        al_set_audio_stream_playing(ambiance_stream, true);
                        ambiance_paused = false;
                    }
                    break;

                //  cmd:  new_cmd - description.
                //case CMD_STR_X:
                    //
                    //break;
            }
            //  Remove processed message from the deck.
            audio_messages.pop_front();
        }  //  End if(!audio_messages.empty())

        /* ***** AUDIO MANAGER MAINTENANCE ***** */
        //  Reset pos
        pos = 0;
    }  //  End while(is_running() == true)

    //  Cleanup local objects.
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) al_destroy_sample(AL_SAMPLES[pos].sample);
    for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) al_destroy_sample_instance(AL_SAMPLE_INSTANCES[pos].instance);

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
