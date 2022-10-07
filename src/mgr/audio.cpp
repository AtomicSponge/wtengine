/*!
 * wtengine | File:  audio.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/audio.hpp"

namespace wte::mgr {

template <> bool manager<audio>::initialized = false;

commands audio::cmds;
ALLEGRO_VOICE* audio::voice = NULL;
ALLEGRO_MIXER* audio::_mixer_main = NULL;
ALLEGRO_MIXER* audio::_mixer_1 = NULL;
ALLEGRO_MIXER* audio::_mixer_1_a = NULL;
ALLEGRO_MIXER* audio::_mixer_1_b = NULL;
ALLEGRO_MIXER* audio::_mixer_2 = NULL;
ALLEGRO_MIXER* audio::_mixer_3 = NULL;
ALLEGRO_MIXER* audio::_mixer_4 = NULL;
wte_asset<al_audio> audio::music_stream_a;
wte_asset<al_audio> audio::music_stream_b;
wte_asset<al_audio> audio::ambiance_stream;
wte_asset<al_audio> audio::voice_stream;
std::map<const std::string, ALLEGRO_SAMPLE_ID> audio::sample_instances;

/*
 *
 */
void audio::initialize(void) {
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
        if(args[0] == "a") {
            if(args[1] == "disable") audio::music::a::loop(false);
            else audio::music::a::loop(true);
        }
        if(args[0] == "b") {
            if(args[1] == "disable") audio::music::b::loop(false);
            else audio::music::b::loop(true);
        }
    });
    cmds.add("music-play", 2, [](const msg_args& args) {
        if(args[0] == "a")
            audio::music::a::play(mgr::assets<al_audio>::get<al_audio>(args[1]));
        if(args[0] == "b")
            audio::music::b::play(mgr::assets<al_audio>::get<al_audio>(args[1]));
    });
    cmds.add("music-stop", 1, [](const msg_args& args) {
        if(args[0] == "a") audio::music::a::stop();
        if(args[0] == "b") audio::music::b::stop();
    });
    cmds.add("music-pause", 1, [](const msg_args& args) {
        if(args[0] == "a") audio::music::a::pause();
        if(args[0] == "b") audio::music::b::pause();
    });
    cmds.add("music-unpause", 1, [](const msg_args& args) {
        if(args[0] == "a") audio::music::a::unpause();
        if(args[0] == "b") audio::music::a::unpause();
    });
    //  Mixer 2
    cmds.add("sample-play", 2, [](const msg_args& args) {
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
        audio::sample::play(
            mgr::assets<al_sample>::get<al_sample>(args[0]),
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
        audio::voice::play(mgr::assets<al_audio>::get<al_audio>(args[0]));
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
        if(args[0] == "disable") audio::ambiance::loop(false);
        else audio::ambiance::loop(true);
    });
    cmds.add("ambiance-play", 1, [](const msg_args& args) {
        audio::ambiance::play(mgr::assets<al_audio>::get<al_audio>(args[0]));
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
}

/*
 *
 */
void audio::de_init(void) {
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
}

/*
 *
 */
void audio::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::main = l :
        config::_volume::main = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::music::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::music = l :
        config::_volume::music = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::music::a::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::music_a = l :
        config::_volume::music_a = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::music::b::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::music_b = l :
        config::_volume::music_b = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::sample::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::sample = l :
        config::_volume::sample = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::voice::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::voice = l :
        config::_volume::voice = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::ambiance::set_level(const float& l) {
    (l >= 0.0f && l <= 1.0f ?
        config::_volume::ambiance = l :
        config::_volume::ambiance = 0.0f);
    set_volume();
}

/*
 *
 */
void audio::set_volume(void) {
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
}

/*
 *
 */
void audio::process_messages(const message_container& messages) { cmds.process_messages(messages); }

/*
 *
 */
void audio::music::a::loop(const bool& loop) {
    if(!al_get_mixer_attached(_mixer_1)) return;  //  Music not loaded, end.
    (loop ? al_set_audio_stream_playmode(**music_stream_a, ALLEGRO_PLAYMODE_LOOP) :
        al_set_audio_stream_playmode(**music_stream_a, ALLEGRO_PLAYMODE_ONCE));
}

/*
 *
 */
void audio::music::a::play(wte_asset<al_audio> audio) {
    music::a::stop();
    music_stream_a = audio;
    al_attach_audio_stream_to_mixer(**music_stream_a, _mixer_1_a);
    al_set_audio_stream_playmode(**music_stream_a, ALLEGRO_PLAYMODE_LOOP);
    al_rewind_audio_stream(**music_stream_a);
    al_set_audio_stream_playing(**music_stream_a, true);
}

/*
 *
 */
void audio::music::a::stop(void) {
    if(al_get_mixer_attached(_mixer_1_a)) {
        al_set_audio_stream_playing(**music_stream_a, false);
        al_drain_audio_stream(**music_stream_a);
        al_detach_audio_stream(**music_stream_a);
    }
}

/*
 *
 */
void audio::music::a::pause(void) {
    if(al_get_mixer_attached(_mixer_1_a) && al_get_mixer_playing(_mixer_1_a))
        al_set_audio_stream_playing(**music_stream_a, false);
}

/*
 *
 */
void audio::music::a::unpause(void) {
    if(al_get_mixer_attached(_mixer_1)) al_set_audio_stream_playing(**music_stream_a, true);
}

/*
 *
 */
void audio::music::b::loop(const bool& loop) {
    if(!al_get_mixer_attached(_mixer_1_a)) return;  //  Music not loaded, end.
    (loop ? al_set_audio_stream_playmode(**music_stream_a, ALLEGRO_PLAYMODE_LOOP) :
        al_set_audio_stream_playmode(**music_stream_a, ALLEGRO_PLAYMODE_ONCE));
}

/*
 *
 */
void audio::music::b::play(wte_asset<al_audio> audio) {
    music::b::stop();
    music_stream_b = audio;
    al_attach_audio_stream_to_mixer(**music_stream_b, _mixer_1_b);
    al_set_audio_stream_playmode(**music_stream_b, ALLEGRO_PLAYMODE_LOOP);
    al_rewind_audio_stream(**music_stream_b);
    al_set_audio_stream_playing(**music_stream_b, true);
}

/*
 *
 */
void audio::music::b::stop(void) {
    if(al_get_mixer_attached(_mixer_1_b)) {
        al_set_audio_stream_playing(**music_stream_b, false);
        al_drain_audio_stream(**music_stream_b);
        al_detach_audio_stream(**music_stream_b);
    }
}

/*
 *
 */
void audio::music::b::pause(void) {
    if(al_get_mixer_attached(_mixer_1_b) && al_get_mixer_playing(_mixer_1_b))
        al_set_audio_stream_playing(**music_stream_b, false);
}

/*
 *
 */
void audio::music::b::unpause(void) {
    if(al_get_mixer_attached(_mixer_1_b)) al_set_audio_stream_playing(**music_stream_b, true);
}

/*
 *
 */
void audio::sample::play(
    wte_asset<al_sample> sample,
    const std::string& ref
) { play(sample, ref, 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f); }

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
void audio::voice::play(wte_asset<al_audio> audio) {
    voice::stop();
    voice_stream = audio;
    al_attach_audio_stream_to_mixer(**voice_stream, _mixer_3);
    al_set_audio_stream_playmode(**voice_stream, ALLEGRO_PLAYMODE_ONCE);
    al_rewind_audio_stream(**voice_stream);
    al_set_audio_stream_playing(**voice_stream, true);
}

/*
 *
 */
void audio::voice::stop(void) {
    if(al_get_mixer_attached(_mixer_3)) {
        al_set_audio_stream_playing(**voice_stream, false);
        al_drain_audio_stream(**voice_stream);
        al_detach_audio_stream(**voice_stream);
    }
}

/*
 *
 */
void audio::voice::pause(void) {
    if(al_get_mixer_attached(_mixer_3) && al_get_mixer_playing(_mixer_3))
        al_set_audio_stream_playing(**voice_stream, false);
}

/*
 *
 */
void audio::voice::unpause(void) {
    if(al_get_mixer_attached(_mixer_3)) al_set_audio_stream_playing(**voice_stream, true);
}

/*
 *
 */
void audio::ambiance::loop(const bool& loop) {
    if(!al_get_mixer_attached(_mixer_4)) return;  //  Ambiance not loaded, end.
    (loop ? al_set_audio_stream_playmode(**ambiance_stream, ALLEGRO_PLAYMODE_LOOP) :
        al_set_audio_stream_playmode(**ambiance_stream, ALLEGRO_PLAYMODE_ONCE));
}

/*
 *
 */
void audio::ambiance::play(wte_asset<al_audio> audio) {
    ambiance::stop();
    ambiance_stream = audio;
    al_attach_audio_stream_to_mixer(**ambiance_stream, _mixer_4);
    al_set_audio_stream_playmode(**ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
    al_rewind_audio_stream(**ambiance_stream);
    al_set_audio_stream_playing(**ambiance_stream, true);
}

/*
 *
 */
void audio::ambiance::stop(void) {
    if(al_get_mixer_attached(_mixer_4)) {
        al_set_audio_stream_playing(**ambiance_stream, false);
        al_drain_audio_stream(**ambiance_stream);
        al_detach_audio_stream(**ambiance_stream);
    }
}

/*
 *
 */
void audio::ambiance::pause(void) {
    if(al_get_mixer_attached(_mixer_4) && al_get_mixer_playing(_mixer_4))
        al_set_audio_stream_playing(**ambiance_stream, false);
}

/*
 *
 */
void audio::ambiance::unpause(void) {
    if(al_get_mixer_attached(_mixer_4)) al_set_audio_stream_playing(**ambiance_stream, true);
}

}  //  end namespace wte::mgr
