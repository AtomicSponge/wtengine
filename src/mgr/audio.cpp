/*!
 * WTEngine | File:  audio.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/audio.hpp"

namespace wte::mgr {

template <> bool audio::manager<audio>::initialized = false;

commands audio::cmds;
ALLEGRO_VOICE* audio::voice = NULL;
ALLEGRO_MIXER* audio::mixer_main = NULL;
ALLEGRO_MIXER* audio::mixer_1 = NULL;
ALLEGRO_MIXER* audio::mixer_2 = NULL;
ALLEGRO_MIXER* audio::mixer_3 = NULL;
ALLEGRO_MIXER* audio::mixer_4 = NULL;
wte_asset<al_audio> audio::music_stream;
ALLEGRO_AUDIO_STREAM* audio::ambiance_stream = NULL;
ALLEGRO_AUDIO_STREAM* audio::voice_stream = NULL;
std::map<const std::string, ALLEGRO_SAMPLE_ID> audio::sample_instances;

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

    //  Map the audio commands.
    //  Mixer 1
    cmds.add("music_loop", 1, [](const msg_args& args) {
        audio::music::loop(args[0]);
    });
    cmds.add("music_play", 1, [](const msg_args& args) {
        //audio::music::play(args[0]);
    });
    cmds.add("music_stop", 0, [](const msg_args& args) {
        audio::music::stop();
    });
    cmds.add("music_pause", 0, [](const msg_args& args) {
        audio::music::pause();
    });
    cmds.add("music_unpause", 0, [](const msg_args& args) {
        audio::music::unpause();
    });
    //  Mixer 2
    cmds.add("sample_play", 2, [](const msg_args& args) {
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
        mgr::audio::sample::play(
            mgr::assets<al_sample>::get<al_sample>(args[0]),
            args[1], gain, pan, speed);
    });
    cmds.add("sample_stop", 1, [](const msg_args& args) {
        audio::sample::stop(args[0]);
    });
    cmds.add("sample_clear_instances", 0, [](const msg_args& args) {
        audio::sample::clear_instances();
    });
    //  Mixer 3
    cmds.add("voice_play", 1, [](const msg_args& args) {
        audio::voice::play(args[0]);
    });
    cmds.add("voice_stop", 0, [](const msg_args& args) {
        audio::voice::stop();
    });
    cmds.add("voice_pause", 0, [](const msg_args& args) {
        audio::voice::pause();
    });
    cmds.add("voice_unpause", 0, [](const msg_args& args) {
        audio::voice::unpause();
    });
    //  Mixer 4
    cmds.add("ambiance_loop", 1, [](const msg_args& args) {
        audio::ambiance::loop(args[0]);
    });
    cmds.add("ambiance_play", 1, [](const msg_args& args) {
        audio::ambiance::play(args[0]);
    });
    cmds.add("ambiance_stop", 0, [](const msg_args& args) {
        audio::ambiance::stop();
    });
    cmds.add("ambiance_pause", 0, [](const msg_args& args) {
        audio::ambiance::pause();
    });
    cmds.add("ambiance_unpause", 0, [](const msg_args& args) {
        audio::ambiance::unpause();
    });
    //  General
    cmds.add("set_volume_level", 2, [](const msg_args& args) {
        audio::set_level(std::stoi(args[0]), std::stof(args[1]));
    });
}

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

    // Check for and unload music stream.
    if(al_get_mixer_attached(mixer_1)) {
        al_drain_audio_stream(**music_stream);
        al_detach_audio_stream(**music_stream);
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
}

/*
 *
 */
void audio::set_level(
    const std::size_t& m,
    const float& l
) {
    switch(m) {
        case 0:
            if(l >= 0.0f && l <= 1.0f) config::_volume::main = l;
            else config::_volume::main = 0.0f;
            break;
        case 1:
            if(l >= 0.0f && l <= 1.0f) config::_volume::mix1 = l;
            else config::_volume::mix1 = 0.0f;
            break;
        case 2:
            if(l >= 0.0f && l <= 1.0f) config::_volume::mix2 = l;
            else config::_volume::mix2 = 0.0f;
            break;
        case 3:
            if(l >= 0.0f && l <= 1.0f) config::_volume::mix3 = l;
            else config::_volume::mix3 = 0.0f;
            break;
        case 4:
            if(l >= 0.0f && l <= 1.0f) config::_volume::mix4 = l;
            else config::_volume::mix4 = 0.0f;
            break;
    };
    set_volume();
}

/*
 *
 */
void audio::set_volume(void) {
    if(config::volume::main >= 0.0f && config::volume::main <= 1.0f)
        al_set_mixer_gain(mixer_main, config::volume::main);
    else
        al_set_mixer_gain(mixer_main, 0.5f);
    if(config::volume::mix1 >= 0.0f && config::volume::mix1 <= 1.0f)
        al_set_mixer_gain(mixer_1, config::volume::mix1);
    else
        al_set_mixer_gain(mixer_1, 0.5f);
    if(config::volume::mix2 >= 0.0f && config::volume::mix2 <= 1.0f)
        al_set_mixer_gain(mixer_2, config::volume::mix2);
    else
        al_set_mixer_gain(mixer_2, 0.5f);
    if(config::volume::mix3 >= 0.0f && config::volume::mix3 <= 1.0f)
        al_set_mixer_gain(mixer_3, config::volume::mix3);
    else
        al_set_mixer_gain(mixer_3, 0.5f);
    if(config::volume::mix4 >= 0.0f && config::volume::mix4 <= 1.0f)
        al_set_mixer_gain(mixer_4, config::volume::mix4);
    else
        al_set_mixer_gain(mixer_4, 0.5f);
}

/*
 *
 */
void audio::process_messages(const message_container& messages) { cmds.process_messages(messages); }

/*
 *
 */
void audio::music::loop(const std::string& arg) {
    if(!al_get_mixer_attached(mixer_1)) return;  //  Music not loaded, end.
    if(arg == "enable") al_set_audio_stream_playmode(**music_stream, ALLEGRO_PLAYMODE_LOOP);
    if(arg == "disable") al_set_audio_stream_playmode(**music_stream, ALLEGRO_PLAYMODE_ONCE);
}

/*
 *
 */
void audio::music::play(wte_asset<al_audio> audio) {
    if(al_get_mixer_attached(mixer_1)) {
        al_drain_audio_stream(**music_stream);
        al_detach_audio_stream(**music_stream);
    }
    music_stream = audio;
    al_attach_audio_stream_to_mixer(**music_stream, mixer_1);
    al_set_audio_stream_playmode(**music_stream, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_playing(**music_stream, true);
}

/*
 *
 */
void audio::music::stop(void) {
    if(al_get_mixer_attached(mixer_1)) al_set_audio_stream_playing(**music_stream, false);
}

/*
 *
 */
void audio::music::pause(void) {
    if(al_get_mixer_attached(mixer_1) && al_get_mixer_playing(mixer_1))
        al_set_audio_stream_playing(**music_stream, false);
}

/*
 *
 */
void audio::music::unpause(void) {
    if(al_get_mixer_attached(mixer_1)) al_set_audio_stream_playing(**music_stream, true);
}

/*
 *
 */
void audio::sample::play(
    wte_asset<al_sample> sample,
    const std::string& ref
) {
    sample::play(sample, ref, 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f);
}

/*
 *
 */
void audio::sample::play(
    wte_asset<al_sample> sample,
    const std::string& ref,
    const float& gain,
    const float& pan,
    const float& speed
) {
    if(ref == "once") {
        // Play the sample once.
        al_play_sample(**sample, gain, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else {
        //  If the reference is already playing, end.
        if(sample_instances.find(ref) != sample_instances.end()) return;
        //  Store playing reference
        ALLEGRO_SAMPLE_ID temp_sample_id;
        if(al_play_sample(**sample, gain, pan, speed, ALLEGRO_PLAYMODE_LOOP, &temp_sample_id))
            sample_instances.insert(std::make_pair(ref, temp_sample_id));
    }
}

/*
 *
 */
void audio::sample::stop(const std::string& ref) {
    //  If instance does not exist, end.
    if(sample_instances.find(ref) == sample_instances.end()) return;
    al_stop_sample(&sample_instances.find(ref)->second);
    sample_instances.erase(sample_instances.find(ref));
}

/*
 *
 */
void audio::sample::clear_instances(void) {
    for(auto sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
        al_stop_sample(&sample_instance->second);
        sample_instances.erase(sample_instance);
        sample_instance = sample_instances.begin();
    }
}

/*
 *
 */
void audio::voice::play(const std::string& fname) {
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
void audio::voice::stop(void) {
    if(al_get_mixer_attached(mixer_3)) al_set_audio_stream_playing(voice_stream, false);
}

/*
 *
 */
void audio::voice::pause(void) {
    if(al_get_mixer_attached(mixer_3) && al_get_mixer_playing(mixer_3))
        al_set_audio_stream_playing(voice_stream, false);
}

/*
 *
 */
void audio::voice::unpause(void) {
    if(al_get_mixer_attached(mixer_3)) al_set_audio_stream_playing(voice_stream, true);
}

/*
 *
 */
void audio::ambiance::loop(const std::string& arg) {
    if(!al_get_mixer_attached(mixer_4)) return;  //  Ambiance not loaded, end.
    if(arg == "enable")
        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
    if(arg == "disable")
        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_ONCE);
}

/*
 *
 */
void audio::ambiance::play(const std::string& fname) {
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
void audio::ambiance::stop(void) {
    if(al_get_mixer_attached(mixer_4)) al_set_audio_stream_playing(ambiance_stream, false);
}

/*
 *
 */
void audio::ambiance::pause(void) {
    if(al_get_mixer_attached(mixer_4) && al_get_mixer_playing(mixer_4))
        al_set_audio_stream_playing(ambiance_stream, false);
}

/*
 *
 */
void audio::ambiance::unpause(void) {
    if(al_get_mixer_attached(mixer_4)) al_set_audio_stream_playing(ambiance_stream, true);
}

}  //  end namespace wte::mgr
