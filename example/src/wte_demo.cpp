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
wte_demo::wte_demo(int argc, char **argv) : engine(argc, argv, "WTE Demo") {
    //  The engine doesn't use Allegro's primitives addon, so init it here.
    al_init_primitives_addon();

    mgr::variables::reg("score", (int)0);
    mgr::variables::reg("hiscore", (int)0);
    mgr::variables::reg("max_lives", (int)3);
    mgr::variables::reg("lives", (int)3);

    //mgr::variables::load();

    /*
     * Set up input handling
     */
    input::event::p1::ondown::up = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_y = -1.0f;
        float rad = std::atan2(config::controls::p1_pola_y, config::controls::p1_pola_x);
        wte_set_component(player_id, cmp::direction)->set_radians(rad);
        wte_set_component(player_id, cmp::velocity)->set_velocity(5.0f);
    };
    input::event::p1::ondown::down = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_y = 1.0f;
        float rad = std::atan2(config::controls::p1_pola_y, config::controls::p1_pola_x);
        wte_set_component(player_id, cmp::direction)->set_radians(rad);
        wte_set_component(player_id, cmp::velocity)->set_velocity(5.0f);
    };
    input::event::p1::ondown::left = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_x = -1.0f;
        float rad = std::atan2(config::controls::p1_pola_y, config::controls::p1_pola_x);
        wte_set_component(player_id, cmp::direction)->set_radians(rad);
        wte_set_component(player_id, cmp::velocity)->set_velocity(5.0f);
    };
    input::event::p1::ondown::right = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_x = 1.0f;
        float rad = std::atan2(config::controls::p1_pola_y, config::controls::p1_pola_x);
        wte_set_component(player_id, cmp::direction)->set_radians(rad);
        wte_set_component(player_id, cmp::velocity)->set_velocity(5.0f);
    };
    input::event::p1::ondown::action1 = []() {
        entity_id player_id = mgr::entities::get_id("player");
        entity_id can_id = mgr::entities::get_id("main_cannon");
        //  Set the cannon's location to match the player.
        wte_set_component(can_id, cmp::location)->set_x(
            wte_get_component(player_id, cmp::location)->get_x()
        );
        wte_set_component(can_id, cmp::location)->set_y(
            wte_get_component(player_id, cmp::location)->get_y() -
            wte_get_component(can_id, cmp::hitbox)->get_height()
        );

        //  Turn the cannon on.
        wte_set_component(can_id, cmp::visible)->show();
        wte_set_component(can_id, cmp::enabled)->enable();
        //  Play sound effect.
        mgr::audio::sample_play("laser", "cannon_fire");
    };
    input::event::p1::ondown::action2 = []() {
        entity_id player_id = mgr::entities::get_id("player");
        entity_id shd_id = mgr::entities::get_id("shield");
        //  Set the shield's location to match the player
        wte_set_component(shd_id, cmp::location)->set_x(
            wte_get_component(player_id, cmp::location)->get_x() - 28.0f
        );
        wte_set_component(shd_id, cmp::location)->set_y(
            wte_get_component(player_id, cmp::location)->get_y() - 16.0f
        );

        if(wte_set_component(shd_id, energy)->amt > 0) {
            //  Enable the shield.
            wte_set_component(shd_id, cmp::visible)->show();
            wte_set_component(shd_id, cmp::enabled)->enable();
            wte_set_component(player_id, cmp::hitbox)->make_fluid();
            //  Play sound effect.
            mgr::audio::sample_play("shield", "shield_sound");
        }
    };

    input::event::p1::onup::up = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_y = 0.0f;
        if(config::controls::p1_pola_x == 0.0f && config::controls::p1_pola_y == 0.0f)
            wte_set_component(player_id, cmp::velocity)->set_velocity(0.0f);
    };
    input::event::p1::onup::down = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_y = 0.0f;
        if(config::controls::p1_pola_x == 0.0f && config::controls::p1_pola_y == 0.0f)
            wte_set_component(player_id, cmp::velocity)->set_velocity(0.0f);
    };
    input::event::p1::onup::left = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_x = 0.0f;
        if(config::controls::p1_pola_x == 0.0f && config::controls::p1_pola_y == 0.0f)
            wte_set_component(player_id, cmp::velocity)->set_velocity(0.0f);
    };
    input::event::p1::onup::right = []() {
        entity_id player_id = mgr::entities::get_id("player");
        config::controls::p1_pola_x = 0.0f;
        if(config::controls::p1_pola_x == 0.0f && config::controls::p1_pola_y == 0.0f)
            wte_set_component(player_id, cmp::velocity)->set_velocity(0.0f);
    };
    input::event::p1::onup::action1 = []() {
        //  Turn the cannon off.
        entity_id can_id = mgr::entities::get_id("main_cannon");
        wte_set_component(can_id, cmp::visible)->hide();
        wte_set_component(can_id, cmp::enabled)->disable();
        //  Stop sound effect.
        mgr::audio::sample_stop("cannon_fire");
    };
    input::event::p1::onup::action2 = []() {
        entity_id player_id = mgr::entities::get_id("player");
        entity_id shd_id = mgr::entities::get_id("shield");
        //  Disable shield.
        wte_set_component(shd_id, cmp::visible)->hide();
        wte_set_component(shd_id, cmp::enabled)->disable();
        wte_set_component(player_id, cmp::hitbox)->make_solid();
        //  Stop sound effect.
        mgr::audio::sample_stop("shield_sound");
    };
}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {
    //mgr::variables::clear_save();
    //mgr::variables::save("max_lives");
    //mgr::variables::save("hiscore");

    al_shutdown_primitives_addon();
}

/*
 * Load menus.
 * Items will appear in the order they were added.
 */
void wte_demo::load_menus(void) {
    //  First set colors used by the menu.
    mgr::menu::set_menu_color(WTE_COLOR_WHITE, WTE_COLOR_DARKPURPLE);
    //  You can also set menu size here.  If not the default size is used.
    //mgr::menus::set_menu_size(200, 200, 10);

    //  Configure the root main menu and game menu.
    {
        //  Configure main menu.
        mgr::menu::set_menu("main_menu")->set_title("WTE Demo");
        mgr::menu::set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game", "game.sdf"));
        //mgr::menu::set_menu("main_menu")->add_item(wte_menu_action("New Game", "new_game"));
        mgr::menu::set_menu("main_menu")->add_item(wte_menu_action("Settings", "open_menu", "settings"));
        mgr::menu::set_menu("main_menu")->add_item(wte_menu_action("Exit Game", "exit"));
    }

    {
        //  Configure in-game menu.
        mgr::menu::set_menu("game_menu")->set_title("WTE Demo - Game Paused");
        mgr::menu::set_menu("game_menu")->add_item(wte_menu_action("Resume Game", "close_menu", "all"));
        mgr::menu::set_menu("game_menu")->add_item(wte_menu_action("Settings", "open_menu", "game_settings"));
        mgr::menu::set_menu("game_menu")->add_item(wte_menu_action("End Game", "end_game"));
        mgr::menu::set_menu("game_menu")->add_item(wte_menu_action("Exit Game", "exit"));
    }

    //  Then define other custom mgr::menus::
    {
        //  Create the main settings menu.
        mnu::menu temp_menu = mnu::menu("settings", "Settings");
        temp_menu.add_item(wte_menu_action("Demo Settings", "open_menu", "demo_settings"));
        temp_menu.add_item(wte_menu_action("Video Settings", "open_menu", "video_settings"));
        temp_menu.add_item(wte_menu_action("Audio Settings", "open_menu", "audio_settings"));
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!mgr::menu::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the in-game settings menu.
        mnu::menu temp_menu = mnu::menu("game_settings", "Settings");
        temp_menu.add_item(wte_menu_action("Video Settings", "open_menu", "video_settings"));
        temp_menu.add_item(wte_menu_action("Audio Settings", "open_menu", "audio_settings"));
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!mgr::menu::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the demo game settings menu.
        mnu::menu temp_menu = mnu::menu("demo_settings", "Demo Settings");
        std::vector<std::string> lives_vec = { "3", "4", "5" };
        temp_menu.add_item(wte_menu_selection("Lives:", "max_lives", lives_vec, lives_vec, mnu::GAME_SETTING));
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!mgr::menu::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
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
            [](void){ return config::flags::draw_fps; }));
        temp_menu.add_item(wte_menu_apply());
        temp_menu.add_item(wte_menu_action("Return", "close_menu"));
        if(!mgr::menu::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
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
        if(!mgr::menu::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
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
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "starfield");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, stars);
    wte_new_component(e_id, cmp::background, mgr::renderer::get_arena_width(),
                                             mgr::renderer::get_arena_height(), 0,
        [](const entity_id& bkg_id) {
            //  Define the animation process for the starfield.
            wte_set_component(bkg_id, cmp::background)->set_drawing();
            al_clear_to_color(WTE_COLOR_BLACK);

            //  Move the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                wte_set_component(bkg_id, stars)->y[i] +=
                    wte_get_component(bkg_id, stars)->speed[i] * wte_get_component(bkg_id, stars)->speed_mult;
                if(wte_get_component(bkg_id, stars)->y[i] > mgr::renderer::get_arena_height()) {
                    //  Make a new star.
                    wte_set_component(bkg_id, stars)->x[i] =
                        std::rand() % mgr::renderer::get_arena_width() + 1;
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
        [](const entity_id& bkg_id, const message& msg) {
            //  Define message processing for the starfield.
            if(msg.get_cmd() == "default") wte_set_component(bkg_id, stars)->speed_mult = 1;
            if(msg.get_cmd() == "up") wte_set_component(bkg_id, stars)->speed_mult *= 2;
            if(msg.get_cmd() == "down") wte_set_component(bkg_id, stars)->speed_mult /= 2;
            if(msg.get_cmd() == "reset") {
                wte_set_component(bkg_id, stars)->speed_mult = 1;

                for(std::size_t i = 0; i < MAX_STARS; i++) {
                    wte_set_component(bkg_id, stars)->x[i] =
                        std::rand() % mgr::renderer::get_arena_width() + 1;
                    wte_set_component(bkg_id, stars)->y[i] =
                        std::rand() % mgr::renderer::get_arena_height() + 1;
                    wte_set_component(bkg_id, stars)->speed[i] = (std::rand() % 3 + 1) * 3;
                    wte_set_component(bkg_id, stars)->color[i] = std::rand() % 4 + 1;
                }
            }
        }  //  End starfield message processing.
    );  //  End dispatcher component.

    /* ********************************* */
    /* *** Score overlay entity ******** */
    /* ********************************* */
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "score_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, 0, mgr::renderer::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            wte_set_component(ovr_id, cmp::overlay)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text(std::to_string(mgr::variables::get<int>("score")), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text(std::to_string(mgr::variables::get<int>("hiscore")), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    );  //  End score overlay drawing.
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /* ********************************* */
    /* *** Player Info overlay entity ** */
    /* ********************************* */
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "player_info_overlay");
    wte_new_component(e_id, cmp::visible);
    wte_new_component(e_id, cmp::overlay, 200, 20, mgr::renderer::get_arena_width() - 200,
                                                   mgr::renderer::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            entity_id shd_id = mgr::entities::get_id("shield");
            wte_set_component(ovr_id, cmp::overlay)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            al_draw_filled_rectangle((float)(120 - wte_get_component(shd_id, energy)->amt), 0, 120, 10, WTE_COLOR_YELLOW);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
            wte_set_component(ovr_id, cmp::overlay)->draw_text("Lives:  " + std::to_string(mgr::variables::get<int>("lives")), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
        }
    );  //  End info overlay drawing.
    wte_set_component(e_id, cmp::overlay)->set_font(al_create_builtin_font());

    /* ********************************* */
    /* *** Game Over overlay entity **** */
    /* ********************************* */
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "game_over_overlay");
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::overlay, 480, 132, (mgr::renderer::get_arena_width() / 2) - 240,
                                                    (mgr::renderer::get_arena_height() / 2) - 66, 1,
        [](const entity_id& ovr_id) {
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
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "player");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, (mgr::renderer::get_arena_width() / 2) - 5,
                                            mgr::renderer::get_arena_height() - 40);
    wte_new_component(e_id, cmp::hitbox, 10, 10);
    wte_new_component(e_id, cmp::bounding_box, 12.0f, 0.0f,
                                               (float)(mgr::renderer::get_arena_width() - 21),
                                               (float)(mgr::renderer::get_arena_height() - 32));
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

    //  Player logic.
    wte_new_component(e_id, cmp::ai,
        [](const entity_id& plr_id) {
            if(wte_get_component(plr_id, health)->hp <= 0) {  //  Check player health.
                wte_set_component(plr_id, cmp::enabled)->disable();
                wte_set_component(plr_id, health)->hp = wte_get_component(plr_id, health)->hp_max;
                std::string player_name = mgr::entities::get_name(plr_id);
                mgr::messages::add_message(message("entities", player_name, player_name, "death", ""));
            }
        }
    );  //  End player logic.

    //  Player message handling.
    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& plr_id, const message& msg) {
            //  Process player death.
            if(msg.get_cmd() == "death") {
                config::flags::input_enabled = false;
                //  Make sure cannon stops firing
                entity_id cannon_id = mgr::entities::get_id("main_cannon");
                wte_set_component(cannon_id, cmp::visible)->hide();
                wte_set_component(cannon_id, cmp::enabled)->disable();
                mgr::audio::sample_stop("cannon_fire");

                //  Just to make sure... turn shield off
                entity_id shield_id = mgr::entities::get_id("shield");
                wte_set_component(shield_id, cmp::visible)->hide();
                wte_set_component(shield_id, cmp::enabled)->disable();
                wte_set_component(plr_id, cmp::hitbox)->make_solid();
                mgr::audio::sample_stop("shield_sound");

                mgr::audio::sample_play("megumin", "once");
                mgr::variables::set<int>("lives", mgr::variables::get<int>("lives") - 1);
                wte_set_component(plr_id, cmp::velocity)->set_velocity(0.0f);
                wte_set_component(plr_id, cmp::sprite)->set_cycle("death");
                if(mgr::variables::get<int>("lives") == 0) {
                    //  Game over!
                    mgr::messages::add_message(message(engine_time::check_time() + 180, "system", "end_game", ""));
                    entity_id go_id = mgr::entities::get_id("game_over_overlay");
                    wte_set_component(go_id, cmp::visible)->show();
                } else {
                    std::string player_name = mgr::entities::get_name(plr_id);
                    mgr::messages::add_message(
                        message(engine_time::check_time() + 120, "entities", player_name, player_name, "reset", "")
                    );
                }
            }

            //  Reset player.
            if(msg.get_cmd() == "reset") {
                config::flags::input_enabled = true;
                wte_set_component(plr_id, cmp::location)->set_x((float)((mgr::renderer::get_arena_width() / 2) - 5));
                wte_set_component(plr_id, cmp::location)->set_y((float)(mgr::renderer::get_arena_height() - 40));
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
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "main_cannon");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 10, 200, false);
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, damage, 3);

    wte_new_component(e_id, cmp::sprite, 10.0f, 200.0f, 0.0f, 0.0f, 2, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("cannon.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 3);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    //  Cannon logic.
    wte_new_component(e_id, cmp::ai,
        [](const entity_id& can_id) {
            entity_id player_entity = mgr::entities::get_id("player");
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
        [](const entity_id& can_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
                                             std::to_string(wte_get_component(can_id, damage)->dmg)));
                //  Play the hit effect.
                mgr::audio::sample_play("fx3", "once");
            }
        }
    );  //  End cannon message processing.

    /* ********************************* */
    /* *** Shield entity *************** */
    /* ********************************* */
    e_id = mgr::entities::new_entity();
    mgr::entities::set_name(e_id, "shield");
    wte_new_component(e_id, cmp::team, 0);
    wte_new_component(e_id, cmp::location, 0, 0);
    wte_new_component(e_id, cmp::hitbox, 64, 64, false);
    wte_set_component(e_id, cmp::hitbox)->make_fluid();
    wte_new_component(e_id, cmp::visible, false);
    wte_new_component(e_id, cmp::enabled, false);
    wte_new_component(e_id, energy, 50, 100);
    wte_new_component(e_id, damage, 100);

    wte_new_component(e_id, cmp::sprite, 64.0f, 64.0f, 0.0f, 0.0f, 6, 2);
    wte_set_component(e_id, cmp::sprite)->load_sprite("shield.bmp");
    wte_set_component(e_id, cmp::sprite)->add_cycle("main", 0, 5);
    wte_set_component(e_id, cmp::sprite)->set_cycle("main");

    //  Shield logic.
    wte_new_component(e_id, cmp::ai,
        //  Enabeled AI.
        [](const entity_id& shd_id) {
            entity_id player_entity = mgr::entities::get_id("player");
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
                mgr::audio::sample_stop("shield_sound");
            }
        },
        //  Disabeled AI.
        [](const entity_id& shd_id) {
            //  Recharge the shield.
            if(wte_set_component(shd_id, energy)->amt < wte_set_component(shd_id, energy)->amt_max)
                wte_set_component(shd_id, energy)->amt += 1;
        }
    );  //  End shield logic.

    //  Shield message processing.
    wte_new_component(e_id, cmp::dispatcher,
        [](const entity_id& shd_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
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
    mgr::spawner::add_spawn("asteroid", 5,
        [](const entity_id& e_id, const msg_arg_list& args) {
            int temp_size = std::stoi(args[5]);
            if(temp_size < 1) temp_size = 1;
            if(temp_size > 8) temp_size = 8;

            mgr::entities::set_name(e_id, "asteroid" + std::to_string(e_id));
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
                [](const entity_id& ast_id) {
                    //  AI for asteroids defined here.
                    //  Perform OOB check.
                    if(wte_get_component(ast_id, cmp::location)->get_y() > (float)(mgr::renderer::get_arena_height() + 100)) {
                        mgr::messages::add_message(message("spawner", "delete", mgr::entities::get_name(ast_id)));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(wte_get_component(ast_id, health)->hp <= 0) {
                        mgr::messages::add_message(message("spawner", "delete", mgr::entities::get_name(ast_id)));
                        mgr::audio::sample_play("megumin", "once", 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.8f);

                        mgr::variables::set("score",
                            (mgr::variables::get<int>("score") + (10 * wte_get_component(ast_id, size)->the_size)));

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
                            mgr::messages::add_message(message("spawner", "new", new_spawner_a));
                            mgr::messages::add_message(message("spawner", "new", new_spawner_b));
                        }
                    }
                }
            );  //  End asteroid AI

            //  Asteroid message processing.
            wte_new_component(e_id, cmp::dispatcher,
                [](const entity_id& ast_id, const message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(), "damage",
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
    mgr::variables::set("score", 0);

    //  Set number of lives.
    if(mgr::variables::get<int>("max_lives") > 5 || mgr::variables::get<int>("max_lives") < 3)
        mgr::variables::set("max_lives", 3);
    mgr::variables::set("lives", mgr::variables::get<int>("max_lives"));

    //  Load some samples in the audio manager.
    mgr::audio::sample_load("sfx/laser.wav");
    mgr::audio::sample_load("sfx/shield.wav");
    mgr::audio::sample_load("sfx/megumin.wav");
}

/*
 * Stop the game.
 */
void wte_demo::end_game(void) {
    if(mgr::variables::get<int>("score") > mgr::variables::get<int>("hiscore"))
        mgr::variables::set("hiscore", mgr::variables::get<int>("score"));
}

/*
 * On menu open.
 */
void wte_demo::on_menu_open(void) {
    mgr::audio::music_pause();
    mgr::audio::ambiance_pause();
}

/*
 * On menu close.
 */
void wte_demo::on_menu_close(void) {
    mgr::audio::music_unpause();
    mgr::audio::ambiance_unpause();
}
