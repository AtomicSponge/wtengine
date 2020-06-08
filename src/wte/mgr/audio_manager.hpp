/*!
 * \brief WTEngine | File:  audio_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Handle playback of audio in its own thread.
 * Waits for messages to be loaded into the audio deck.
 * 
 * \bug debugging
 */

#ifndef WTE_MGR_AUDIO_MANAGER_HPP
#define WTE_MGR_AUDIO_MANAGER_HPP

#define ALLEGRO_UNSTABLE  //  For sample panning, see Allegro docs.

//  Set max number of samples.
#ifndef WTE_MAX_SAMPLES
#define WTE_MAX_SAMPLES 8
#endif

#include <string>
#include <map>
#include <deque>
#include <stdexcept>
//#include <mutex>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_physfs.h>

#include "manager.hpp"
#include "make_thread.hpp"
#include "../message.hpp"

namespace wte
{

namespace mgr
{

//!  Audio Manager
/*!
 * Handles audio messages in a thread.
 * Initializes the Allegro audio and Allegro codec addons durring construction.
 * Messages get passed from the main engine loop via transfer_messages().
 * The Audio Manager is then implemented as a thread, creating local Allegro objects.
 * As messages are placed in the deck, take them from the top and process.
 *
 * The manager creates the following mixers:
 * Mixer Main - All other mixers attach to this.
 * Mixer 1 - Play music - Load a file and play in a loop.  Looping can be disabled.
 * Mixer 2 - Play samples - Has set number of samples that can be loaded in.
 * Mixer 3 - Play voice - Load a file and play once.
 * Mixer 4 - Play ambiance - Load a file and play in a loop.  Looping can be disabled.
 */
class audio_manager final : public manager<audio_manager>, public make_thread {
    public:
        /*!
         * Configures the Allegro audio addons.
         * Clears the internal audio deck and maps the audio commands.
         */
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
            map_cmd_str_values["pan_sample"] = CMD_STR_PAN_SAMPLE;
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
            //  General
            map_cmd_str_values["set_volume"] = CMD_STR_SET_VOLUME;

            voice = NULL;
            mixer_main = NULL;
            mixer_1 = NULL;
            mixer_2 = NULL;
            mixer_3 = NULL;
            mixer_4 = NULL;

            music_stream = NULL;
            ambiance_stream = NULL;
            voice_stream = NULL;

            samples[WTE_MAX_SAMPLES] = { NULL };

            audio_messages.clear();
        };

        /*!
         * Uninstalls Allegro audio addons.
         * Clears the internal audio deck and audio command map.
         */
        inline ~audio_manager() {
            for(std::size_t pos = 0; pos < WTE_MAX_SAMPLES; pos++) al_destroy_sample(samples[pos]);

            al_destroy_audio_stream(music_stream);
            al_destroy_audio_stream(ambiance_stream);
            al_destroy_audio_stream(voice_stream);

            al_destroy_mixer(mixer_1);
            al_destroy_mixer(mixer_2);
            al_destroy_mixer(mixer_3);
            al_destroy_mixer(mixer_4);
            al_destroy_mixer(mixer_main);

            al_destroy_voice(voice);
            
            al_uninstall_audio();

            map_cmd_str_values.clear();
            audio_messages.clear();
        };

        /*!
         * Initialize audio manager.
         */
        inline void initialize(void) {
            voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
            mixer_main = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
            mixer_1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
            mixer_2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
            mixer_3 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
            mixer_4 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

            //  Set up the mixers.
            al_attach_mixer_to_voice(mixer_main, voice);
            al_attach_mixer_to_mixer(mixer_1, mixer_main);
            al_attach_mixer_to_mixer(mixer_2, mixer_main);
            al_attach_mixer_to_mixer(mixer_3, mixer_main);
            al_attach_mixer_to_mixer(mixer_4, mixer_main);

            al_set_default_mixer(mixer_2);
            al_reserve_samples(WTE_MAX_SAMPLES);
        };

        /*!
         * Get the volume level of a mixer.
         */
        /*inline const float get_volume(const std::size_t pos) {
            if(pos == 0) return al_get_mixer_gain(mixer_main);
            if(pos == 1) return al_get_mixer_gain(mixer_1);
            if(pos == 2) return al_get_mixer_gain(mixer_2);
            if(pos == 3) return al_get_mixer_gain(mixer_3);
            if(pos == 4) return al_get_mixer_gain(mixer_4);
            return -1.0f;
        };*/

        /*!
         * Take a vector of messages and pass them into the audio messages deck.
         */
        inline void transfer_messages(const message_container messages) {
            audio_messages.insert(audio_messages.end(),
                                  std::make_move_iterator(messages.begin()),
                                  std::make_move_iterator(messages.end()));
        };

    private:
        //!  Run the audio manager in a thread.
        void run(void) override;

        //  Used for switching on audio messages:
        enum CMD_STR_VALUE {
            //  Mixer 1
            CMD_STR_MUSIC_LOOP,
            CMD_STR_PLAY_MUSIC,      CMD_STR_STOP_MUSIC,
            CMD_STR_PAUSE_MUSIC,     CMD_STR_UNPAUSE_MUSIC,
            //  Mixer 2
            CMD_STR_LOAD_SAMPLE,     CMD_STR_UNLOAD_SAMPLE,
            CMD_STR_PLAY_SAMPLE,     CMD_STR_STOP_SAMPLE,
            CMD_STR_PAN_SAMPLE,
            //  Mixer 3
            CMD_STR_PLAY_VOICE,      CMD_STR_STOP_VOICE,
            CMD_STR_PAUSE_VOICE,     CMD_STR_UNPAUSE_VOICE,
            //  Mixer 4
            CMD_STR_AMBIANCE_LOOP,
            CMD_STR_PLAY_AMBIANCE,      CMD_STR_STOP_AMBIANCE,
            CMD_STR_PAUSE_AMBIANCE,     CMD_STR_UNPAUSE_AMBIANCE,
            //  General
            CMD_STR_SET_VOLUME
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        //  Deck of audio messages to be processed by the manager.
        std::deque<message> audio_messages;

        ALLEGRO_VOICE* voice;

        ALLEGRO_MIXER* mixer_main;
        ALLEGRO_MIXER* mixer_1;
        ALLEGRO_MIXER* mixer_2;
        ALLEGRO_MIXER* mixer_3;
        ALLEGRO_MIXER* mixer_4;

        ALLEGRO_AUDIO_STREAM* music_stream;
        ALLEGRO_AUDIO_STREAM* ambiance_stream;
        ALLEGRO_AUDIO_STREAM* voice_stream;

        ALLEGRO_SAMPLE* samples[WTE_MAX_SAMPLES];
};

//  Used to restrict class to a single instance.
template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

/*!
 * Waits for messages to be loaded into the internal queue then processes.
 * On startup, creates multiple mixer objects to play sound through, then allows playback
 * control via messages.
 */
inline void audio_manager::run(void) {
    //  Set PhysFS interface for the thread.
    //  PhysFS is initialized in the wte_main constructor.
    al_set_physfs_file_interface();

    al_set_default_mixer(mixer_main);

    //  Used to reference playing samples.
    ALLEGRO_SAMPLE_ID sample_id[WTE_MAX_SAMPLES];
    bool sample_playing[WTE_MAX_SAMPLES] = { false };
    ALLEGRO_SAMPLE_INSTANCE* instance = NULL;

    while(keep_running() == true) {
        std::size_t pos = 0;
        float gain = 0.0f;
        float pan = 0.0f;
        float speed = 0.0f;

        if(!audio_messages.empty()) {
            //  Switch over the audio message and process.
            switch(map_cmd_str_values[audio_messages.front().get_cmd()]) {
                /* ***  Mixer 1 - Music controls  *** */
                //  cmd:  music_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_MUSIC_LOOP:
                    if(!music_stream) break;  //  Music not loaded, end.
                    if(audio_messages.front().get_arg(0) == "enable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_arg(0) == "disable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;

                //  cmd:  play_music - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_MUSIC:
                    //  Load stream and play.
                    music_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!music_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(music_stream, mixer_1);
                    break;

                //  cmd:  stop_music - Stop current music from playing.
                case CMD_STR_STOP_MUSIC:
                    al_destroy_audio_stream(music_stream);
                    break;

                //  cmd:  pause_music - Pause music if it is playing.
                case CMD_STR_PAUSE_MUSIC:
                    //if(!music_stream) break;  //  Music not loaded, end.
                    al_set_audio_stream_playing(music_stream, false);
                    break;

                //  cmd:  unpause_music - Unpause music if it is paused.
                case CMD_STR_UNPAUSE_MUSIC:
                    //if(!music_stream) break;  //  Music not loaded, end.
                    al_set_audio_stream_playing(music_stream, true);
                    break;

                /* ***  Mixer 2 - Sample controls  *** */
                //  cmd:  load_sample - args:  sample_num (0 - MAX); file ; mode (once, loop) - Load a sample.
                //  Need to review
                case CMD_STR_LOAD_SAMPLE:
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    if(pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    //  Load sample from file
                    samples[pos] = al_load_sample(audio_messages.front().get_arg(1).c_str());
                    break;

                //  cmd:  unload_sample - arg:  sample_num (0 - MAX) - Unload sample if one is loaded.
                //  If arg == "all" unload all samples.
                case CMD_STR_UNLOAD_SAMPLE:
                    //  Unload all samples.
                    if(audio_messages.front().get_arg(0) == "all") {
                        for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) al_destroy_sample(samples[pos]);
                        break;
                    }
                    //  Unload a sample by position.
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    if(pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    al_destroy_sample(samples[pos]);
                    break;

                //  cmd:  play_sample - args:  sample_num (0 - MAX) ; mode (once, loop) ; gain ; pan ; speed
                //  Start playing loaded sample.
                case CMD_STR_PLAY_SAMPLE:
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    if(pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(!samples[pos]) break;  //  Sample not loaded, end.
                    //  Check to see if gain, pan or speed is set.
                    if(audio_messages.front().get_arg(2) != "") {
                        gain = std::atof(audio_messages.front().get_arg(2).c_str());
                        if(gain > 1.0f || gain <= 0.0f) gain = 1.0f;
                    } else gain = 1.0f;
                    if(audio_messages.front().get_arg(3) != "") {
                        pan = std::atof(audio_messages.front().get_arg(3).c_str());
                        if(pan < -1.0f || pan > 1.0f) pan = 0.0f;
                    } else pan = 0.0f;
                    if(audio_messages.front().get_arg(4) != "") {
                        speed = std::atof(audio_messages.front().get_arg(4).c_str());
                        if(speed > 1.0f || speed <= 0.0f) speed = 1.0f;
                    } else speed = 1.0f;
                    //  Play once or in a loop.
                    if(audio_messages.front().get_arg(1) == "once")
                        sample_playing[pos] = al_play_sample(samples[pos], gain, pan, speed,
                                                             ALLEGRO_PLAYMODE_ONCE, &sample_id[pos]);
                    else
                        sample_playing[pos] = al_play_sample(samples[pos], gain, pan, speed,
                                                             ALLEGRO_PLAYMODE_LOOP, &sample_id[pos]);
                    break;

                //  cmd:  stop_sample - arg:  sample_num (0 - MAX) - Stop playing loaded sample.
                case CMD_STR_STOP_SAMPLE:
                    if(audio_messages.front().get_arg(0) == "all") {
                        for(pos = 0; pos < WTE_MAX_SAMPLES; pos++) {
                            if(sample_playing[pos]) al_stop_sample(&sample_id[pos]);
                        }
                        break;
                    }
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    if(pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(sample_playing[pos]) al_stop_sample(&sample_id[pos]);
                    break;

                //  cmd:  pan_sample - arg:  sample_num (0 - MAX) ; pan ([left]-1.0 thru 1.0[right] or none) - Set sample pan.
                case CMD_STR_PAN_SAMPLE:
                    #ifdef ALLEGRO_UNSTABLE
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    if(pos >= WTE_MAX_SAMPLES) break;  //  Out of sample range, end.
                    if(!sample_playing[pos]) break;  //  Sample not loaded, end.
                    //  If arg == "none" set no panning
                    if(audio_messages.front().get_arg(1) == "none") {
                        instance = al_lock_sample_id(&sample_id[pos]);
                        if(instance) al_set_sample_instance_pan(instance, ALLEGRO_AUDIO_PAN_NONE);
                        al_unlock_sample_id(&sample_id[pos]);
                        break;
                    }
                    pan = std::atof(audio_messages.front().get_arg(1).c_str());
                    if(pan < -1.0f || pan > 1.0f) break;  //  Out of pan range
                    instance = al_lock_sample_id(&sample_id[pos]);
                    if(instance) al_set_sample_instance_pan(instance, pan);
                    al_unlock_sample_id(&sample_id[pos]);
                    #endif
                    break;

                /* ***  Mixer 3 - Voice controls  *** */
                //  cmd:  play_voice - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_VOICE:
                    //  Load stream and play.
                    voice_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!voice_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(voice_stream, ALLEGRO_PLAYMODE_ONCE);
                    al_attach_audio_stream_to_mixer(voice_stream, mixer_3);
                    break;

                //  cmd:  stop_voice - Stop current voice from playing.
                case CMD_STR_STOP_VOICE:
                    if(!voice_stream) break;  //  Voice not loaded, end.
                    al_destroy_audio_stream(voice_stream);
                    break;

                //  cmd:  pause_voice - Pause voice if it is playing.
                case CMD_STR_PAUSE_VOICE:
                    //if(!voice_stream) break;  //  Voice not loaded, end.
                    al_set_audio_stream_playing(voice_stream, false);
                    break;

                //  cmd:  unpause_voice - Unpause voice if it is paused.
                case CMD_STR_UNPAUSE_VOICE:
                    //if(!voice_stream) break;  //  Voice not loaded, end.
                    al_set_audio_stream_playing(voice_stream, true);
                    break;

                /* ***  Mixer 4 - Ambiance controls  *** */
                //  cmd:  ambiance_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_AMBIANCE_LOOP:
                    if(!ambiance_stream) break;  //  Ambiance not loaded, end.
                    if(audio_messages.front().get_arg(0) == "enable") al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_arg(0) == "disable") al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;

                //  cmd:  play_ambiance - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_AMBIANCE:
                    //  Load stream and play.
                    ambiance_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!ambiance_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(ambiance_stream, mixer_4);
                    break;

                //  cmd:  stop_ambiance - Stop current ambiance from playing.
                case CMD_STR_STOP_AMBIANCE:
                    al_destroy_audio_stream(ambiance_stream);
                    break;

                //  cmd:  pause_ambiance - Pause ambiance if it is playing.
                case CMD_STR_PAUSE_AMBIANCE:
                    //if(!ambiance_stream) break;  //  Ambiance not loaded, end.
                    al_set_audio_stream_playing(ambiance_stream, false);
                    break;

                //  cmd:  unpause_ambiance - Unpause ambiance if it is paused.
                case CMD_STR_UNPAUSE_AMBIANCE:
                    //if(!ambiance_stream) break;  //  Ambiance not loaded, end.
                    al_set_audio_stream_playing(ambiance_stream, true);
                    break;

                /* *** General commands *** */
                //  cmd:  set_volume - arg:  mixer # ; volume - Set the volume of a mixer.
                case CMD_STR_SET_VOLUME:
                    pos = std::stoi(audio_messages.front().get_arg(0));
                    float vol = std::atof(audio_messages.front().get_arg(1).c_str());
                    if(vol >= 0.0f && vol <= 1.0f) {
                        if(pos == 0) al_set_mixer_gain(mixer_main, vol);
                        if(pos == 1) al_set_mixer_gain(mixer_1, vol);
                        if(pos == 2) al_set_mixer_gain(mixer_2, vol);
                        if(pos == 3) al_set_mixer_gain(mixer_3, vol);
                        if(pos == 4) al_set_mixer_gain(mixer_4, vol);
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
    }  //  End while(is_running() == true)
    al_destroy_sample_instance(instance);
}

} //  namespace mgr

} //  end namespace wte

#endif
