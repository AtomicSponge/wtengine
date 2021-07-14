/*!
 * WTEngine | File:  audio.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_AUDIO_HPP
#define WTE_MGR_AUDIO_HPP

#include <string>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "wtengine/config.hpp"
#include "wtengine/mgr/manager.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/message.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class audio
 * \brief Handles audio messages.
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
class audio final : private manager<audio> {
    friend class interface;

    public:
        /*!
         * \brief 
         */
        static void music_loop(const std::string& arg);

        /*!
         * \brief 
         */
        static void music_play(const std::string& arg);

        /*!
         * \brief 
         */
        static void music_stop(void);

        /*!
         * \brief 
         */
        static void music_pause(void);

        /*!
         * \brief 
         */
        static void music_unpause(void);

        /*!
         * \brief 
         */
        static void sample_load(const std::string& arg);

        /*!
         * \brief 
         */
        static void sample_unload(const std::string& arg);

        /*!
         * \brief 
         */
        static void sample_play(
            const std::string& arga,
            const std::string& argb
        );

        /*!
         * \brief 
         */
        static void sample_play(
            const std::string& arga,
            const std::string& argb,
            const float& gain,
            const float& pan,
            const float& speed
        );

        /*!
         * \brief 
         */
        static void sample_stop(const std::string& arg);

        /*!
         * \brief 
         */
        static void sample_clear_instances(void);

        /*!
         * \brief 
         */
        static void voice_play(const std::string& arg);

        /*!
         * \brief 
         */
        static void voice_stop(void);

        /*!
         * \brief 
         */
        static void voice_pause(void);

        /*!
         * \brief 
         */
        static void voice_unpause(void);

        /*!
         * \brief 
         */
        static void ambiance_loop(const std::string& arg);

        /*!
         * \brief 
         */
        static void ambiance_play(const std::string& arg);

        /*!
         * \brief 
         */
        static void ambiance_stop(void);

        /*!
         * \brief 
         */
        static void ambiance_pause(void);

        /*!
         * \brief 
         */
        static void ambiance_unpause(void);

    private:
        /*!
         * \brief Audio Manager constructor.
         * 
         * Clears the internal audio deck and maps the audio commands.
         */
        audio();

        /*!
         * \brief Audio Manager destructor.
         * 
         * Clears the internal audio deck and audio command map.
         */
        ~audio();

        /*!
         * \brief Initialize audio manager.
         * 
         * Sets up the various Allegro objects for the audio manager to use.
         */
        static void initialize(void);

        /*!
         * \brief De-initialize the audio manager.
         * 
         * Destroies the Allegro objects used by the manager.
         */
        static void de_init(void);

        /*!
         * \brief Process audio manager messages
         */
        static void process_messages(const message_container& messages);

        /*!
         * \brief Get sample name.
         * 
         * Pass the full file path and return just the name, no extension.
         * 
         * \param full_path Full filename including path.
         * \return Filename stripped of folder path and extension.
         */
        static const std::string get_sample_name(const std::string& full_path);

        /*!
         * \brief Set volume levels based on engine cfg settings.
         */
        static void set_volume(void);

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
        inline static std::map<std::string, CMD_STR_VALUE> map_cmd_str_values = {};

        /* Allegro objects used by audio manager */
        //  Main audio output
        inline static ALLEGRO_VOICE* voice = NULL;

        //  Mixers
        inline static ALLEGRO_MIXER* mixer_main = NULL;
        inline static ALLEGRO_MIXER* mixer_1 = NULL;
        inline static ALLEGRO_MIXER* mixer_2 = NULL;
        inline static ALLEGRO_MIXER* mixer_3 = NULL;
        inline static ALLEGRO_MIXER* mixer_4 = NULL;

        // Streams
        inline static ALLEGRO_AUDIO_STREAM* music_stream = NULL;
        inline static ALLEGRO_AUDIO_STREAM* ambiance_stream = NULL;
        inline static ALLEGRO_AUDIO_STREAM* voice_stream = NULL;

        //  Store a reference of loaded samples.
        inline static std::map<std::string, ALLEGRO_SAMPLE*> sample_map = {};
        //  Store a reference of playing samples.
        inline static std::map<std::string, ALLEGRO_SAMPLE_ID> sample_instances = {};
};

} //  namespace mgr

} //  end namespace wte

#endif
