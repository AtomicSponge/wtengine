/*!
 * WTEngine | File:  audio_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_AUDIO_MANAGER_HPP
#define WTE_MGR_AUDIO_MANAGER_HPP

//#define ALLEGRO_UNSTABLE  //  For sample panning, see Allegro docs.

#include <string>
#include <map>
#include <deque>
#include <mutex>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_physfs.h>

#include "manager.hpp"
#include "make_thread.hpp"
#include "../_globals/engine_cfg.hpp"
#include "../wte_global_defines.hpp"
#include "../message.hpp"

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
class audio_manager final : private manager<audio_manager>, public make_thread {
    public:
        /*!
         * \brief Audio Manager constructor.
         * 
         * Clears the internal audio deck and maps the audio commands.
         */
        inline audio_manager();

        /*!
         * \brief Audio Manager destructor.
         * 
         * Clears the internal audio deck and audio command map.
         */
        inline ~audio_manager();

        /*!
         * \brief Initialize audio manager.
         * 
         * Sets up the various Allegro objects for the audio manager to use.
         */
        inline void initialize(void);

        /*!
         * \brief De-initialize the audio manager.
         * 
         * Destroies the Allegro objects used by the manager.
         */
        inline void de_init(void);

        /*!
         * \brief Update engine cfg with the current volume levels.
         * 
         */
        inline void get_volume(void) const;

        /*!
         * \brief Take a vector of messages and pass them into the audio messages deck.
         * 
         * \param messages Vector of messages to be passed.
         */
        inline void transfer_messages(const message_container& messages);

    private:
        /*!
         * \brief Get sample name.
         * 
         * Pass the full file path and return just the name, no extension.
         * 
         * \param full_path Full filename including path.
         * \return Filename stripped of folder path and extension.
         */
        inline const std::string get_sample_name(const std::string& full_path);

        /*!
         * \brief Set volume levels based on engine cfg settings.
         */
        inline void set_volume(void);

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

        //  Deck of audio messages to be processed by the manager.
        std::deque<message> audio_messages;
        std::mutex deque_mtx;

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

//  Used to restrict class to a single instance.
template <> inline bool audio_manager::manager<audio_manager>::initialized = false;

} //  namespace mgr

} //  end namespace wte

#endif
