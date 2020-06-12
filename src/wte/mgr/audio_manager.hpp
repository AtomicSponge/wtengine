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
 * \bug Need to finish sample unloading & run tests
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

            //samples[WTE_MAX_SAMPLES] = { NULL };

            audio_messages.clear();
        };

        /*!
         * Uninstalls Allegro audio addons.
         * Clears the internal audio deck and audio command map.
         */
        inline ~audio_manager() {
            map_cmd_str_values.clear();
            audio_messages.clear();
        };

        /*!
         * Initialize audio manager.
         * \param void
         * \return void
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

            samples[WTE_MAX_SAMPLES] = { NULL };
        };

        /*!
         * de-init
         * \param void
         * \return void
         */
        inline void de_init(void) {
            //for(std::size_t pos = 0; pos < WTE_MAX_SAMPLES; pos++)
                //if(samples[pos] != NULL) al_destroy_sample(samples[pos]);

            if(al_get_mixer_attached(mixer_1)) al_destroy_audio_stream(music_stream);
            if(al_get_mixer_attached(mixer_3)) al_destroy_audio_stream(ambiance_stream);
            if(al_get_mixer_attached(mixer_4)) al_destroy_audio_stream(voice_stream);

            al_destroy_mixer(mixer_1);
            al_destroy_mixer(mixer_2);
            al_destroy_mixer(mixer_3);
            al_destroy_mixer(mixer_4);
            al_destroy_mixer(mixer_main);

            al_destroy_voice(voice);
        };

        /*!
         * Get the volume level of a mixer.
         * \param pos Mixer position.
         * \return Value of the mixer gain.
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
         * \param messages Vector of messages to be passed.
         * \return void
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
