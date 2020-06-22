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
#define _USE_MATH_DEFINES

#include <cmath>

#include <allegro5/allegro_primitives.h>

#include "include/wte_demo.hpp"
#include "include/custom_input.hpp"
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
    game_cfg::reg("max_shield=100");
    game_cfg::reg("shield=50");

    game_cfg_map::load();
}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {
    game_cfg_map::clear_save();
    game_cfg_map::save("max_lives");
    game_cfg_map::save("hiscore");

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
        //menus.set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game", "game.sdf"));
        menus.set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game"));
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
        mnu::menu temp_menu = mnu::menu("audio_settings", "Audio Settings");
        //
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    wte_add_system(custom_input);
    wte_add_system(sys::logic);
    wte_add_system(sys::colision);
    wte_add_system(sys::animate);
}

/*
 * New game process.
 * Create starting entities and load entities into the spawner.
 */
void wte_demo::new_game(void) {
    entity e_id;

    /*
     * Background entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "starfield");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, stars);
    wte_new_component(e_id, cmp::background, mgr::render_manager::get_arena_width(),
                                             mgr::render_manager::get_arena_height(), 0,
        [](entity bkg_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define the animation process for the starfield.
            al_set_target_bitmap(wte_get_component(bkg_id, cmp::background)->background_bitmap);
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
    );  //  End background component.
    wte_new_component(e_id, cmp::dispatcher,
        [](entity bkg_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time, message msg) {
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
        }
    );  //  End dispatcher component.

    /*
     * Score overlay entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "score_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, 0, mgr::render_manager::get_arena_height() - 20, 0,
        [](entity ovr_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define what gets displayed on the overlay.
            al_set_target_bitmap(wte_get_component(ovr_id, cmp::overlay)->overlay_bitmap);
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            wte_set_component(ovr_id, cmp::overlay)->set_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->set_text(game_cfg::get("score"), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            wte_set_component(ovr_id, cmp::overlay)->set_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->set_text(game_cfg::get("hiscore"), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    );
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /*
     * Player Info overlay entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "player_info_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, mgr::render_manager::get_arena_width() - 200,
                                                   mgr::render_manager::get_arena_height() - 20, 0,
        [](entity ovr_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define what gets displayed on the overlay.
            al_set_target_bitmap(wte_get_component(ovr_id, cmp::overlay)->overlay_bitmap);
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            al_draw_filled_rectangle((float)(120 - game_cfg::get<int>("shield")), 0, 120, 10, WTE_COLOR_YELLOW);
            wte_set_component(ovr_id, cmp::overlay)->set_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->set_text("Lives:  " + game_cfg::get("lives"), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
        }
    );
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /*
     * Player entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "player");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, (mgr::render_manager::get_arena_width() / 2) - 5,
                                            mgr::render_manager::get_arena_height() - 40);
    wte_new_component(e_id, cmp::hitbox, 10, 10);
    wte_new_component(e_id, health, 1, 1);
    wte_new_component(e_id, cmp::input_handler);
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::enabled);
    wte_new_component(e_id, cmp::ai,
        [](entity plr_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t engine_time) {
            if(wte_get_component(plr_id, health)->hp <= 0) {
                //  Process player death.
                game_cfg_map::subtract<int>("lives", 1);

                if(game_cfg::get<int>("lives") == 0) {
                    //  Game over!
                    messages.add_message(message("system", "end_game", ""));
                }

                wte_set_component(plr_id, cmp::location)->pos_x = (mgr::render_manager::get_arena_width() / 2) - 5;
                wte_set_component(plr_id, cmp::location)->pos_y = mgr::render_manager::get_arena_height() - 40;
                wte_set_component(plr_id, health)->hp = wte_get_component(plr_id, health)->hp_max;
            }
        }
    );  //  End player logic
    wte_new_component(e_id, cmp::dispatcher,
        [](entity plr_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time, message msg) {
            if(msg.get_cmd() == "damage") {
                wte_set_component(plr_id, health)->hp -= msg.get_arg<int>(0);
            }
        }
    );
    wte_new_component(e_id, cmp::sprite, 32, 32, -11.0f, 0.0f, 1, 1);
    wte_set_component(e_id, cmp::sprite)->load_sprite("ship.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 3);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    /*
     * Shield entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "shield");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 64, 64, false);
    wte_new_component(e_id, damage, 100);
    wte_new_component(e_id, cmp::input_handler);
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, cmp::sprite, 64, 64, 0.0f, 0.0f, 6, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("shield.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 5);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");
    wte_new_component(e_id, cmp::dispatcher,
        [](entity shd_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time, message msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                             std::to_string(wte_get_component(shd_id, damage)->dmg)));
            }
        }
    );

    /*
     * Main cannon entity.
     */
    e_id = world.new_entity();
    wte_new_component(e_id, cmp::name, "main_cannon");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 10, 200, false);
    wte_new_component(e_id, damage, 5);
    wte_new_component(e_id, cmp::input_handler);
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, cmp::sprite, 10, 200, 0.0f, 0.0f, 2, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("cannon.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 3);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");
    wte_new_component(e_id, cmp::dispatcher,
        [](entity can_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time, message msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                             std::to_string(wte_get_component(can_id, damage)->dmg)));
            }
        }
    );

    /*
     * Add the asteroid entity to the spawner.
     */
    spawner.add_spawn("asteroid", 5,
        [](entity e_id, mgr::entity_manager& world, msg_arg_list args) {
            int size = std::stoi(args[5]);
            if(size < 1) size = 1;
            if(size > 8) size = 8;

            wte_new_component(e_id, cmp::name, "asteroid" + std::to_string(e_id));
            wte_new_component(e_id, cmp::team, 1);
            wte_new_component(e_id, cmp::location, std::stof(args[1]), std::stof(args[2]));
            wte_new_component(e_id, cmp::hitbox, (float)(size * 16), (float)(size * 16));
            wte_new_component(e_id, health, size * 10, size * 10);
            wte_new_component(e_id, damage, 10);
            wte_new_component(e_id, cmp::direction, std::stof(args[3]));
            wte_new_component(e_id, velocity, std::stof(args[4]));
            wte_new_component(e_id, cmp::visible);
            wte_new_component(e_id, cmp::enabled);
            wte_new_component(e_id, cmp::ai,
                [](entity ast_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t engine_time) {
                    //  AI for asteroids defined here.
                    //  Move them at their speed and angle.
                    wte_set_component(ast_id, cmp::location)->pos_x +=
                        wte_get_component(ast_id, velocity)->speed *
                        cos(wte_get_component(ast_id, cmp::direction)->angle * (M_PI / 180));

                    wte_set_component(ast_id, cmp::location)->pos_y +=
                        wte_get_component(ast_id, velocity)->speed *
                        sin(wte_get_component(ast_id, cmp::direction)->angle * (M_PI / 180));

                    //  Perform OOB check.
                    if(wte_get_component(ast_id, cmp::location)->pos_y > mgr::render_manager::get_arena_height() + 100) {
                        messages.add_message(message("spawner", "delete",
                                             wte_get_component(ast_id, cmp::name)->name_str));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(wte_get_component(ast_id, health)->hp <= 0) {
                        messages.add_message(message("spawner", "delete", wte_get_component(ast_id, cmp::name)->name_str));
                        game_cfg_map::add<int>("score", 10);
                    }
                }
            );  //  End asteroid AI
            wte_new_component(e_id, cmp::dispatcher,
                [](entity ast_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time, message msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        messages.add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                                     std::to_string(wte_get_component(ast_id, damage)->dmg)));
                    }

                    if(msg.get_cmd() == "damage") {
                        wte_set_component(ast_id, health)->hp -= msg.get_arg<int>(0);
                    }
                }
            );  //  End asteroid  message dispatching.
            wte_new_component(e_id, cmp::sprite, 16, 16, 0.0f, 0.0f, 10, 0);
            wte_set_component(e_id, cmp::sprite)->load_sprite("asteroid.bmp");
            wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 5);
            wte_set_component(e_id, cmp::sprite)->set_cycle("main");
            wte_set_component(e_id, cmp::sprite)->scale_factor_x = (float)size;
            wte_set_component(e_id, cmp::sprite)->scale_factor_y = (float)size;
        }
    );

    //  Reset score.
    game_cfg::set("score=0");

    //  Set number of lives.
    if(game_cfg::get<int>("max_lives") > 5 || game_cfg::get<int>("max_lives") < 3)
        game_cfg::set("max_lives=3");
    game_cfg::set("lives", game_cfg::get("max_lives"));
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
