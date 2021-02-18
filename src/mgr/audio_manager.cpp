/*!
 * WTEngine | File:  audio_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <map>
#include <deque>
#include <mutex>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_physfs.h>

#include "wtengine/mgr/audio_manager.hpp"

namespace wte
{

namespace mgr
{

audio_manager::audio_manager() {
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
    map_cmd_str_values["clear_instances"] = CMD_STR_CLEAR_INSTANCES;
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

    audio_messages.clear();
    sample_map.clear();
    sample_instances.clear();
};

audio_manager::~audio_manager() {
    map_cmd_str_values.clear();
    audio_messages.clear();
    sample_map.clear();
    sample_instances.clear();
};

void audio_manager::initialize(void) {
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

    //  Verify audio levels are registered in engine cfg.
    if(!engine_cfg::is_reg("main_vol")) engine_cfg::reg("main_vol=1.0");
    if(!engine_cfg::is_reg("mix1_vol")) engine_cfg::reg("mix1_vol=1.0");
    if(!engine_cfg::is_reg("mix2_vol")) engine_cfg::reg("mix2_vol=1.0");
    if(!engine_cfg::is_reg("mix3_vol")) engine_cfg::reg("mix3_vol=1.0");
    if(!engine_cfg::is_reg("mix4_vol")) engine_cfg::reg("mix4_vol=1.0");

    //  Set volume levels.
    set_volume();
};

void audio_manager::de_init(void) {
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

void audio_manager::get_volume(void) const {
    engine_cfg::set("main_vol", std::to_string(al_get_mixer_gain(mixer_main)));
    engine_cfg::set("mix1_vol", std::to_string(al_get_mixer_gain(mixer_1)));
    engine_cfg::set("mix2_vol", std::to_string(al_get_mixer_gain(mixer_2)));
    engine_cfg::set("mix3_vol", std::to_string(al_get_mixer_gain(mixer_3)));
    engine_cfg::set("mix4_vol", std::to_string(al_get_mixer_gain(mixer_4)));
};

void audio_manager::transfer_messages(const message_container& messages) {
    deque_mtx.lock();
    audio_messages.insert(audio_messages.end(),
                            std::make_move_iterator(messages.begin()),
                            std::make_move_iterator(messages.end()));
    deque_mtx.unlock();
};

const std::string audio_manager::get_sample_name(const std::string& full_path) {
    if(full_path.find("/") == std::string::npos)
        return full_path.substr(0, full_path.find("."));
    return full_path.substr(full_path.find_last_of("/") + 1,
        full_path.find(".") - (full_path.find_last_of("/") + 1));
};

void audio_manager::set_volume(void) {
    float vol = engine_cfg::get<float>("main_vol");
    if(vol >= 0.0f && vol <= 1.0f) al_set_mixer_gain(mixer_main, vol);
    vol = engine_cfg::get<float>("mix1_vol");
    if(vol >= 0.0f && vol <= 1.0f) al_set_mixer_gain(mixer_1, vol);
    vol = engine_cfg::get<float>("mix2_vol");
    if(vol >= 0.0f && vol <= 1.0f) al_set_mixer_gain(mixer_2, vol);
    vol = engine_cfg::get<float>("mix3_vol");
    if(vol >= 0.0f && vol <= 1.0f) al_set_mixer_gain(mixer_3, vol);
    vol = engine_cfg::get<float>("mix4_vol");
    if(vol >= 0.0f && vol <= 1.0f) al_set_mixer_gain(mixer_4, vol);
};

//  Used to restrict class to a single instance.
template <> bool audio_manager::manager<audio_manager>::initialized = false;

void audio_manager::run(void) {
    //  Set PhysFS interface for the thread.
    //  PhysFS is initialized in the wte_main constructor.
    al_set_physfs_file_interface();

    while(keep_running() == true) {
        //  Initialize variables for local use.
        float gain = 0.0f;
        float pan = 0.0f;
        float speed = 0.0f;
        bool test = false;

        //  Iterators for referencing saved & playing samples.
        std::map<std::string, ALLEGRO_SAMPLE*>::iterator sample_iterator;
        std::map<std::string, ALLEGRO_SAMPLE_ID>::iterator sample_instance;

        //  Temp sample ID for storing playing sample references.
        ALLEGRO_SAMPLE_ID temp_sample_id;

        if(!audio_messages.empty()) {
            //  Switch over the audio message and process.
            switch(map_cmd_str_values[audio_messages.front().get_cmd()]) {
                /* ********************************** */
                /* ***  Mixer 1 - Music controls  *** */
                /* ********************************** */
                /////////////////////////////////////////////
                //  cmd:  music_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_MUSIC_LOOP:
                    if(!al_get_mixer_attached(mixer_1)) break;  //  Music not loaded, end.
                    if(audio_messages.front().get_arg(0) == "enable")
                        al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_arg(0) == "disable")
                        al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  play_music - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_MUSIC:
                    //  Unload audio stream if one is already attached.
                    if(al_get_mixer_attached(mixer_1)) {
                        al_drain_audio_stream(music_stream);
                        al_detach_audio_stream(music_stream);
                        al_destroy_audio_stream(music_stream);
                    }
                    //  Load stream and play.
                    music_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!music_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(music_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(music_stream, mixer_1);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  stop_music - Stop current music from playing.
                case CMD_STR_STOP_MUSIC:
                    if(al_get_mixer_attached(mixer_1))
                        al_set_audio_stream_playing(music_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  pause_music - Pause music if it is playing.
                case CMD_STR_PAUSE_MUSIC:
                    if(al_get_mixer_attached(mixer_1) && al_get_mixer_playing(mixer_1))
                        al_set_audio_stream_playing(music_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  unpause_music - Unpause music if it is paused.
                case CMD_STR_UNPAUSE_MUSIC:
                    if(al_get_mixer_attached(mixer_1))
                        al_set_audio_stream_playing(music_stream, true);
                    break;
                /////////////////////////////////////////////

                /* *********************************** */
                /* ***  Mixer 2 - Sample controls  *** */
                /* *********************************** */
                /////////////////////////////////////////////
                //  cmd:  load_sample - args:  file - Load a sample.
                case CMD_STR_LOAD_SAMPLE:
                    //  Insert sample into reference map
                    if(al_load_sample(audio_messages.front().get_arg(0).c_str()) != NULL) {
                        sample_map.insert(
                            std::make_pair(get_sample_name(audio_messages.front().get_arg(0)),
                                           al_load_sample(audio_messages.front().get_arg(0).c_str()))
                        );
                    }
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  unload_sample - arg:  sample_name - Unload sample if one is loaded.
                //  If arg == "all" unload all samples.
                case CMD_STR_UNLOAD_SAMPLE:
                    //  Unload all samples.
                    if(audio_messages.front().get_arg(0) == "all") {
                        //  First clear out the sample instances.
                        for(sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
                            al_stop_sample(&sample_instance->second);
                            sample_instances.erase(sample_instance);
                            sample_instance = sample_instances.begin();
                        }
                        //  Then unload all samples.
                        for(sample_iterator = sample_map.begin(); sample_iterator != sample_map.end();) {
                            al_destroy_sample(sample_iterator->second);
                            sample_map.erase(sample_iterator);
                            sample_iterator = sample_map.begin();
                        }
                        break;
                    }
                    //  Find the sample in the map and unload it.
                    sample_iterator = sample_map.find(audio_messages.front().get_arg(0));
                    if(sample_iterator != sample_map.end()) {
                        al_destroy_sample(sample_iterator->second);
                        sample_map.erase(sample_iterator);
                    }
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  play_sample - args:  sample_name ; mode (once, loop_ref) ; gain ; pan ; speed
                //  Start playing loaded sample.
                //  If passed "once" no reference will be stored and the sample will be played once.
                //  Any other argument for the mode will be used for the reference storage.
                case CMD_STR_PLAY_SAMPLE:
                    //  If sample name not found in map, end.
                    if(sample_map.find(audio_messages.front().get_arg(0)) == sample_map.end()) break;
                    //  If no second argument, end.
                    if(audio_messages.front().get_arg(1) == "") break;
                    if(audio_messages.front().get_arg(2) != "") {
                        gain = audio_messages.front().get_arg<float>(2);
                        if(gain < 0.0f || gain > 1.0f) gain = 1.0f;
                    } else gain = 1.0f;
                    if(audio_messages.front().get_arg(3) != "") {
                        pan = audio_messages.front().get_arg<float>(3);
                        if(pan < -1.0f || pan > 1.0f) pan = ALLEGRO_AUDIO_PAN_NONE;
                    } else pan = ALLEGRO_AUDIO_PAN_NONE;
                    if(audio_messages.front().get_arg(4) != "") {
                        speed = audio_messages.front().get_arg<float>(4);
                        if(speed <= 0.0f || speed > 2.0f) speed = 1.0f;
                    } else speed = 1.0f;
                    if(audio_messages.front().get_arg(1) == "once") {
                        // Play the sample once.
                        al_play_sample((sample_map.find(audio_messages.front().get_arg(0)))->second,
                                       gain, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
                    } else {
                        //  If the reference is already playing, end.
                        if(sample_instances.find(audio_messages.front().get_arg(1)) != sample_instances.end()) break;
                        //  Store playing reference
                        test = al_play_sample((sample_map.find(audio_messages.front().get_arg(0)))->second,
                                              gain, pan, speed, ALLEGRO_PLAYMODE_LOOP, &temp_sample_id);
                        if(test)
                            sample_instances.insert(
                                std::make_pair(audio_messages.front().get_arg(1), temp_sample_id));
                    }
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  stop_sample - arg:  loop_ref - Stop looping sample instance.
                case CMD_STR_STOP_SAMPLE:
                        //  If instance does not exist, end.
                        if(sample_instances.find(audio_messages.front().get_arg(0)) == sample_instances.end()) break;
                        al_stop_sample(&sample_instances.find(audio_messages.front().get_arg(0))->second);
                        sample_instances.erase(sample_instances.find(audio_messages.front().get_arg(0)));
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  pan_sample - arg:  loop_ref ; pan ([left]-1.0 thru 1.0[right] or none) - Set sample pan.
                //  Note:  Disabled for now.  Needs testing.
                case CMD_STR_PAN_SAMPLE:
                    #ifdef ALLEGRO_UNSTABLE
                    if(sample_instances.find(audio_messages.front().get_arg(0)) == sample_instances.end()) break;
                    if(audio_messages.front().get_arg(1) == "none") {
                        pan = ALLEGRO_AUDIO_PAN_NONE;
                    } else {
                        pan = audio_messages.front().get_arg<float>(1);
                        if(pan < -1.0f || pan > 1.0f) pan = ALLEGRO_AUDIO_PAN_NONE;
                    }
                    al_set_sample_instance_pan(&sample_instances.find(audio_messages.front().get_arg(0))->second, pan);
                    #endif
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  clear_instances - Stop all looping sample instances.
                case CMD_STR_CLEAR_INSTANCES:
                    for(sample_instance = sample_instances.begin(); sample_instance != sample_instances.end();) {
                        al_stop_sample(&sample_instance->second);
                        sample_instances.erase(sample_instance);
                        sample_instance = sample_instances.begin();
                    }
                    break;
                /////////////////////////////////////////////

                /* ********************************** */
                /* ***  Mixer 3 - Voice controls  *** */
                /* ********************************** */
                /////////////////////////////////////////////
                //  cmd:  play_voice - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_VOICE:
                    //  Unload audio stream if one is already attached.
                    if(al_get_mixer_attached(mixer_3)) {
                        al_drain_audio_stream(voice_stream);
                        al_detach_audio_stream(voice_stream);
                        al_destroy_audio_stream(voice_stream);
                    }
                    //  Load stream and play.
                    voice_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!voice_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(voice_stream, ALLEGRO_PLAYMODE_ONCE);
                    al_attach_audio_stream_to_mixer(voice_stream, mixer_3);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  stop_voice - Stop current voice from playing.
                case CMD_STR_STOP_VOICE:
                    if(al_get_mixer_attached(mixer_3))
                        al_set_audio_stream_playing(voice_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  pause_voice - Pause voice if it is playing.
                case CMD_STR_PAUSE_VOICE:
                    if(al_get_mixer_attached(mixer_3) && al_get_mixer_playing(mixer_3))
                        al_set_audio_stream_playing(voice_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  unpause_voice - Unpause voice if it is paused.
                case CMD_STR_UNPAUSE_VOICE:
                    if(al_get_mixer_attached(mixer_3))
                        al_set_audio_stream_playing(voice_stream, true);
                    break;
                /////////////////////////////////////////////

                /* ************************************* */
                /* ***  Mixer 4 - Ambiance controls  *** */
                /* ************************************* */
                /////////////////////////////////////////////
                //  cmd:  ambiance_loop - arg:  enable/disable - Turn music looping on or off.
                case CMD_STR_AMBIANCE_LOOP:
                    if(!al_get_mixer_attached(mixer_4)) break;  //  Ambiance not loaded, end.
                    if(audio_messages.front().get_arg(0) == "enable")
                        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    if(audio_messages.front().get_arg(0) == "disable")
                        al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_ONCE);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  play_ambiance - arg:  file.name - Load a file and play in a stream.
                case CMD_STR_PLAY_AMBIANCE:
                    //  Unload audio stream if one is already attached.
                    if(al_get_mixer_attached(mixer_4)) {
                        al_drain_audio_stream(ambiance_stream);
                        al_detach_audio_stream(ambiance_stream);
                        al_destroy_audio_stream(ambiance_stream);
                    }
                    //  Load stream and play.
                    ambiance_stream = al_load_audio_stream(audio_messages.front().get_arg(0).c_str(), 4, 2048);
                    if(!ambiance_stream) break;  //  Didn't load audio, end.
                    al_set_audio_stream_playmode(ambiance_stream, ALLEGRO_PLAYMODE_LOOP);
                    al_attach_audio_stream_to_mixer(ambiance_stream, mixer_4);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  stop_ambiance - Stop current ambiance from playing.
                case CMD_STR_STOP_AMBIANCE:
                    if(al_get_mixer_attached(mixer_4))
                        al_set_audio_stream_playing(ambiance_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  pause_ambiance - Pause ambiance if it is playing.
                case CMD_STR_PAUSE_AMBIANCE:
                    if(al_get_mixer_attached(mixer_4) && al_get_mixer_playing(mixer_4))
                        al_set_audio_stream_playing(ambiance_stream, false);
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  unpause_ambiance - Unpause ambiance if it is paused.
                case CMD_STR_UNPAUSE_AMBIANCE:
                    if(al_get_mixer_attached(mixer_4))
                        al_set_audio_stream_playing(ambiance_stream, true);
                    break;
                /////////////////////////////////////////////

                /* ************************ */
                /* *** General commands *** */
                /* ************************ */
                /////////////////////////////////////////////
                //  cmd:  set_volume - Get audio levels from engine cfg and set.
                case CMD_STR_SET_VOLUME:
                    set_volume();
                    break;
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                //  cmd:  new_cmd - description.
                //case CMD_STR_X:
                    //
                    //break;
                /////////////////////////////////////////////
            }
            //  Remove processed message from the deck.
            deque_mtx.lock();
            audio_messages.pop_front();
            deque_mtx.unlock();
        }  //  End if(!audio_messages.empty())
    }  //  End while(is_running() == true)
}  //  End run member

} //  namespace mgr

} //  end namespace wte
