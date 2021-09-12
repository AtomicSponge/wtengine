/*!
 * WTEngine | File:  audio.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
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

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/commands.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/config.hpp"

namespace wte {
    class engine;
}

namespace wte::mgr {

/*!
 * \class audio
 * \brief Handles audio playback.
 *
 * Messages get passed from the main engine loop via process_messages(). \n
 * Also can be controlled directly via function calls. \n
 * \n
 * The manager creates the following mixers: \n
 * Mixer Main - All other mixers attach to this. \n
 * Mixer 1 - Play music - Load a file and play in a loop.  Looping can be disabled. \n
 * Mixer 2 - Play samples - Has set number of samples that can be loaded in. \n
 * Mixer 3 - Play voice - Load a file and play once. \n
 * Mixer 4 - Play ambiance - Load a file and play in a loop.  Looping can be disabled.
 */
class audio final : private manager<audio> {
    friend class wte::engine;

    public:
        /*!
         * \struct music
         * \brief Music commands.
         */
        struct music {
            /*!
            * \brief Toggle music looping.
            * \param arg "enable" or "disable"
            */
            static void loop(
                const std::string& arg
            );

            /*!
            * \brief Play a music file.
            * \param fname Music filename.
            */
            static void play(
                const std::string& fname
            );

            /*!
            * \brief Stop playing music.
            */
            static void stop(void);

            /*!
            * \brief Pause music.
            */
            static void pause(void);

            /*!
            * \brief Unpause music.
            */
            static void unpause(void);
        };

        /*!
         * \struct sample
         * \brief Sample commands.
         */
        struct sample {
            /*!
            * \brief Load a sample.
            * \param fname Sample filename.
            * \param sname Sample reference name.
            */
            static void load(
                const std::string& fname,
                const std::string& sname
            );

            /*!
            * \brief Unload a sample.
            * \param sname Sample name or "all" for unload all samples.
            */
            static void unload(
                const std::string& sname
            );

            /*!
            * \brief Play a sample.
            *
            * When setting the playmode, passing "once" will play the sample once.
            * Passing a reference name will play the sample in a loop.
            * The sample can be stopped later using this reference name.
            *
            * \param sname Sample name.
            * \param ref Playmode.
            */
            static void play(
                const std::string& sname,
                const std::string& ref
            );

            /*!
            * \brief Play a sample.
            * \param sname Sample name.
            * \param ref Playmode.
            * \param gain Gain value.  See allegro docs on al_play_sample for more info.
            * \param pan Pan value.  See allegro docs on al_play_sample for more info.
            * \param speed Speed value.  See allegro docs on al_play_sample for more info.
            */
            static void play(
                const std::string& sname,
                const std::string& ref,
                const float& gain,
                const float& pan,
                const float& speed
            );

            /*!
            * \brief Play a sample.
            *
            * When setting the playmode, passing "once" will play the sample once.
            * Passing a reference name will play the sample in a loop.
            * The sample can be stopped later using this reference name.
            *
            * \param sample Sample asset.
            * \param ref Playmode.
            */
            static void play(
                wte_asset<al_sample> sample,
                const std::string& ref
            );

            /*!
            * \brief Play a sample.
            * \param sample Sample asset.
            * \param ref Playmode.
            * \param gain Gain value.  See allegro docs on al_play_sample for more info.
            * \param pan Pan value.  See allegro docs on al_play_sample for more info.
            * \param speed Speed value.  See allegro docs on al_play_sample for more info.
            */
            static void play(
                wte_asset<al_sample> sample,
                const std::string& ref,
                const float& gain,
                const float& pan,
                const float& speed
            );

            /*!
            * \brief Stop a playing sample.
            * \param ref Playing sample reference name.
            */
            static void stop(
                const std::string& ref
            );

            /*!
            * \brief Claer all playing sample instances.
            */
            static void clear_instances(void);
        };

        /*!
         * \struct voice
         * \brief Voice commands.
         */
        struct voice {
            /*!
            * \brief Play an audio file.
            * \param fname Filename to play.
            */
            static void play(
                const std::string& fname
            );

            /*!
            * \brief Stop a playing audio file.
            */
            static void stop(void);

            /*!
            * \brief Pause playing audio file.
            */
            static void pause(void);

            /*!
            * \brief Resume playing audio file.
            */
            static void unpause(void);
        };

        /*!
         * \struct ambiance
         * \brief Ambiance commands.
         */
        struct ambiance {
            /*!
            * \brief Toggle ambiance looping.
            * \param arg "enable" or "disable"
            */
            static void loop(
                const std::string& arg
            );

            /*!
            * \brief Play a file.
            * \param fname Ambiance filename.
            */
            static void play(
                const std::string& fname
            );

            /*!
            * \brief Stop playing ambiance.
            */
            static void stop(void);

            /*!
            * \brief Pause playing ambiance.
            */
            static void pause(void);

            /*!
            * \brief Resume playing ambiance.
            */
            static void unpause(void);
        };

        /*!
         * \brief Adjust a mixer's volume level.
         * \param m Mixer to adjust.
         * \param l Volume level to set.
         */
        static void set_level(
            const std::size_t& m,
            const float& l
        );

    private:
        audio() = default;
        ~audio() = default;

        /* 
         * Sets up the various Allegro objects for the audio manager to use.
         */
        static void initialize(void);

        /* 
         * Destroies the Allegro objects used by the manager.
         */
        static void de_init(void);

        /*
         * Process audio manager messages
         */
        static void process_messages(const message_container& messages);

        /*
         * Set volume levels based on engine cfg settings.
         */
        static void set_volume(void);

        static commands cmds;

        /* Allegro objects used by audio manager */
        //  Main audio output
        static ALLEGRO_VOICE* voice;

        //  Mixers
        static ALLEGRO_MIXER* mixer_main;
        static ALLEGRO_MIXER* mixer_1;
        static ALLEGRO_MIXER* mixer_2;
        static ALLEGRO_MIXER* mixer_3;
        static ALLEGRO_MIXER* mixer_4;

        // Streams
        static ALLEGRO_AUDIO_STREAM* music_stream;
        static ALLEGRO_AUDIO_STREAM* ambiance_stream;
        static ALLEGRO_AUDIO_STREAM* voice_stream;

        //  Store a reference of loaded samples.
        static std::map<std::string, ALLEGRO_SAMPLE*> sample_map;
        //  Store a reference of playing samples.
        static std::map<const std::string, ALLEGRO_SAMPLE_ID> sample_instances;
};

}  //  end namespace wte::mgr

#endif
