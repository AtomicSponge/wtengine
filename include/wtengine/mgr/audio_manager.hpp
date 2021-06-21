/*!
 * WTEngine | File:  audio_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_AUDIO_MANAGER_HPP
#define WTE_MGR_AUDIO_MANAGER_HPP

//#define ALLEGRO_UNSTABLE  //  For sample panning, see Allegro docs.

#include <string>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/_globals/engine_cfg.hpp"
#include "wtengine/wte_global_defines.hpp"
#include "wtengine/message.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class audio_manager
 * \brief Handles audio messages in a thread.
 * 
 * Messages get passed from the main engine loop via transfer_messages(). \n 
 * The Audio Manager is then implemented as a thread, creating local Allegro objects. \n 
 * As messages are placed in the deck, take them from the top and process. \n 
 * \n 
 * The manager creates the following mixers: \n 
 * Mixer Main - All other mixers attach to this. \n 
 * Mixer 1 - Play music - Load a file and play in a loop.  Looping can be disabled. \n 
 * Mixer 2 - Play samples - Has set number of samples that can be loaded in. \n 
 * Mixer 3 - Play voice - Load a file and play once. \n 
 * Mixer 4 - Play ambiance - Load a file and play in a loop.  Looping can be disabled.
 */
//class audio_manager final : private manager<audio_manager>, public make_thread {
class audio_manager final : private manager<audio_manager> {
    public:
        /*!
         * \brief Audio Manager constructor.
         * 
         * Clears the internal audio deck and maps the audio commands.
         */
        audio_manager();

        /*!
         * \brief Audio Manager destructor.
         * 
         * Clears the internal audio deck and audio command map.
         */
        ~audio_manager();

        /*!
         * \brief Initialize audio manager.
         * 
         * Sets up the various Allegro objects for the audio manager to use.
         */
        void initialize(void);

        /*!
         * \brief De-initialize the audio manager.
         * 
         * Destroies the Allegro objects used by the manager.
         */
        void de_init(void);

        /*!
         * \brief Update engine cfg with the current volume levels.
         */
        void get_volume(void) const;

        /*!
         * \brief Process audio manager messages
         */
        void process_messages(const message_container& messages);

        /*!
         * \brief 
         */
        void music_loop(const std::string& arg);

        /*!
         * \brief 
         */
        void music_play(const std::string& arg);

        /*!
         * \brief 
         */
        void music_stop(void);

        /*!
         * \brief 
         */
        void music_pause(void);

        /*!
         * \brief 
         */
        void music_unpause(void);

        /*!
         * \brief 
         */
        void sample_load(const std::string& arg);

        /*!
         * \brief 
         */
        void sample_unload(const std::string& arg);

        /*!
         * \brief 
         */
        void sample_play(
            const std::string& arga,
            const std::string& argb,
            const float gain,
            const float pan,
            const float speed
        );

        /*!
         * \brief 
         */
        void sample_stop(const std::string& arg);

        /*!
         * \brief 
         */
        void sample_pan(void);

        /*!
         * \brief 
         */
        void sample_clear_instances(void);

        /*!
         * \brief 
         */
        void voice_play(const std::string& arg);

        /*!
         * \brief 
         */
        void voice_stop(void);

        /*!
         * \brief 
         */
        void voice_pause(void);

        /*!
         * \brief 
         */
        void voice_unpause(void);

        /*!
         * \brief 
         */
        void ambiance_loop(const std::string& arg);

        /*!
         * \brief 
         */
        void ambiance_play(const std::string& arg);

        /*!
         * \brief 
         */
        void ambiance_stop(void);

        /*!
         * \brief 
         */
        void ambiance_pause(void);

        /*!
         * \brief 
         */
        void ambiance_unpause(void);

    private:
        /*!
         * \brief Get sample name.
         * 
         * Pass the full file path and return just the name, no extension.
         * 
         * \param full_path Full filename including path.
         * \return Filename stripped of folder path and extension.
         */
        const std::string get_sample_name(const std::string& full_path);

        /*!
         * \brief Set volume levels based on engine cfg settings.
         */
        void set_volume(void);

        //  Used for switching on audio messages:
        enum CMD_STR_VALUE {
            //  Mixer 1
            CMD_STR_MUSIC_LOOP,
            CMD_STR_PLAY_MUSIC,      CMD_STR_STOP_MUSIC,
            CMD_STR_PAUSE_MUSIC,     CMD_STR_UNPAUSE_MUSIC,
            //  Mixer 2
            CMD_STR_LOAD_SAMPLE,     CMD_STR_UNLOAD_SAMPLE,
            CMD_STR_PLAY_SAMPLE,     CMD_STR_STOP_SAMPLE,
            CMD_STR_PAN_SAMPLE,      CMD_STR_CLEAR_INSTANCES,
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

        /* Allegro objects used by audio manager */
        //  Main audio output
        ALLEGRO_VOICE* voice;

        //  Mixers
        ALLEGRO_MIXER* mixer_main;
        ALLEGRO_MIXER* mixer_1;
        ALLEGRO_MIXER* mixer_2;
        ALLEGRO_MIXER* mixer_3;
        ALLEGRO_MIXER* mixer_4;

        // Streams
        ALLEGRO_AUDIO_STREAM* music_stream;
        ALLEGRO_AUDIO_STREAM* ambiance_stream;
        ALLEGRO_AUDIO_STREAM* voice_stream;

        //  Store a reference of loaded samples.
        std::map<std::string, ALLEGRO_SAMPLE*> sample_map;
        //  Store a reference of playing samples.
        std::map<std::string, ALLEGRO_SAMPLE_ID> sample_instances;
};

} //  namespace mgr

} //  end namespace wte

#endif
