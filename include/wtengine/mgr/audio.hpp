/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_AUDIO_HPP)
#define WTE_MGR_AUDIO_HPP

#include <string>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/commands.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/assets.hpp"
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

  private:
    audio() = default;
    ~audio() = default;

    //  Sets up the various Allegro objects for the audio manager to use.
    static void initialize(void) {
      voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
      _mixer_main = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_1 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_1_a = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_1_b = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_3 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
      _mixer_4 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

      //  Set up the mixers.
      al_attach_mixer_to_voice(_mixer_main, voice);
      al_attach_mixer_to_mixer(_mixer_1, _mixer_main);
      al_attach_mixer_to_mixer(_mixer_1_a, _mixer_1);
      al_attach_mixer_to_mixer(_mixer_1_b, _mixer_1);
      al_attach_mixer_to_mixer(_mixer_2, _mixer_main);
      al_attach_mixer_to_mixer(_mixer_3, _mixer_main);
      al_attach_mixer_to_mixer(_mixer_4, _mixer_main);

      //  Set number of samples.
      al_set_default_mixer(_mixer_2);
      al_reserve_samples(max_playing_samples);

      //  Set volume levels.
      config::_volume::music_a = 1.0f;
      config::_volume::music_b = 1.0f;
      set_volume();

      //  Map the audio commands.
      //  Mixer 1
      cmds.add("music-loop", 2, [](const msg_args& args) {
        if (args[0] == "a") {
          if (args[1] == "disable") audio::music::a::loop(false);
          else audio::music::a::loop(true);
        }
        if (args[0] == "b") {
          if (args[1] == "disable") audio::music::b::loop(false);
          else audio::music::b::loop(true);
        }
      });
      cmds.add("music-play", 2, [](const msg_args& args) {
        if (args[0] == "a")
          audio::music::a::play(mgr::assets::get<ALLEGRO_AUDIO_STREAM>(args[1]));
        if (args[0] == "b")
          audio::music::b::play(mgr::assets::get<ALLEGRO_AUDIO_STREAM>(args[1]));
      });
      cmds.add("music-stop", 1, [](const msg_args& args) {
        if (args[0] == "a") audio::music::a::stop();
        if (args[0] == "b") audio::music::b::stop();
      });
      cmds.add("music-pause", 1, [](const msg_args& args) {
        if (args[0] == "a") audio::music::a::pause();
        if (args[0] == "b") audio::music::b::pause();
      });
      cmds.add("music-unpause", 1, [](const msg_args& args) {
        if (args[0] == "a") audio::music::a::unpause();
        if (args[0] == "b") audio::music::a::unpause();
      });
      //  Mixer 2
      cmds.add("sample-play", 2, [](const msg_args& args) {
        float gain = 1.0f;
        float pan = ALLEGRO_AUDIO_PAN_NONE;
        float speed = 1.0f;

        if (args.size() >= 3) {
          gain = std::stof(args[2]);
          if (gain < 0.0f || gain > 1.0f) gain = 1.0f;
        }
        if (args.size() >= 4) {
          pan = std::stof(args[3]);
          if (pan < -1.0f || pan > 1.0f) pan = ALLEGRO_AUDIO_PAN_NONE;
        }
        if (args.size() >= 5) {
          speed = std::stof(args[4]);
          if (speed <= 0.0f || speed > 2.0f) speed = 1.0f;
        }
        audio::sample::play(
          mgr::assets::get<ALLEGRO_SAMPLE>(args[0]),
          args[1], gain, pan, speed
        );
      });
      cmds.add("sample-stop", 1, [](const msg_args& args) {
        audio::sample::stop(args[0]);
      });
      cmds.add("sample-clear-instances", 0, [](const msg_args& args) {
        audio::sample::clear_instances();
      });
      //  Mixer 3
      cmds.add("voice-play", 1, [](const msg_args& args) {
        audio::voice::play(mgr::assets::get<ALLEGRO_AUDIO_STREAM>(args[0]));
      });
      cmds.add("voice-stop", 0, [](const msg_args& args) {
        audio::voice::stop();
      });
      cmds.add("voice-pause", 0, [](const msg_args& args) {
        audio::voice::pause();
      });
      cmds.add("voice-unpause", 0, [](const msg_args& args) {
        audio::voice::unpause();
      });
      //  Mixer 4
      cmds.add("ambiance-loop", 1, [](const msg_args& args) {
        if (args[0] == "disable") audio::ambiance::loop(false);
        else audio::ambiance::loop(true);
      });
      cmds.add("ambiance-play", 1, [](const msg_args& args) {
        audio::ambiance::play(mgr::assets::get<ALLEGRO_AUDIO_STREAM>(args[0]));
      });
      cmds.add("ambiance-stop", 0, [](const msg_args& args) {
        audio::ambiance::stop();
      });
      cmds.add("ambiance-pause", 0, [](const msg_args& args) {
        audio::ambiance::pause();
      });
      cmds.add("ambiance-unpause", 0, [](const msg_args& args) {
        audio::ambiance::unpause();
      });
      //  General
      cmds.add("set-volume-level", 2, [](const msg_args& args) {
        switch(std::stoi(args[0])) {
          case 0:
            audio::set_level(std::stof(args[1]));
            break;
          case 1:
            audio::music::set_level(std::stof(args[1]));
            break;
          case 2:
            audio::sample::set_level(std::stof(args[1]));
            break;
          case 3:
            audio::voice::set_level(std::stof(args[1]));
            break;
          case 4:
            audio::ambiance::set_level(std::stof(args[1]));
            break;
        }
      });
    };

    //  Destroies the Allegro objects used by the manager.
    static void de_init(void) {
      audio::sample::clear_instances();
      audio::music::a::stop();
      audio::music::b::stop();
      audio::voice::stop();
      audio::ambiance::stop();

      //  Unload all mixers.
      al_destroy_mixer(_mixer_1_a);
      al_destroy_mixer(_mixer_1_b);
      al_destroy_mixer(_mixer_1);
      al_destroy_mixer(_mixer_2);
      al_destroy_mixer(_mixer_3);
      al_destroy_mixer(_mixer_4);
      al_destroy_mixer(_mixer_main);

      //  Unload main audio output.
      al_destroy_voice(voice);

      music_stream_a.reset();
      music_stream_b.reset();
      ambiance_stream.reset();
      voice_stream.reset();
    };

    //  Process audio manager messages
    static void process_messages(const message_container& messages) {
      cmds.process_messages(messages);
    };

    //  Set volume levels based on engine cfg settings.
    static void set_volume(void) {
      (config::volume::main >= 0.0f && config::volume::main <= 1.0f ?
        al_set_mixer_gain(_mixer_main, config::volume::main) :
        al_set_mixer_gain(_mixer_main, 0.5f));
      (config::volume::music >= 0.0f && config::volume::music <= 1.0f ?
        al_set_mixer_gain(_mixer_1, config::volume::music) :
        al_set_mixer_gain(_mixer_1, 0.5f));
      (config::volume::music_a >= 0.0f && config::volume::music_a <= 1.0f ?
        al_set_mixer_gain(_mixer_1_a, config::volume::music_a) :
        al_set_mixer_gain(_mixer_1_a, 0.5f));
      (config::volume::music_b >= 0.0f && config::volume::music_b <= 1.0f ?
        al_set_mixer_gain(_mixer_1_b, config::volume::music_b) :
        al_set_mixer_gain(_mixer_1_b, 0.5f));
      (config::volume::sample >= 0.0f && config::volume::sample <= 1.0f ?
        al_set_mixer_gain(_mixer_2, config::volume::sample) :
        al_set_mixer_gain(_mixer_2, 0.5f));
      (config::volume::voice >= 0.0f && config::volume::voice <= 1.0f ?
        al_set_mixer_gain(_mixer_3, config::volume::voice) :
        al_set_mixer_gain(_mixer_3, 0.5f));
      (config::volume::ambiance >= 0.0f && config::volume::ambiance <= 1.0f ?
        al_set_mixer_gain(_mixer_4, config::volume::ambiance) :
        al_set_mixer_gain(_mixer_4, 0.5f));
    };

    constexpr static int max_playing_samples = build_options.max_playing_samples;
    
    inline static commands cmds;

    //  Main audio output
    inline static ALLEGRO_VOICE* voice;

    //  Mixers
    inline static ALLEGRO_MIXER* _mixer_main = NULL;
    inline static ALLEGRO_MIXER* _mixer_1 = NULL;
    inline static ALLEGRO_MIXER* _mixer_1_a = NULL;
    inline static ALLEGRO_MIXER* _mixer_1_b = NULL;
    inline static ALLEGRO_MIXER* _mixer_2 = NULL;
    inline static ALLEGRO_MIXER* _mixer_3 = NULL;
    inline static ALLEGRO_MIXER* _mixer_4 = NULL;

    // Streams
    inline static wte_asset<ALLEGRO_AUDIO_STREAM> music_stream_a = nullptr;
    inline static wte_asset<ALLEGRO_AUDIO_STREAM> music_stream_b = nullptr;
    inline static wte_asset<ALLEGRO_AUDIO_STREAM> ambiance_stream = nullptr;
    inline static wte_asset<ALLEGRO_AUDIO_STREAM> voice_stream = nullptr;

    //  Store a reference of playing samples.
    inline static message_container _messages;
    //  Vector of all messages to be processed
    inline static std::map<const std::string, ALLEGRO_SAMPLE_ID> sample_instances;

  public:
    /*!
     * \brief Adjust main mixer volume level.
     * \param l Volume level to set.
     */
    static void set_level(const float& l) {
      (l >= 0.0f && l <= 1.0f ?
        config::_volume::main = l :
        config::_volume::main = 0.0f);
      set_volume();
    };

    /*!
     * \struct music
     * \brief Music commands.
     */
    struct music {
      /*!
       * \brief Adjust the music mixer volume level.
       * \param l Volume level to set.
       */
      static void set_level(const float& l) {
        (l >= 0.0f && l <= 1.0f ?
          config::_volume::music = l :
          config::_volume::music = 0.0f);
        set_volume();
      };
      /*!
       * \struct a
       * \brief Music track A
       */
      struct a {
        /*!
         * \brief Toggle music looping.
         * \param loop True to enable, false to disable.
         */
        static void loop(const bool& loop) {
          if (!al_get_mixer_attached(_mixer_1)) return;  //  Music not loaded, end.
          (loop ? al_set_audio_stream_playmode(music_stream_a.get(), ALLEGRO_PLAYMODE_LOOP) :
            al_set_audio_stream_playmode(music_stream_a.get(), ALLEGRO_PLAYMODE_ONCE));
        };

        /*!
         * \brief Play a music asset.
         * \param audio Audio asset.
         */
        static void play(wte_asset<ALLEGRO_AUDIO_STREAM> audio) {
          music::a::stop();
          music_stream_a = audio;
          al_attach_audio_stream_to_mixer(music_stream_a.get(), _mixer_1_a);
          al_set_audio_stream_playmode(music_stream_a.get(), ALLEGRO_PLAYMODE_LOOP);
          al_rewind_audio_stream(music_stream_a.get());
          al_set_audio_stream_playing(music_stream_a.get(), true);
        };

        /*!
         * \brief Stop playing music.
         */
        static void stop(void) {
          if (al_get_mixer_attached(_mixer_1_a)) {
            al_set_audio_stream_playing(music_stream_a.get(), false);
            al_drain_audio_stream(music_stream_a.get());
            al_detach_audio_stream(music_stream_a.get());
          }
        };

        /*!
         * \brief Pause music.
         */
        static void pause(void) {
          if (al_get_mixer_attached(_mixer_1_a) && al_get_mixer_playing(_mixer_1_a))
            al_set_audio_stream_playing(music_stream_a.get(), false);
        };

        /*!
         * \brief Unpause music.
         */
        static void unpause(void) {
          if (al_get_mixer_attached(_mixer_1)) al_set_audio_stream_playing(music_stream_a.get(), true);
        };

        /*!
         * \brief Adjust music track A mixer volume level.
         * \param l Volume level to set.
         */
        static void set_level(const float& l) {
          (l >= 0.0f && l <= 1.0f ?
            config::_volume::music_a = l :
            config::_volume::music_a = 0.0f);
          set_volume();
        };
      };

      /*!
       * \struct b
       * \brief Music track B
       */
      struct b {
        /*!
         * \brief Toggle music looping.
         * \param loop True to enable, false to disable.
         */
        static void loop(const bool& loop) {
          if (!al_get_mixer_attached(_mixer_1_a)) return;  //  Music not loaded, end.
          (loop ? al_set_audio_stream_playmode(music_stream_a.get(), ALLEGRO_PLAYMODE_LOOP) :
            al_set_audio_stream_playmode(music_stream_a.get(), ALLEGRO_PLAYMODE_ONCE));
        };

        /*!
         * \brief Play a music asset.
         * \param audio Audio asset.
         */
        static void play(wte_asset<ALLEGRO_AUDIO_STREAM> audio) {
          music::b::stop();
          music_stream_b = audio;
          al_attach_audio_stream_to_mixer(music_stream_b.get(), _mixer_1_b);
          al_set_audio_stream_playmode(music_stream_b.get(), ALLEGRO_PLAYMODE_LOOP);
          al_rewind_audio_stream(music_stream_b.get());
          al_set_audio_stream_playing(music_stream_b.get(), true);
        };

        /*!
         * \brief Stop playing music.
         */
        static void stop(void) {
          if (al_get_mixer_attached(_mixer_1_b)) {
            al_set_audio_stream_playing(music_stream_b.get(), false);
            al_drain_audio_stream(music_stream_b.get());
            al_detach_audio_stream(music_stream_b.get());
          }
        };

        /*!
         * \brief Pause music.
         */
        static void pause(void) {
          if (al_get_mixer_attached(_mixer_1_b) && al_get_mixer_playing(_mixer_1_b))
            al_set_audio_stream_playing(music_stream_b.get(), false);
        };

        /*!
         * \brief Unpause music.
         */
        static void unpause(void) {
          if (al_get_mixer_attached(_mixer_1_b)) al_set_audio_stream_playing(music_stream_b.get(), true);
        };

        /*!
         * \brief Adjust music track B mixer volume level.
         * \param l Volume level to set.
         */
        static void set_level(const float& l) {
          (l >= 0.0f && l <= 1.0f ?
            config::_volume::music_b = l :
            config::_volume::music_b = 0.0f);
          set_volume();
        };
      };
    };

    /*!
     * \struct sample
     * \brief Sample commands.
     */
    struct sample {
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
        wte_asset<ALLEGRO_SAMPLE> sample,
        const std::string& ref
      ) { play(sample, ref, 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f); };

      /*!
       * \brief Play a sample.
       * \param sample Sample asset.
       * \param ref Playmode.
       * \param gain Gain value.  See allegro docs on al_play_sample for more info.
       * \param pan Pan value.  See allegro docs on al_play_sample for more info.
       * \param speed Speed value.  See allegro docs on al_play_sample for more info.
       */
      static void play(
        wte_asset<ALLEGRO_SAMPLE> sample,
        const std::string& ref,
        const float& gain,
        const float& pan,
        const float& speed
      ) {
        if (ref == "once") {
          // Play the sample once.
          al_play_sample(sample.get(), gain, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
        } else {
          //  If the reference is already playing, end.
          if (sample_instances.find(ref) != sample_instances.end()) return;
          //  Store playing reference
          ALLEGRO_SAMPLE_ID temp_sample_id;
          if (al_play_sample(sample.get(), gain, pan, speed, ALLEGRO_PLAYMODE_LOOP, &temp_sample_id))
            sample_instances.insert(std::make_pair(ref, temp_sample_id));
        }
      };

      /*!
       * \brief Stop a playing sample.
       * \param ref Playing sample reference name.
       */
      static void stop(const std::string& ref) {
        //  If instance does not exist, end.
        if (sample_instances.find(ref) == sample_instances.end()) return;
        al_stop_sample(&sample_instances.find(ref)->second);
        sample_instances.erase(sample_instances.find(ref));
      };

      /*!
       * \brief Claer all playing sample instances.
       */
      static void clear_instances(void) {
        for(auto sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
          al_stop_sample(&sample_instance->second);
          sample_instances.erase(sample_instance);
          sample_instance = sample_instances.begin();
        }
      };

      /*!
       * \brief Adjust the sample mixer volume level.
       * \param l Volume level to set.
       */
      static void set_level(const float& l) {
        (l >= 0.0f && l <= 1.0f ?
          config::_volume::sample = l :
          config::_volume::sample = 0.0f);
        set_volume();
      };
    };

    /*!
     * \struct voice
     * \brief Voice commands.
     */
    struct voice {
      /*!
       * \brief Play an audio asset on the voice channel.
       * \param audio Audio asset.
       */
      static void play(wte_asset<ALLEGRO_AUDIO_STREAM> audio) {
        voice::stop();
        voice_stream = audio;
        al_attach_audio_stream_to_mixer(voice_stream.get(), _mixer_3);
        al_set_audio_stream_playmode(voice_stream.get(), ALLEGRO_PLAYMODE_ONCE);
        al_rewind_audio_stream(voice_stream.get());
        al_set_audio_stream_playing(voice_stream.get(), true);
      };

      /*!
       * \brief Stop a playing audio file.
       */
      static void stop(void) {
        if (al_get_mixer_attached(_mixer_3)) {
          al_set_audio_stream_playing(voice_stream.get(), false);
          al_drain_audio_stream(voice_stream.get());
          al_detach_audio_stream(voice_stream.get());
        }
      };

      /*!
       * \brief Pause playing audio file.
       */
      static void pause(void) {
        if (al_get_mixer_attached(_mixer_3) && al_get_mixer_playing(_mixer_3))
          al_set_audio_stream_playing(voice_stream.get(), false);
      };

      /*!
       * \brief Resume playing audio file.
       */
      static void unpause(void) {
        if (al_get_mixer_attached(_mixer_3)) al_set_audio_stream_playing(voice_stream.get(), true);
      };

      /*!
       * \brief Adjust the voice mixer volume level.
       * \param l Volume level to set.
       */
      static void set_level(const float& l) {
        (l >= 0.0f && l <= 1.0f ?
          config::_volume::voice = l :
          config::_volume::voice = 0.0f);
        set_volume();
      };
    };

    /*!
     * \struct ambiance
     * \brief Ambiance commands.
     */
    struct ambiance {
      /*!
       * \brief Toggle ambiance looping.
       * \param loop True to enable, false to disable.
       */
      static void loop(const bool& loop) {
        if (!al_get_mixer_attached(_mixer_4)) return;  //  Ambiance not loaded, end.
        (loop ? al_set_audio_stream_playmode(ambiance_stream.get(), ALLEGRO_PLAYMODE_LOOP) :
          al_set_audio_stream_playmode(ambiance_stream.get(), ALLEGRO_PLAYMODE_ONCE));
      };

      /*!
       * \brief Play an audio asset on the ambiance channel.
       * \param audio Audio asset.
       */
      static void play(wte_asset<ALLEGRO_AUDIO_STREAM> audio) {
        ambiance::stop();
        ambiance_stream = audio;
        al_attach_audio_stream_to_mixer(ambiance_stream.get(), _mixer_4);
        al_set_audio_stream_playmode(ambiance_stream.get(), ALLEGRO_PLAYMODE_LOOP);
        al_rewind_audio_stream(ambiance_stream.get());
        al_set_audio_stream_playing(ambiance_stream.get(), true);
      };

      /*!
       * \brief Stop playing ambiance.
       */
      static void stop(void) {
        if (al_get_mixer_attached(_mixer_4)) {
          al_set_audio_stream_playing(ambiance_stream.get(), false);
          al_drain_audio_stream(ambiance_stream.get());
          al_detach_audio_stream(ambiance_stream.get());
        }
      };

      /*!
       * \brief Pause playing ambiance.
       */
      static void pause(void) {
        if (al_get_mixer_attached(_mixer_4) && al_get_mixer_playing(_mixer_4))
          al_set_audio_stream_playing(ambiance_stream.get(), false);
      };

      /*!
       * \brief Resume playing ambiance.
       */
      static void unpause(void) {
        if (al_get_mixer_attached(_mixer_4)) al_set_audio_stream_playing(ambiance_stream.get(), true);
      };

      /*!
       * \brief Adjust the ambiance mixer volume level.
       * \param l Volume level to set.
       */
      static void set_level(const float& l) {
        (l >= 0.0f && l <= 1.0f ?
          config::_volume::ambiance = l :
          config::_volume::ambiance = 0.0f);
        set_volume();
      };
    };
};

template <> bool manager<audio>::initialized = false;

}  //  end namespace wte::mgr

#endif
