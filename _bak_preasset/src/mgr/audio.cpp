/*!
 * WTEngine | File:  audio.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/audio.hpp"

//#include <iostream>

namespace wte
{

namespace mgr
{

template <> bool audio::manager<audio>::initialized = false;

commands audio::cmds;
ALLEGRO_VOICE* audio::voice = NULL;
ALLEGRO_MIXER* audio::mixer_main = NULL;
ALLEGRO_MIXER* audio::mixer_1 = NULL;
ALLEGRO_MIXER* audio::mixer_2 = NULL;
ALLEGRO_MIXER* audio::mixer_3 = NULL;
ALLEGRO_MIXER* audio::mixer_4 = NULL;
ALLEGRO_AUDIO_STREAM* audio::music_stream = NULL;
ALLEGRO_AUDIO_STREAM* audio::ambiance_stream = NULL;
ALLEGRO_AUDIO_STREAM* audio::voice_stream = NULL;
std::map<std::string, ALLEGRO_SAMPLE*> audio::sample_map = {};
std::map<std::string, ALLEGRO_SAMPLE_ID> audio::sample_instances = {};

/*
 *
 */
audio::audio() {
    sample_map.clear();
    sample_instances.clear();

    //  Map the audio commands.
    //  Mixer 1
    cmds.add("music_loop", 1, [this](const msg_args& args) {
        music_loop(args[0]);
    });
    cmds.add("play_music", 1, [this](const msg_args& args) {
        //std::cout << "command ran" << std::endl;
        music_play(args[0]);
    });
    cmds.add("stop_music", 0, [this](const msg_args& args) {
        music_stop();
    });
    cmds.add("pause_music", 0, [this](const msg_args& args) {
        music_pause();
    });
    cmds.add("unpause_music", 0, [this](const msg_args& args) {
        music_unpause();
    });
    //  Mixer 2
    cmds.add("load_sample", 2, [this](const msg_args& args) {
        sample_load(args[0], args[1]);
    });
    cmds.add("unload_sample", 1, [this](const msg_args& args) {
        sample_unload(args[0]);
    });
    cmds.add("play_sample", 2, [this](const msg_args& args) {
        float gain = 1.0f;
        float pan = ALLEGRO_AUDIO_PAN_NONE;
        float speed = 1.0f;

        if(args.size() >= 3) {
            gain = std::stof(args[2]);
            if(gain < 0.0f || gain > 1.0f) gain = 1.0f;
        }
        if(args.size() >= 4) {
            pan = std::stof(args[3]);
            if(pan < -1.0f || pan > 1.0f) pan = ALLEGRO_AUDIO_PAN_NONE;
        }
        if(args.size() >= 5) {
            speed = std::stof(args[4]);
            if(speed <= 0.0f || speed > 2.0f) speed = 1.0f;
        }

        sample_play(args[0], args[1], gain, pan, speed);
    });
    cmds.add("stop_sample", 1, [this](const msg_args& args) {
        sample_stop(args[0]);
    });
    cmds.add("clear_instances", 0, [this](const msg_args& args) {
        sample_clear_instances();
    });
    //  Mixer 3
    cmds.add("play_voice", 1, [this](const msg_args& args) {
        voice_play(args[0]);
    });
    cmds.add("stop_voice", 0, [this](const msg_args& args) {
        voice_stop();
    });
    cmds.add("pause_voice", 0, [this](const msg_args& args) {
        voice_pause();
    });
    cmds.add("unpause_voice", 0, [this](const msg_args& args) {
        voice_unpause();
    });
    //  Mixer 4
    cmds.add("ambiance_loop", 1, [this](const msg_args& args) {
        ambiance_loop(args[0]);
    });
    cmds.add("play_ambiance", 1, [this](const msg_args& args) {
        ambiance_play(args[0]);
    });
    cmds.add("stop_ambiance", 0, [this](const msg_args& args) {
        ambiance_stop();
    });
    cmds.add("pause_ambiance", 0, [this](const msg_args& args) {
        ambiance_pause();
    });
    cmds.add("unpause_ambiance", 0, [this](const msg_args& args) {
        ambiance_unpause();
    });
    //  General
    cmds.add("set_volume", 0, [this](const msg_args& args) {
        set_volume();
    });
};

/*
 *
 */
audio::~audio() {
    sample_map.clear();
    sample_instances.clear();
};

/*
 *
 */
void audio::initialize(void) {
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

    //  Set number of samples.
    al_set_default_mixer(mixer_2);
    al_reserve_samples(WTE_MAX_PLAYING_SAMPLES);

    //  Set volume levels.
    set_volume();
};

/*
 *
 */
void audio::de_init(void) {
    //  Clear any left over sample instances.
    for(auto sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
        al_stop_sample(&sample_instance->second);
        sample_instances.erase(sample_instance);
        sample_instance = sample_instances.begin();
    }

    //  Check for and destroy all samples loaded in the map.
    for(auto sample_iterator = sample_map.begin(); sample_iterator != sample_map.end();) {
        al_destroy_sample(sample_iterator->second);
        sample_map.erase(sample_iterator);
        sample_iterator = sample_map.begin();
    }

    // Check for and unload music stream.
    if(al_get_mixer_attached(mixer_1)) {
        al_drain_audio_stream(music_stream);
        al_detach_audio_stream(music_stream);
        al_destroy_audio_stream(music_stream);
    }

    // Check for and unload voice stream.
    if(al_get_mixer_attached(mixer_3)) {
        al_drain_audio_stream(voice_stream);
        al_detach_audio_stream(voice_stream);
        al_destroy_audio_stream(voice_stream);
    }

    // Check for and unload ambiance stream.
    if(al_get_mixer_attached(mixer_4)) {
        al_drain_audio_stream(ambiance_stream);
        al_detach_audio_stream(ambiance_stream);
        al_destroy_audio_stream(ambiance_stream);
    }

    //  Unload all mixers.
    al_destroy_mixer(mixer_1);
    al_destroy_mixer(mixer_2);
    al_destroy_mixer(mixer_3);
    al_destroy_mixer(mixer_4);
    al_destroy_mixer(mixer_main);

    //  Unload main audio output.
    al_destroy_voice(voice);
};

/*
 *
 */
void audio::set_volume(void) {
    if(config::volume::main >= 0.0f && config::volume::main <= 1.0f)
        al_set_mixer_gain(mixer_main, config::volume::main);

    if(config::volume::mix1 >= 0.0f && config::volume::mix1 <= 1.0f)
        al_set_mixer_gain(mixer_1, config::volume::mix1);

    if(config::volume::mix2 >= 0.0f && config::volume::mix2 <= 1.0f)
        al_set_mixer_gain(mixer_2, config::volume::mix2);

    if(config::volume::mix3 >= 0.0f && config::volume::mix3 <= 1.0f)
        al_set_mixer_gain(mixer_3, config::volume::mix3);

    if(config::volume::mix4 >= 0.0f && config::volume::mix4 <= 1.0f)
        al_set_mixer_gain(mixer_4, config::volume::mix4);
};

/*
 *
 */
void audio::process_messages(const message_container& messages) { cmds.process_messages(messages); }

/*
 *
 */
void audio::music_loop(const std::string& arg) {
    if(!al_get_mixer_attached(mixer_1)) return;  //  Music not loaded, end.
    if(arg == "enable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
    if(arg == "disable") al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_ONCE);
}

/*
 *
 */
void audio::music_play(const std::string& fname) {
    //  Unload audio stream if one is already attached.
    if(al_get_mixer_attached(mixer_1)) {
        al_drain_audio_stream(music_stream);
        al_detach_audio_stream(music_stream);
        al_destroy_audio_stream(music_stream);
    }
    //  Load stream and play.
    music_stream = al_load_audio_stream(fname.c_str(), 4, 2048);
    if(!music_stream) {
        //std::cout << "no music" << std::endl;
        return;  //  Didn't load audio, end.
    }
    //std::cout << "music should be playing" << std::endl;
    al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(music_stream, mixer_1);
}

/*
 *
 */
void audio::music_stop(void) {
    if(al_get_mixer_attached(mixer_1)) al_set_audio_stream_playing(music_stream, false);
}

/*
 *
 */
void audio::music_pause(void) {
    if(al_get_mixer_attached(mixer_1) && al_get_mixer_playing(mixer_1))
        al_set_audio_stream_playing(music_stream, false);
}

/*
 *
 */
void audio::music_unpause(void) {
    if(al_get_mixer_attached(mixer_1)) al_set_audio_stream_playing(music_stream, true);
}

/*
 *
 */
void audio::sample_load(const std::string& fname, const std::string& sname) {
    //  Insert sample into reference map
    if(al_load_sample(fname.c_str()) != NULL)
        sample_map.insert(std::make_pair(sname, al_load_sample(fname.c_str())));
}

/*
 *
 */
void audio::sample_unload(const std::string& sname) {
    //  Unload all samples.
    if(sname == "all") {
        //  First clear out the sample instances.
        for(auto sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
            al_stop_sample(&sample_instance->second);
            sample_instances.erase(sample_instance);
            sample_instance = sample_instances.begin();
        }
        //  Then unload all samples.
        for(auto sample_iterator = sample_map.begin(); sample_iterator != sample_map.end();) {
            al_destroy_sample(sample_iterator->second);
            sample_map.erase(sample_iterator);
            sample_iterator = sample_map.begin();
        }
        return;
    }
    //  Find the sample in the map and unload it.
    auto sample_iterator = sample_map.find(sname);
    if(sample_iterator != sample_map.end()) {
        al_destroy_sample(sample_iterator->second);
        sample_map.erase(sample_iterator);
    }
}

/*
 *
 */
void audio::sample_play(
    const std::string& sname,
    const std::string& ref
) {
    sample_play(sname, ref, 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f);
}

/*
 *
 */
void audio::sample_play(
    const std::string& sname,
    const std::string& ref,
    const float& gain,
    const float& pan,
    const float& speed
) {
    //  If sample name not found in map, end.
    if(sample_map.find(sname) == sample_map.end()) return;

    if(ref == "once") {
        // Play the sample once.
        al_play_sample((sample_map.find(sname))->second,
                        gain, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else {
        //  If the reference is already playing, end.
        if(sample_instances.find(ref) != sample_instances.end()) return;
        //  Store playing reference
        ALLEGRO_SAMPLE_ID temp_sample_id;
        if(al_play_sample((sample_map.find(sname))->second,
                           gain, pan, speed, ALLEGRO_PLAYMODE_LOOP, &temp_sample_id))
            sample_instances.insert(std::make_pair(ref, temp_sample_id));
    }
}

/*
 *
 */
void audio::sample_stop(const std::string& ref) {
    //  If instance does not exist, end.
    if(sample_instances.find(ref) == sample_instances.end()) return;
    al_stop_sample(&sample_instances.find(ref)->second);
    sample_instances.erase(sample_instances.find(ref));
}

/*
 *
 */
void audio::sample_clear_instances(void) {
    for(auto sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
        al_stop_sample(&sample_instance->second);
        sample_instances.erase(sample_instance);
        sample_instance = sample_instances.begin();
    }
}

/*
 *
 */
void audio::voice_play(const std::string& fname) {
    //  Unload audio stream if one is already attached.
    if(al_get_mixer_attached(mixer_3)) {
        al_drain_audio_stream(voice_stream);
        al_detach_audio_stream(voice_stream);
        al_destroy_audio_stream(voice_stream);
    }
    //  Load stream and play.
    voice_stream = al_load_audio_stream(fname.c_str(), 4, 2048);
    if(!voice_stream) return;  //  Didn't load audio, end.
    al_set_audio_stream_playmode(voice_stream, ALLEGRO_PLAYMODE_ONCE);
    al_attach_audio_stream_to_mixer(voice_stream, mixer_3);
}

/*
 *
 */
void audio::voice_stop(void) {
    if(al_get_mixer_attached(mixer_3)) al_set_audio_stream_playing(voice_stream, false);
}

/*
 *
 */
void audio::voice_pause(void) {
    if(al_get_mixer_attached(mixer_3) && al_get_mixer_playing(mixer_3))
        al_set_audio_stream_playing(voice_stream, false);
}

/*
 *
 */
void audio::voice_unpause(void) {
    if(al_get_mixer_attached(mixer_3)) al_set_audio_stream_playing(voice_stream, true);
}

/*
 *
 */
void audio::ambiance_loop(const std::string& arg) {
    if(!al_get_mixer_attached(mixer_4)) return;  //  Ambiance not loaded, end.
    if(arg == "enable")
        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
    if(arg == "disable")
        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_ONCE);
}

/*
 *
 */
void audio::ambiance_play(const std::string& fname) {
    //  Unload audio stream if one is already attached.
    if(al_get_mixer_attached(mixer_4)) {
        al_drain_audio_stream(ambiance_stream);
        al_detach_audio_stream(ambiance_stream);
        al_destroy_audio_stream(ambiance_stream);
    }
    //  Load stream and play.
    ambiance_stream = al_load_audio_stream(fname.c_str(), 4, 2048);
    if(!ambiance_stream) return;  //  Didn't load audio, end.
    al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(ambiance_stream, mixer_4);
}

/*
 *
 */
void audio::ambiance_stop(void) {
    if(al_get_mixer_attached(mixer_4)) al_set_audio_stream_playing(ambiance_stream, false);
}

/*
 *
 */
void audio::ambiance_pause(void) {
    if(al_get_mixer_attached(mixer_4) && al_get_mixer_playing(mixer_4))
        al_set_audio_stream_playing(ambiance_stream, false);
}

/*
 *
 */
void audio::ambiance_unpause(void) {
    if(al_get_mixer_attached(mixer_4)) al_set_audio_stream_playing(ambiance_stream, true);
}

} //  namespace mgr

} //  end namespace wte
