/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  wte_demo.cpp
 *
 * See LICENSE.md for copyright information
 *
 * WTE Demo game code.
 * This extends the wte_main object to define a custom game.
 */

#define _WTE_USE_HELPER_MACROS  //  Use engine helper macros.

#include <numeric>

#include <allegro5/allegro_primitives.h>

#include "include/wte_demo.hpp"
#include "include/my_components.hpp"

using namespace wte;  //  Required for macro use.

/*
 * Game's constructor.
 * Initialize game specific variables here.
 */
wte_demo::wte_demo(int argc, char **argv) : wte_main(argc, argv, "WTE Demo") {
    //  The engine doesn't use Allegro's primitives addon, so init it here.
    al_init_primitives_addon();

    game_cfg::reg("score=0");
    game_cfg::reg("hiscore=0");
    game_cfg::reg("max_lives=3");
    game_cfg::reg("lives=3");

    game_cfg::load();
}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {
    game_cfg::clear_save();
    game_cfg::save("max_lives");
    game_cfg::save("hiscore");

    al_shutdown_primitives_addon();
}

/*
 * Load menus.
 * Items will appear in the order they were added.
 */
void wte_demo::load_menus(void) {
    //  First set colors used by the menu.
    menus.set_menu_color(WTE_COLOR_WHITE, WTE_COLOR_DARKPURPLE);
    //  You can also set menu size here.  If not the default size is used.
    //menus.set_menu_size(200, 200, 10);

    //  Configure the root main menu and game menu.
    {
        //  Configure main menu.
        menus.set_menu("main_menu")->set_title("WTE Demo");
        menus.set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game", "game.sdf"));
        //menus.set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game"));
        menus.set_menu("main_menu")->add_item(wte_menu_action("Settings", "open_menu", "settings"));
        menus.set_menu("main_menu")->add_item(wte_menu_action("Exit Game", "exit"));
    }

    {
        //  Configure in-game menu.
        menus.set_menu("game_menu")->set_title("WTE Demo - Game Paused");
        menus.set_menu("game_menu")->add_item(wte_menu_action("Resume Game", "close_menu", "all"));
        menus.set_menu("game_menu")->add_item(wte_menu_action("Settings", "open_menu", "game_settings"));
        menus.set_menu("game_menu")->add_item(wte_menu_action("End Game", "end_game"));
        menus.set_menu("game_menu")->add_item(wte_menu_action("Exit Game", "exit"));
    }

    //  Then define other custom menus.
    {
        //  Create the main settings menu.
        mnu::menu temp_menu = mnu::menu("settings", "Settings");
        temp_menu.add_item(wte_menu_action("Demo Settings", "open_menu", "demo_settings"));
        temp_menu.add_item(wte_menu_action("Video Settings", "open_menu", "video_settings"));
        temp_menu.add_item(wte_menu_action("Audio Settings", "open_menu", "audio_settings"));
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the in-game settings menu.
        mnu::menu temp_menu = mnu::menu("game_settings", "Settings");
        temp_menu.add_item(wte_menu_action("Video Settings", "open_menu", "video_settings"));
        temp_menu.add_item(wte_menu_action("Audio Settings", "open_menu", "audio_settings"));
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the demo game settings menu.
        mnu::menu temp_menu = mnu::menu("demo_settings", "Demo Settings");
        std::vector<std::string> lives_vec = { "3", "4", "5" };
        temp_menu.add_item(wte_menu_selection("Lives:", "max_lives", lives_vec, lives_vec, mnu::GAME_SETTING));
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the video settings menu.
        mnu::menu temp_menu = mnu::menu("video_settings", "Video Settings");
        std::vector<std::string> mode_dvec = { "Windowed", "Windowed Full Screen" };
        std::vector<std::string> mode_vec = { "windowed", "windowed_full_screen" };
        temp_menu.add_item(wte_menu_selection("Display Mode:", "display_mode", mode_dvec, mode_vec, mnu::ENGINE_SETTING_RECONF));
        std::vector<std::string> scale_vec = { "0.5", "1", "1.25", "1.5", "1.75", "2" };
        temp_menu.add_item(wte_menu_selection("Scale factor:", "scale_factor", scale_vec, scale_vec, mnu::ENGINE_SETTING_RECONF));
        temp_menu.add_item(wte_menu_toggle("FPS:", "fps_counter", "on", "fps_counter", "off",
            [](void){ return engine_flags::is_set(DRAW_FPS); }));
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the audio settings menu.

        //  Create a vector filled with 0 through 100.
        std::vector<int> vol_values(101);
        std::iota(std::begin(vol_values), std::end(vol_values), 0);

        //  Create the volume display and levels vectors.
        std::vector<std::string> vol_dvec;
        std::vector<std::string> vol_vec;
        for(auto & vol_it : vol_values) {
            vol_dvec.push_back(std::to_string(vol_it));
            vol_vec.push_back(std::to_string((float)vol_it / 100.0f));
        }

        mnu::menu temp_menu = mnu::menu("audio_settings", "Audio Settings");
        temp_menu.add_item(wte_menu_selection("Main Volume:", "main_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING));
        temp_menu.add_item(wte_menu_selection("Music Volume:", "mix1_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING));
        temp_menu.add_item(wte_menu_selection("Effects Volume:", "mix2_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING));
        temp_menu.add_item(wte_menu_selection("Voice Volume:", "mix3_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING));
        temp_menu.add_item(wte_menu_selection("Ambiance Volume:", "mix4_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING));
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    wte_add_system(sys::input);
    wte_add_system(sys::movement);
    wte_add_system(sys::colision);
    wte_add_system(sys::logic);
    wte_add_system(sys::animate);
}

/*
 * New game process.
 * Create starting entities and load entities into the spawner.
 */
void wte_demo::new_game(void) {
    entity_id e_id;

    /* ********************************* */
    /* *** Background entity *********** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "starfield");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, stars);
    wte_new_component(e_id, cmp::background, mgr::render_manager::get_arena_width(),
                                             mgr::render_manager::get_arena_height(), 0,
        [](const entity_id& bkg_id, mgr::entity_manager& world, const int64_t& engine_time) {
            //  Define the animation process for the starfield.
            wte_set_component(bkg_id, cmp::background)->set_drawing();
            al_clear_to_color(WTE_COLOR_BLACK);

            //  Move the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                wte_set_component(bkg_id, stars)->y[i] +=
                    wte_get_component(bkg_id, stars)->speed[i] * wte_get_component(bkg_id, stars)->speed_mult;
                if(wte_get_component(bkg_id, stars)->y[i] > mgr::render_manager::get_arena_height()) {
                    //  Make a new star.
                    wte_set_component(bkg_id, stars)->x[i] =
                        std::rand() % mgr::render_manager::get_arena_width() + 1;
                    wte_set_component(bkg_id, stars)->y[i] = 0;
                    wte_set_component(bkg_id, stars)->speed[i] = (std::rand() % 3 + 1) * 3;
                    wte_set_component(bkg_id, stars)->color[i] = std::rand() % 4 + 1;
                }
            }

            //  Draw the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                if(wte_get_component(bkg_id, stars)->color[i] == 1 ||
                   wte_get_component(bkg_id, stars)->color[i] == 4)
                    al_draw_pixel(wte_get_component(bkg_id, stars)->x[i],
                                  wte_get_component(bkg_id, stars)->y[i], WTE_COLOR_WHITE);
                if(wte_get_component(bkg_id, stars)->color[i] == 2)
                    al_draw_pixel(wte_get_component(bkg_id, stars)->x[i],
                                  wte_get_component(bkg_id, stars)->y[i], WTE_COLOR_YELLOW);
                if(wte_get_component(bkg_id, stars)->color[i] == 3)
                    al_draw_pixel(wte_get_component(bkg_id, stars)->x[i],
                                  wte_get_component(bkg_id, stars)->y[i], WTE_COLOR_RED);
            }
        }
    );  //  End background rendering.

    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& bkg_id, const message& msg, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& current_time) {
            //  Define message processing for the starfield.
            if(msg.get_cmd() == "default") wte_set_component(bkg_id, stars)->speed_mult = 1;
            if(msg.get_cmd() == "up") wte_set_component(bkg_id, stars)->speed_mult *= 2;
            if(msg.get_cmd() == "down") wte_set_component(bkg_id, stars)->speed_mult /= 2;
            if(msg.get_cmd() == "reset") {
                wte_set_component(bkg_id, stars)->speed_mult = 1;

                for(std::size_t i = 0; i < MAX_STARS; i++) {
                    wte_set_component(bkg_id, stars)->x[i] =
                        std::rand() % mgr::render_manager::get_arena_width() + 1;
                    wte_set_component(bkg_id, stars)->y[i] =
                        std::rand() % mgr::render_manager::get_arena_height() + 1;
                    wte_set_component(bkg_id, stars)->speed[i] = (std::rand() % 3 + 1) * 3;
                    wte_set_component(bkg_id, stars)->color[i] = std::rand() % 4 + 1;
                }
            }
        }  //  End starfield message processing.
    );  //  End dispatcher component.

    /* ********************************* */
    /* *** Score overlay entity ******** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "score_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, 0, mgr::render_manager::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id, mgr::entity_manager& world, const int64_t& engine_time) {
            //  Define what gets displayed on the overlay.
            wte_set_component(ovr_id, cmp::overlay)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text(game_cfg::get("score"), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text(game_cfg::get("hiscore"), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    );  //  End score overlay drawing.
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /* ********************************* */
    /* *** Player Info overlay entity ** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "player_info_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, mgr::render_manager::get_arena_width() - 200,
                                                   mgr::render_manager::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id, mgr::entity_manager& world, const int64_t& engine_time) {
            //  Define what gets displayed on the overlay.
            entity_id shd_id = world.get_id("shield");
            wte_set_component(ovr_id, cmp::overlay)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            al_draw_filled_rectangle((float)(120 - wte_get_component(shd_id, energy)->amt), 0, 120, 10, WTE_COLOR_YELLOW);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Lives:  " + game_cfg::get("lives"), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
        }
    );  //  End info overlay drawing.
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /* ********************************* */
    /* *** Game Over overlay entity **** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "game_over_overlay");
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::overlay, 480, 132, (mgr::render_manager::get_arena_width() / 2) - 240,
                                                    (mgr::render_manager::get_arena_height() / 2) - 66, 1,
        [](const entity_id& ovr_id, mgr::entity_manager& world, const int64_t& engine_time) {
            //  Define what gets displayed on the overlay.
            wte_set_component(ovr_id, cmp::overlay)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            wte_set_component(ovr_id, cmp::overlay)->draw_bitmap("game_over", 0.0f, 0.0f, 0);
        }
    );  //  End game over overlay drawing.
    wte_set_component(e_id, cmp::overlay)->load_bitmap("game_over", "game_over.bmp");

    /* ********************************* */
    /* *** Player entity *************** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "player");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, (mgr::render_manager::get_arena_width() / 2) - 5,
                                            mgr::render_manager::get_arena_height() - 40);
    wte_new_component(e_id, cmp::hitbox, 10, 10);
    wte_new_component(e_id, cmp::bounding_box, 12.0f, 0.0f,
                                               (float)(mgr::render_manager::get_arena_width() - 21),
                                               (float)(mgr::render_manager::get_arena_height() - 32));
    wte_new_component(e_id, health, 1, 1);
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::enabled);
    wte_new_component(e_id, cmp::direction, false);
    wte_new_component(e_id, cmp::velocity);

    wte_new_component(e_id, cmp::sprite, 32.0f, 32.0f, -11.0f, 0.0f, 1, 1);
    wte_set_component(e_id, cmp::sprite)->load_sprite("ship.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 3);
    wte_set_component(e_id, cmp::sprite)->add_cycle("death", 4, 7);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    //  Player input handling.
    wte_new_component(e_id, cmp::input_directional, WTE_JOYSTICK_A,
        [](const entity_id& plr_id, const float& rad, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            wte_set_component(plr_id, cmp::direction)->set_radians(rad);
            wte_set_component(plr_id, cmp::velocity)->set_velocity(5.0f);
        },
        [](const entity_id& plr_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            wte_set_component(plr_id, cmp::velocity)->set_velocity(0.0f);
        }
    );  //  End player directional handling.

    //  Player logic.
    wte_new_component(e_id, cmp::ai,
        [](const entity_id& plr_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            if(wte_get_component(plr_id, health)->hp <= 0) {  //  Check player health.
                wte_set_component(plr_id, cmp::enabled)->disable();
                wte_set_component(plr_id, health)->hp = wte_get_component(plr_id, health)->hp_max;
                std::string player_name = world.get_name(plr_id);
                messages.add_message(message("entities", player_name, player_name, "death", ""));
            }
        }
    );  //  End player logic.

    //  Player message handling.
    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& plr_id, const message& msg, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& current_time) {
            //  Process player death.
            if(msg.get_cmd() == "death") {
                //  Make sure cannon stops firing
                entity_id cannon_id = world.get_id("main_cannon");
                wte_set_component(cannon_id, cmp::visible)->hide();
                wte_set_component(cannon_id, cmp::enabled)->disable();
                wte_set_component(cannon_id, cmp::sample_loop)->stop(messages, "cannon_fire");

                //  Just to make sure... turn shield off
                entity_id shield_id = world.get_id("shield");
                wte_set_component(shield_id, cmp::visible)->hide();
                wte_set_component(shield_id, cmp::enabled)->disable();
                wte_set_component(plr_id, cmp::hitbox)->make_solid();
                wte_set_component(shield_id, cmp::sample_loop)->stop(messages, "shield_sound");

                messages.add_message(message("audio", "play_sample", "megumin;once"));
                game_cfg::subtract<int>("lives", 1);
                wte_set_component(plr_id, cmp::velocity)->set_velocity(0.0f);
                wte_set_component(plr_id, cmp::sprite)->set_cycle("death");
                messages.add_message(message("system", "disable_system", "input"));
                if(game_cfg::get<int>("lives") == 0) {
                    //  Game over!
                    messages.add_message(message(current_time + 180, "system", "end_game", ""));
                    entity_id go_id = world.get_id("game_over_overlay");
                    wte_set_component(go_id, cmp::visible)->show();
                } else {
                    std::string player_name = world.get_name(plr_id);
                    messages.add_message(
                        message(current_time + 120, "entities", player_name, player_name, "reset", "")
                    );
                }
            }

            //  Reset player.
            if(msg.get_cmd() == "reset") {
                messages.add_message(message("system", "enable_system", "input"));
                wte_set_component(plr_id, cmp::location)->set_x((float)((mgr::render_manager::get_arena_width() / 2) - 5));
                wte_set_component(plr_id, cmp::location)->set_y((float)(mgr::render_manager::get_arena_height() - 40));
                wte_set_component(plr_id, health)->hp = wte_get_component(plr_id, health)->hp_max;
                wte_set_component(plr_id, cmp::enabled)->enable();
                wte_set_component(plr_id, cmp::sprite)->set_cycle("main");
            }

            //  Take damage.
            if(msg.get_cmd() == "damage") {
                wte_set_component(plr_id, health)->hp -= msg.get_arg<int>(0);
            }
        }
    );  //  End player message processing.

    /* ********************************* */
    /* *** Main cannon entity ********** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "main_cannon");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 10, 200, false);
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, damage, 3);
    wte_new_component(e_id, cmp::sample_loop);
    wte_set_component(e_id, cmp::sample_loop)->add_handle("laser", "cannon_fire");

    wte_new_component(e_id, cmp::sprite, 10.0f, 200.0f, 0.0f, 0.0f, 2, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("cannon.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 3);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    //  Cannon input handling.
    wte_new_component(e_id, cmp::input_button, WTE_INPUT_BUTTON_1,
        [](const entity_id& can_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            entity_id player_entity = world.get_id("player");
            //  Set the cannon's location to match the player.
            wte_set_component(can_id, cmp::location)->set_x(
                wte_get_component(player_entity, cmp::location)->get_x()
            );
            wte_set_component(can_id, cmp::location)->set_y(
                wte_get_component(player_entity, cmp::location)->get_y() -
                wte_get_component(can_id, cmp::hitbox)->get_height()
            );

            //  Turn the cannon on.
            wte_set_component(can_id, cmp::visible)->show();
            wte_set_component(can_id, cmp::enabled)->enable();

            //  Play sound effect.
            wte_set_component(can_id, cmp::sample_loop)->start(messages, "cannon_fire");
        },
        [](const entity_id& can_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            //  Turn the cannon off.
            wte_set_component(can_id, cmp::visible)->hide();
            wte_set_component(can_id, cmp::enabled)->disable();

            //  Stop sound effect.
            wte_set_component(can_id, cmp::sample_loop)->stop(messages, "cannon_fire");
        }
    );  //  End cannon input handler.

    //  Cannon logic.
    wte_new_component(e_id, cmp::ai,
        [](const entity_id& can_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            entity_id player_entity = world.get_id("player");
            //  Set the cannon's location to match the player.
            wte_set_component(can_id, cmp::location)->set_x(
                wte_get_component(player_entity, cmp::location)->get_x()
            );
            wte_set_component(can_id, cmp::location)->set_y(
                wte_get_component(player_entity, cmp::location)->get_y() -
                wte_get_component(can_id, cmp::hitbox)->get_height()
            );
        }
    );  //  End cannon logic.

    //  Cannon message processing.
    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& can_id, const message& msg, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& current_time) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                             std::to_string(wte_get_component(can_id, damage)->dmg)));
                //  Play the hit effect.
                //messages.add_message(message("audio", "play_sample", "fx3;once"));
            }
        }
    );  //  End cannon message processing.

    /* ********************************* */
    /* *** Shield entity *************** */
    /* ********************************* */
    e_id = world.new_entity();
    world.set_name(e_id, "shield");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 64, 64, false);
    wte_set_component(e_id, cmp::hitbox)->make_fluid();
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, energy, 50, 100);
    wte_new_component(e_id, damage, 100);
    wte_new_component(e_id, cmp::sample_loop);
    wte_set_component(e_id, cmp::sample_loop)->add_handle("shield", "shield_sound");

    wte_new_component(e_id, cmp::sprite, 64.0f, 64.0f, 0.0f, 0.0f, 6, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("shield.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 5);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    //  Shield input handling.
    wte_new_component(e_id, cmp::input_button, WTE_INPUT_BUTTON_2,
        [](const entity_id& shd_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            entity_id player_entity = world.get_id("player");
            //  Set the shield's location to match the player
            wte_set_component(shd_id, cmp::location)->set_x(
                wte_get_component(player_entity, cmp::location)->get_x() - 28.0f
            );
            wte_set_component(shd_id, cmp::location)->set_y(
                wte_get_component(player_entity, cmp::location)->get_y() - 16.0f
            );

            if(wte_set_component(shd_id, energy)->amt > 0) {
                //  Enable the shield.
                wte_set_component(shd_id, cmp::visible)->show();
                wte_set_component(shd_id, cmp::enabled)->enable();
                wte_set_component(player_entity, cmp::hitbox)->make_fluid();

                //  Play sound effect.
                wte_set_component(shd_id, cmp::sample_loop)->start(messages, "shield_sound");
            }
        },
        [](const entity_id& shd_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            //  Disable shield.
            wte_set_component(shd_id, cmp::visible)->hide();
            wte_set_component(shd_id, cmp::enabled)->disable();
            entity_id player_entity = world.get_id("player");
            wte_set_component(player_entity, cmp::hitbox)->make_solid();
            //  Stop sound effect.
            wte_set_component(shd_id, cmp::sample_loop)->stop(messages, "shield_sound");
        }
    );  //  End shield input handler.

    //  Shield logic.
    wte_new_component(e_id, cmp::ai,
        //  Enabeled AI.
        [](const entity_id& shd_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            entity_id player_entity = world.get_id("player");
            //  Set the shield's location to match the player.
            wte_set_component(shd_id, cmp::location)->set_x(
                wte_get_component(player_entity, cmp::location)->get_x() - 28.0f
            );
            wte_set_component(shd_id, cmp::location)->set_y(
                wte_get_component(player_entity, cmp::location)->get_y() - 16.0f
            );

            //  Drain the shield.
            if(wte_set_component(shd_id, energy)->amt > 0)
                wte_set_component(shd_id, energy)->amt -= 1;

            if(wte_get_component(shd_id, energy)->amt <= 0) {
                //  Disable shield.
                wte_set_component(shd_id, cmp::visible)->hide();
                wte_set_component(shd_id, cmp::enabled)->disable();
                wte_set_component(player_entity, cmp::hitbox)->make_solid();
                //  Stop sound effect.
                wte_set_component(shd_id, cmp::sample_loop)->stop(messages, "shield_sound");
            }
        },
        //  Disabeled AI.
        [](const entity_id& shd_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
            //  Recharge the shield.
            if(wte_set_component(shd_id, energy)->amt < wte_set_component(shd_id, energy)->amt_max)
                wte_set_component(shd_id, energy)->amt += 1;
        }
    );  //  End shield logic.

    //  Shield message processing.
    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& shd_id, const message& msg, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& current_time) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                             std::to_string(wte_get_component(shd_id, damage)->dmg)));
            }
        }
    );  //  End shield message processing.

    /* ************************************** */
    /* Add the asteroid entity to the spawner */
    /* Arguments:                             */
    /*  (1) X location                        */
    /*  (2) Y location                        */
    /*  (3) Direction in degrees              */
    /*  (4) Velocity                          */
    /*  (5) Size                              */
    /* ************************************** */
    spawner.add_spawn("asteroid", 5,
        [](const entity_id& e_id, mgr::entity_manager& world, const msg_arg_list& args) {
            int temp_size = std::stoi(args[5]);
            if(temp_size < 1) temp_size = 1;
            if(temp_size > 8) temp_size = 8;

            world.set_name(e_id, "asteroid" + std::to_string(e_id));
            wte_new_component(e_id, cmp::team, 1);
            wte_new_component(e_id, cmp::location, std::stof(args[1]), std::stof(args[2]));
            wte_new_component(e_id, cmp::hitbox, (float)(temp_size * 16), (float)(temp_size * 16));
            wte_new_component(e_id, health, temp_size * 10, temp_size * 10);
            wte_new_component(e_id, damage, 10);
            wte_new_component(e_id, size, temp_size);
            wte_new_component(e_id, cmp::direction);
            wte_set_component(e_id, cmp::direction)->set_degrees(std::stof(args[3]));
            wte_new_component(e_id, cmp::velocity, std::stof(args[4]));
            wte_new_component(e_id, cmp::visible);
            wte_new_component(e_id, cmp::enabled);

            wte_new_component(e_id, cmp::sprite, 16.0f, 16.0f, 0.0f, 0.0f, (int)(30 / std::stof(args[4])), 0);
            wte_set_component(e_id, cmp::sprite)->load_sprite("asteroid.bmp");
            wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 5);
            wte_set_component(e_id, cmp::sprite)->set_cycle("main");
            wte_set_component(e_id, cmp::sprite)->set_scale_factor_x((float)temp_size);
            wte_set_component(e_id, cmp::sprite)->set_scale_factor_y((float)temp_size);

            //  Asteroid logic.
            wte_new_component(e_id, cmp::ai,
                [](const entity_id& ast_id, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& engine_time) {
                    //  AI for asteroids defined here.
                    //  Perform OOB check.
                    if(wte_get_component(ast_id, cmp::location)->get_y() > (float)(mgr::render_manager::get_arena_height() + 100)) {
                        messages.add_message(message("spawner", "delete", world.get_name(ast_id)));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(wte_get_component(ast_id, health)->hp <= 0) {
                        messages.add_message(message("spawner", "delete", world.get_name(ast_id)));
                        messages.add_message(message("audio", "play_sample", "megumin;once;;;1.8"));
                        game_cfg::add<int>("score", (10 * wte_get_component(ast_id, size)->the_size));

                        //  If the asteroid was size >= 4, split into two.
                        if(wte_get_component(ast_id, size)->the_size >= 4) {
                            const int new_size = wte_get_component(ast_id, size)->the_size / 2;
                            float dir_a = wte_get_component(ast_id, cmp::direction)->get_degrees() - 90.0f;
                            if(dir_a < 0.0f) dir_a = 0.0f;
                            float dir_b = wte_get_component(ast_id, cmp::direction)->get_degrees() + 90.0f;
                            if(dir_b > 360.0f) dir_b = 360.0f;
                            const float new_x = wte_get_component(ast_id, cmp::location)->get_x();
                            const float new_y = wte_get_component(ast_id, cmp::location)->get_y();
                            const float new_vel = wte_get_component(ast_id, cmp::velocity)->get_x_vel() / 2;
                            std::string new_spawner_a = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_a) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            std::string new_spawner_b = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_b) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            messages.add_message(message("spawner", "new", new_spawner_a));
                            messages.add_message(message("spawner", "new", new_spawner_b));
                        }
                    }
                }
            );  //  End asteroid AI

            //  Asteroid message processing.
            wte_new_component(e_id, cmp::dispatcher,
                [](const entity_id& ast_id, const message& msg, mgr::entity_manager& world, mgr::message_manager& messages, const int64_t& current_time) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                                     std::to_string(wte_get_component(ast_id, damage)->dmg)));
                    }

                    if(msg.get_cmd() == "damage") {
                        wte_set_component(ast_id, health)->hp -= msg.get_arg<int>(0);
                    }
                }
            );  //  End asteroid message dispatching.
        }  //  End asteroid spawner function.
    );

    //  Reset score.
    game_cfg::set("score=0");

    //  Set number of lives.
    if(game_cfg::get<int>("max_lives") > 5 || game_cfg::get<int>("max_lives") < 3)
        game_cfg::set("max_lives=3");
    game_cfg::set("lives", game_cfg::get("max_lives"));

    //  Load some samples in the audio manager.
    wte_load_sample("sfx/laser.wav");
    wte_load_sample("sfx/shield.wav");
    wte_load_sample("sfx/megumin.wav");
}

/*
 * Stop the game.
 */
void wte_demo::end_game(void) {
    if(game_cfg::get<int>("score") > game_cfg::get<int>("hiscore"))
        game_cfg::set("hiscore", game_cfg::get("score"));
}

/*
 * On menu open.
 */
void wte_demo::on_menu_open(void) {
    messages.add_message(message("audio", "pause_music", ""));
    messages.add_message(message("audio", "pause_ambiance", ""));
}

/*
 * On menu close.
 */
void wte_demo::on_menu_close(void) {
    messages.add_message(message("audio", "unpause_music", ""));
    messages.add_message(message("audio", "unpause_ambiance", ""));
}
