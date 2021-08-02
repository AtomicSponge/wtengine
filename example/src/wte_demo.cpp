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

#include <numeric>

#include <allegro5/allegro_primitives.h>

#include "include/wte_demo.hpp"
#include "include/my_components.hpp"

using namespace wte;

/*
 * Game's constructor.
 * Initialize game specific variables here.
 */
wte_demo::wte_demo(int argc, char **argv) : engine(argc, argv, "WTE Demo") {
    //  The engine doesn't use Allegro's primitives addon, so init it here.
    al_init_primitives_addon();

    mgr::variables::reg<int>("score", 0);
    mgr::variables::reg<int>("hiscore", 0);
    mgr::variables::reg<int>("max_lives", 3);
    mgr::variables::reg<int>("lives", 3);

    mgr::variables::set_data_file("game.cfg");
    mgr::variables::load<int>("max_lives");
    mgr::variables::load<int>("hiscore");

    /*
     * Set up input handling
     */
    input::event::p1::ondown::up = []() {
        entity_id player_id = mgr::world::get_id("player");
        config::controls::p1_polc_y = -1.0f;
        float rad = std::atan2(config::controls::p1_polc_y, config::controls::p1_polc_x);
        mgr::world::set_component<cmp::direction>(player_id)->set_radians(rad);
        mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(5.0f);
    };
    input::event::p1::ondown::down = []() {
        entity_id player_id = mgr::world::get_id("player");
        config::controls::p1_polc_y = 1.0f;
        float rad = std::atan2(config::controls::p1_polc_y, config::controls::p1_polc_x);
        mgr::world::set_component<cmp::direction>(player_id)->set_radians(rad);
        mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(5.0f);
    };
    input::event::p1::ondown::left = []() {
        entity_id player_id = mgr::world::get_id("player");
        config::controls::p1_polc_x = -1.0f;
        float rad = std::atan2(config::controls::p1_polc_y, config::controls::p1_polc_x);
        mgr::world::set_component<cmp::direction>(player_id)->set_radians(rad);
        mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(5.0f);
    };
    input::event::p1::ondown::right = []() {
        entity_id player_id = mgr::world::get_id("player");
        config::controls::p1_polc_x = 1.0f;
        float rad = std::atan2(config::controls::p1_polc_y, config::controls::p1_polc_x);
        mgr::world::set_component<cmp::direction>(player_id)->set_radians(rad);
        mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(5.0f);
    };
    input::event::p1::ondown::action1 = []() {
        entity_id player_id = mgr::world::get_id("player");
        entity_id can_id = mgr::world::get_id("main_cannon");
        //  Set the cannon's location to match the player.
        mgr::world::set_component<cmp::location>(can_id)->set_x(
            mgr::world::get_component<cmp::location>(player_id)->get_x()
        );
        mgr::world::set_component<cmp::location>(can_id)->set_y(
            mgr::world::get_component<cmp::location>(player_id)->get_y() -
            mgr::world::get_component<cmp::hitbox>(can_id)->get_height()
        );

        //  Turn the cannon on.
        mgr::world::set_component<cmp::visible>(can_id)->show();
        mgr::world::set_component<cmp::enabled>(can_id)->enable();
        //  Play sound effect.
        mgr::audio::sample_play("laser", "cannon_fire");
    };
    input::event::p1::ondown::action2 = []() {
        entity_id player_id = mgr::world::get_id("player");
        entity_id shd_id = mgr::world::get_id("shield");
        //  Set the shield's location to match the player
        mgr::world::set_component<cmp::location>(shd_id)->set_x(
            mgr::world::get_component<cmp::location>(player_id)->get_x() - 28.0f
        );
        mgr::world::set_component<cmp::location>(shd_id)->set_y(
            mgr::world::get_component<cmp::location>(player_id)->get_y() - 16.0f
        );

        if(mgr::world::set_component<energy>(shd_id)->amt > 0) {
            //  Enable the shield.
            mgr::world::set_component<cmp::visible>(shd_id)->show();
            mgr::world::set_component<cmp::enabled>(shd_id)->enable();
            mgr::world::set_component<cmp::hitbox>(player_id)->make_fluid();
            //  Play sound effect.
            mgr::audio::sample_play("shield", "shield_sound");
        }
    };

    input::event::p1::onup::up = []() {
        entity_id player_id = mgr::world::get_id("player");
        if(config::controls::p1_polc_y != 1.0f) config::controls::p1_polc_y = 0.0f;
        if(config::controls::p1_polc_x == 0.0f && config::controls::p1_polc_y == 0.0f)
            mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(0.0f);
    };
    input::event::p1::onup::down = []() {
        entity_id player_id = mgr::world::get_id("player");
        if(config::controls::p1_polc_y != -1.0f) config::controls::p1_polc_y = 0.0f;
        if(config::controls::p1_polc_x == 0.0f && config::controls::p1_polc_y == 0.0f)
            mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(0.0f);
    };
    input::event::p1::onup::left = []() {
        entity_id player_id = mgr::world::get_id("player");
        if(config::controls::p1_polc_x != 1.0f) config::controls::p1_polc_x = 0.0f;
        if(config::controls::p1_polc_x == 0.0f && config::controls::p1_polc_y == 0.0f)
            mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(0.0f);
    };
    input::event::p1::onup::right = []() {
        entity_id player_id = mgr::world::get_id("player");
        if(config::controls::p1_polc_x != -1.0f) config::controls::p1_polc_x = 0.0f;
        if(config::controls::p1_polc_x == 0.0f && config::controls::p1_polc_y == 0.0f)
            mgr::world::set_component<cmp::velocity>(player_id)->set_velocity(0.0f);
    };
    input::event::p1::onup::action1 = []() {
        //  Turn the cannon off.
        entity_id can_id = mgr::world::get_id("main_cannon");
        mgr::world::set_component<cmp::visible>(can_id)->hide();
        mgr::world::set_component<cmp::enabled>(can_id)->disable();
        //  Stop sound effect.
        mgr::audio::sample_stop("cannon_fire");
    };
    input::event::p1::onup::action2 = []() {
        entity_id player_id = mgr::world::get_id("player");
        entity_id shd_id = mgr::world::get_id("shield");
        //  Disable shield.
        mgr::world::set_component<cmp::visible>(shd_id)->hide();
        mgr::world::set_component<cmp::enabled>(shd_id)->disable();
        mgr::world::set_component<cmp::hitbox>(player_id)->make_solid();
        //  Stop sound effect.
        mgr::audio::sample_stop("shield_sound");
    };

    input::event::p2::ondown::up = input::event::p1::ondown::up;
    input::event::p2::ondown::down = input::event::p1::ondown::down;
    input::event::p2::ondown::left = input::event::p1::ondown::left;
    input::event::p2::ondown::right = input::event::p1::ondown::right;
    input::event::p2::ondown::action1 = input::event::p1::ondown::action1;
    input::event::p2::ondown::action2 = input::event::p1::ondown::action2;
    input::event::p2::onup::up = input::event::p1::onup::up;
    input::event::p2::onup::down = input::event::p1::onup::down;
    input::event::p2::onup::left = input::event::p1::onup::left;
    input::event::p2::onup::right = input::event::p1::onup::right;
    input::event::p2::onup::action1 = input::event::p1::onup::action1;
    input::event::p2::onup::action2 = input::event::p1::onup::action2;
}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {
    mgr::variables::clear_save();
    mgr::variables::save<int>("max_lives");
    mgr::variables::save<int>("hiscore");

    al_shutdown_primitives_addon();
}

/*
 * Load menus.
 * Items will appear in the order they were added.
 */
void wte_demo::load_menus(void) {
    //  Create a menu background and load into menu mgr.
    mgr::assets::load<al_bitmap>("menu_background", 300, 200);
    al_set_target_bitmap(**mgr::assets::get<al_bitmap>("menu_background"));
    al_clear_to_color(WTE_COLOR_DARKPURPLE);
    mgr::menus::set_background(mgr::assets::get<al_bitmap>("menu_background"));

    //  Set menu mgr font.
    mgr::menus::set_font(mgr::assets::get<al_font>("wte_default_font"), WTE_COLOR_WHITE);

    //  Create a cursor bitmap and load into menu mgr.
    mgr::assets::load<al_bitmap>("cursor", 8, 8);
    al_set_target_bitmap(**mgr::assets::get<al_bitmap>("cursor"));
    al_clear_to_color(WTE_COLOR_DARKPURPLE);
    mgr::menus::set_cursor(mgr::assets::get<al_bitmap>("cursor"));

    //  Configure the root main menu and game menu.
    {
        //  Configure main menu.
        mgr::menus::set_menu("main_menu")->set_title("WTE Demo");
        mgr::menus::set_menu("main_menu")->add_item<mnu::action>("New Game", "new_game", "game.sdf");
        //mgr::menus::set_menu("main_menu")->add_item<mnu::action>("New Game", "new_game");
        mgr::menus::set_menu("main_menu")->add_item<mnu::action>("Settings", "open_menu", "settings");
        mgr::menus::set_menu("main_menu")->add_item<mnu::action>("Exit Game", "exit");
    }

    {
        //  Configure in-game menu.
        mgr::menus::set_menu("game_menu")->set_title("WTE Demo - Game Paused");
        mgr::menus::set_menu("game_menu")->add_item<mnu::action>("Resume Game", "close_menu", "all");
        mgr::menus::set_menu("game_menu")->add_item<mnu::action>("Settings", "open_menu", "game_settings");
        mgr::menus::set_menu("game_menu")->add_item<mnu::action>("End Game", "end_game");
        mgr::menus::set_menu("game_menu")->add_item<mnu::action>("Exit Game", "exit");
    }

    //  Then define other custom menus.
    {
        //  Create the main settings menu.
        mnu::menu temp_menu = mnu::menu("settings", "Settings");
        temp_menu.add_item<mnu::action>("Demo Settings", "open_menu", "demo_settings");
        temp_menu.add_item<mnu::action>("Video Settings", "open_menu", "video_settings");
        temp_menu.add_item<mnu::action>("Audio Settings", "open_menu", "audio_settings");
        temp_menu.add_item<mnu::action>("Return", "close_menu");
        if(!mgr::menus::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the in-game settings menu.
        mnu::menu temp_menu = mnu::menu("game_settings", "Settings");
        temp_menu.add_item<mnu::action>("Video Settings", "open_menu", "video_settings");
        temp_menu.add_item<mnu::action>("Audio Settings", "open_menu", "audio_settings");
        temp_menu.add_item<mnu::action>("Return", "close_menu");
        if(!mgr::menus::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the demo game settings menu.
        mnu::menu temp_menu = mnu::menu("demo_settings", "Demo Settings");
        std::vector<std::string> lives_vec = { "3", "4", "5" };
        temp_menu.add_item<mnu::selection>("Lives:", "max_lives", lives_vec, lives_vec, mnu::GAME_SETTING);
        temp_menu.add_item<mnu::apply>();
        temp_menu.add_item<mnu::action>("Return", "close_menu");
        if(!mgr::menus::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    {
        //  Create the video settings menu.
        mnu::menu temp_menu = mnu::menu("video_settings", "Video Settings");
        std::vector<std::string> mode_dvec = { "Windowed", "Windowed Full Screen" };
        std::vector<std::string> mode_vec = { "windowed", "windowed_full_screen" };
        temp_menu.add_item<mnu::selection>("Display Mode:", "display_mode", mode_dvec, mode_vec, mnu::ENGINE_SETTING_RECONF);
        std::vector<std::string> scale_vec = { "0.5", "1", "1.25", "1.5", "1.75", "2" };
        temp_menu.add_item<mnu::selection>("Scale factor:", "scale_factor", scale_vec, scale_vec, mnu::ENGINE_SETTING_RECONF);
        temp_menu.add_item<mnu::toggle>("FPS:", "fps_counter", "on", "fps_counter", "off",
                                        [](void){ return config::flags::draw_fps; });
        temp_menu.add_item<mnu::apply>();
        temp_menu.add_item<mnu::action>("Return", "close_menu");
        if(!mgr::menus::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
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
        temp_menu.add_item<mnu::selection>("Main Volume:", "main_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING);
        temp_menu.add_item<mnu::selection>("Music Volume:", "mix1_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING);
        temp_menu.add_item<mnu::selection>("Effects Volume:", "mix2_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING);
        temp_menu.add_item<mnu::selection>("Voice Volume:", "mix3_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING);
        temp_menu.add_item<mnu::selection>("Ambiance Volume:", "mix4_vol", vol_dvec, vol_vec, mnu::AUDIO_SETTING);
        temp_menu.add_item<mnu::apply>();
        temp_menu.add_item<mnu::action>("Return", "close_menu");
        if(!mgr::menus::new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    mgr::systems::add(std::make_unique<sys::movement>());
    mgr::systems::add(std::make_unique<sys::colision>());
    mgr::systems::add(std::make_unique<sys::logic>());
    mgr::systems::add(std::make_unique<sys::animate>());
}

/*
 * New game process.
 * Create starting world and load world into the spawner.
 */
void wte_demo::new_game(void) {
    entity_id e_id;

    /* ********************************* */
    /* *** Background entity *********** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "starfield");
    mgr::world::add_component<cmp::visible>(e_id);
    mgr::world::add_component<stars>(e_id);
    mgr::assets::load<al_bitmap>("starfield", mgr::renderer::get_arena_width(), mgr::renderer::get_arena_height(), true);
    mgr::world::add_component<cmp::background>(e_id, mgr::assets::get<al_bitmap>("starfield"), 0,
        [](const entity_id& bkg_id) {
            //  Define the animation process for the starfield.
            mgr::world::set_component<cmp::background>(bkg_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_BLACK);

            //  Move the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                mgr::world::set_component<stars>(bkg_id)->y[i] +=
                    mgr::world::get_component<stars>(bkg_id)->speed[i] * mgr::world::get_component<stars>(bkg_id)->speed_mult;
                if(mgr::world::get_component<stars>(bkg_id)->y[i] > mgr::renderer::get_arena_height()) {
                    //  Make a new star.
                    mgr::world::set_component<stars>(bkg_id)->x[i] =
                        std::rand() % mgr::renderer::get_arena_width() + 1;
                    mgr::world::set_component<stars>(bkg_id)->y[i] = 0;
                    mgr::world::set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                    mgr::world::set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                }
            }

            //  Draw the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                if(mgr::world::get_component<stars>(bkg_id)->color[i] == 1 ||
                   mgr::world::get_component<stars>(bkg_id)->color[i] == 4)
                    al_draw_pixel(mgr::world::get_component<stars>(bkg_id)->x[i],
                                  mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_WHITE);
                if(mgr::world::get_component<stars>(bkg_id)->color[i] == 2)
                    al_draw_pixel(mgr::world::get_component<stars>(bkg_id)->x[i],
                                  mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_YELLOW);
                if(mgr::world::get_component<stars>(bkg_id)->color[i] == 3)
                    al_draw_pixel(mgr::world::get_component<stars>(bkg_id)->x[i],
                                  mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_RED);
            }
        }
    );  //  End background rendering.

    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& bkg_id, const message& msg) {
            //  Define message processing for the starfield.
            if(msg.get_cmd() == "default") mgr::world::set_component<stars>(bkg_id)->speed_mult = 1;
            if(msg.get_cmd() == "up") mgr::world::set_component<stars>(bkg_id)->speed_mult *= 2;
            if(msg.get_cmd() == "down") mgr::world::set_component<stars>(bkg_id)->speed_mult /= 2;
            if(msg.get_cmd() == "reset") {
                mgr::world::set_component<stars>(bkg_id)->speed_mult = 1;

                for(std::size_t i = 0; i < MAX_STARS; i++) {
                    mgr::world::set_component<stars>(bkg_id)->x[i] =
                        std::rand() % mgr::renderer::get_arena_width() + 1;
                    mgr::world::set_component<stars>(bkg_id)->y[i] =
                        std::rand() % mgr::renderer::get_arena_height() + 1;
                    mgr::world::set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                    mgr::world::set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                }
            }
        }  //  End starfield message processing.
    );  //  End dispatcher component.

    /* ********************************* */
    /* *** Score overlay entity ******** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "score_overlay");
    mgr::world::add_component<cmp::visible>(e_id);
    mgr::assets::load<al_bitmap>("score_overlay", 200, 20, true);
    mgr::world::add_component<cmp::overlay>(e_id,
        mgr::assets::get<al_bitmap>("score_overlay"),
        mgr::assets::get<al_font>("wte_default_font"),
        0, mgr::renderer::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            mgr::world::set_component<cmp::overlay>(ovr_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text(std::to_string(mgr::variables::get<int>("score")), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text(std::to_string(mgr::variables::get<int>("hiscore")), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    );  //  End score overlay drawing.

    /* ********************************* */
    /* *** Player Info overlay entity ** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "player_info_overlay");
    mgr::world::add_component<cmp::visible>(e_id);
    mgr::assets::load<al_bitmap>("player_info_overlay", 200, 20, true);
    mgr::world::add_component<cmp::overlay>(e_id,
        mgr::assets::get<al_bitmap>("player_info_overlay"),
        mgr::assets::get<al_font>("wte_default_font"),
        mgr::renderer::get_arena_width() - 200,
        mgr::renderer::get_arena_height() - 20, 0,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            entity_id shd_id = mgr::world::get_id("shield");
            mgr::world::set_component<cmp::overlay>(ovr_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            al_draw_filled_rectangle((float)(120 - mgr::world::get_component<energy>(shd_id)->amt), 0, 120, 10, WTE_COLOR_YELLOW);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::overlay>(ovr_id)->draw_text("Lives:  " + std::to_string(mgr::variables::get<int>("lives")), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
        }
    );  //  End info overlay drawing.

    /* ********************************* */
    /* *** Game Over overlay entity **** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "game_over_overlay");
    mgr::world::add_component<cmp::visible>(e_id, false);
    //mgr::assets::load<al_bitmap>("game_over_overlay", "game_over.bmp");
    mgr::assets::load<al_bitmap>("game_over_overlay");
    mgr::assets::get<al_bitmap>("game_over_overlay")->load("game_over.bmp");
    mgr::world::add_component<cmp::overlay>(e_id,
        mgr::assets::get<al_bitmap>("game_over_overlay"),
        mgr::assets::get<al_font>("wte_default_font"),
        (mgr::renderer::get_arena_width() / 2) - 240,
        (mgr::renderer::get_arena_height() / 2) - 66, 1,
        [](const entity_id& ovr_id) {}
    );

    /* ********************************* */
    /* *** Player entity *************** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "player");
    mgr::world::add_component<cmp::team>(e_id, 0);
    mgr::world::add_component<cmp::location>(e_id,
        (mgr::renderer::get_arena_width() / 2) - 5,
         mgr::renderer::get_arena_height() - 40);
    mgr::world::add_component<cmp::hitbox>(e_id, 10, 10);
    mgr::world::add_component<cmp::bounding_box>(e_id, 12.0f, 0.0f,
        (float)(mgr::renderer::get_arena_width() - 21),
        (float)(mgr::renderer::get_arena_height() - 32));
    mgr::world::add_component<health>(e_id, 1, 1);
    mgr::world::add_component<cmp::visible>(e_id);
    mgr::world::add_component<cmp::enabled>(e_id);
    mgr::world::add_component<cmp::direction>(e_id, false);
    mgr::world::add_component<cmp::velocity>(e_id);

    //mgr::assets::load<al_bitmap>("ship", "ship.bmp");
    mgr::assets::load<al_bitmap>("ship");
    mgr::assets::get<al_bitmap>("ship")->load("ship.bmp");
    mgr::world::add_component<cmp::sprite>(e_id, mgr::assets::get<al_bitmap>("ship"),
                                           32.0f, 32.0f, -11.0f, 0.0f, 1, 1);
    mgr::world::set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 3);
    mgr::world::set_component<cmp::sprite>(e_id)->add_cycle("death", 4, 7);
    mgr::world::set_component<cmp::sprite>(e_id)->set_cycle("main");

    //  Player logic.
    mgr::world::add_component<cmp::ai>(e_id,
        [](const entity_id& plr_id) {
            if(mgr::world::get_component<health>(plr_id)->hp <= 0) {  //  Check player health.
                mgr::world::set_component<cmp::enabled>(plr_id)->disable();
                mgr::world::set_component<health>(plr_id)->hp = mgr::world::get_component<health>(plr_id)->hp_max;
                std::string player_name = mgr::world::get_name(plr_id);
                mgr::messages::add_message(message("entities", player_name, player_name, "death", ""));
            }
        }
    );  //  End player logic.

    //  Player message handling.
    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& plr_id, const message& msg) {
            //  Process player death.
            if(msg.get_cmd() == "death") {
                config::flags::input_enabled = false;
                //  Make sure cannon stops firing
                entity_id cannon_id = mgr::world::get_id("main_cannon");
                mgr::world::set_component<cmp::visible>(cannon_id)->hide();
                mgr::world::set_component<cmp::enabled>(cannon_id)->disable();
                mgr::audio::sample_stop("cannon_fire");

                //  Just to make sure... turn shield off
                entity_id shield_id = mgr::world::get_id("shield");
                mgr::world::set_component<cmp::visible>(shield_id)->hide();
                mgr::world::set_component<cmp::enabled>(shield_id)->disable();
                mgr::world::set_component<cmp::hitbox>(plr_id)->make_solid();
                mgr::audio::sample_stop("shield_sound");

                mgr::audio::sample_play("megumin", "once");
                mgr::variables::set<int>("lives", mgr::variables::get<int>("lives") - 1);
                mgr::world::set_component<cmp::velocity>(plr_id)->set_velocity(0.0f);
                mgr::world::set_component<cmp::sprite>(plr_id)->set_cycle("death");
                if(mgr::variables::get<int>("lives") == 0) {
                    //  Game over!
                    mgr::messages::add_message(message(engine_time::check_time() + 180, "system", "end_game", ""));
                    entity_id go_id = mgr::world::get_id("game_over_overlay");
                    mgr::world::set_component<cmp::visible>(go_id)->show();
                } else {
                    std::string player_name = mgr::world::get_name(plr_id);
                    mgr::messages::add_message(
                        message(engine_time::check_time() + 120, "entities", player_name, player_name, "reset", "")
                    );
                }
            }

            //  Reset player.
            if(msg.get_cmd() == "reset") {
                config::flags::input_enabled = true;
                config::controls::p1_polc_x = 0.0f;
                config::controls::p1_polc_y = 0.0f;
                mgr::world::set_component<cmp::velocity>(plr_id)->set_velocity(0.0f);
                mgr::world::set_component<cmp::location>(plr_id)->set_x((float)((mgr::renderer::get_arena_width() / 2) - 5));
                mgr::world::set_component<cmp::location>(plr_id)->set_y((float)(mgr::renderer::get_arena_height() - 40));
                mgr::world::set_component<health>(plr_id)->hp = mgr::world::get_component<health>(plr_id)->hp_max;
                mgr::world::set_component<cmp::enabled>(plr_id)->enable();
                mgr::world::set_component<cmp::sprite>(plr_id)->set_cycle("main");
            }

            //  Take damage.
            if(msg.get_cmd() == "damage") {
                mgr::world::set_component<health>(plr_id)->hp -= std::stoi(msg.get_arg(0));
            }
        }
    );  //  End player message processing.

    /* ********************************* */
    /* *** Main cannon entity ********** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "main_cannon");
    mgr::world::add_component<cmp::team>(e_id, 0);
    mgr::world::add_component<cmp::location>(e_id, 0, 0);
    mgr::world::add_component<cmp::hitbox>(e_id, 10, 200, false);
    mgr::world::add_component<cmp::visible>(e_id, false);
    mgr::world::add_component<cmp::enabled>(e_id, false);
    mgr::world::add_component<damage>(e_id, 3);

    //mgr::assets::load<al_bitmap>("cannon", "cannon.bmp");
    mgr::assets::load<al_bitmap>("cannon");
    mgr::assets::get<al_bitmap>("cannon")->load("cannon.bmp");
    mgr::world::add_component<cmp::sprite>(e_id, mgr::assets::get<al_bitmap>("cannon"),
                                           10.0f, 200.0f, 0.0f, 0.0f, 2, 2);
    mgr::world::set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 3);
    mgr::world::set_component<cmp::sprite>(e_id)->set_cycle("main");

    //  Cannon logic.
    mgr::world::add_component<cmp::ai>(e_id,
        [](const entity_id& can_id) {
            entity_id player_entity = mgr::world::get_id("player");
            //  Set the cannon's location to match the player.
            mgr::world::set_component<cmp::location>(can_id)->set_x(
                mgr::world::get_component<cmp::location>(player_entity)->get_x()
            );
            mgr::world::set_component<cmp::location>(can_id)->set_y(
                mgr::world::get_component<cmp::location>(player_entity)->get_y() -
                mgr::world::get_component<cmp::hitbox>(can_id)->get_height()
            );
        }
    );  //  End cannon logic.

    //  Cannon message processing.
    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& can_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(),
                    "damage", std::to_string(mgr::world::get_component<damage>(can_id)->dmg)));
                //  Play the hit effect.
                mgr::audio::sample_play("fx3", "once");
            }
        }
    );  //  End cannon message processing.

    /* ********************************* */
    /* *** Shield entity *************** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "shield");
    mgr::world::add_component<cmp::team>(e_id, 0);
    mgr::world::add_component<cmp::location>(e_id, 0, 0);
    mgr::world::add_component<cmp::hitbox>(e_id, 64, 64, false);
    mgr::world::set_component<cmp::hitbox>(e_id)->make_fluid();
    mgr::world::add_component<cmp::visible>(e_id, false);
    mgr::world::add_component<cmp::enabled>(e_id, false);
    mgr::world::add_component<energy>(e_id, 50, 100);
    mgr::world::add_component<damage>(e_id, 100);

    //mgr::assets::load<al_bitmap>("shield", "shield.bmp");
    mgr::assets::load<al_bitmap>("shield");
    mgr::assets::get<al_bitmap>("shield")->load("shield.bmp");
    mgr::world::add_component<cmp::sprite>(e_id, mgr::assets::get<al_bitmap>("shield"),
                                           64.0f, 64.0f, 0.0f, 0.0f, 6, 2);
    mgr::world::set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 5);
    mgr::world::set_component<cmp::sprite>(e_id)->set_cycle("main");

    //  Shield logic.
    mgr::world::add_component<cmp::ai>(e_id,
        //  Enabeled AI.
        [](const entity_id& shd_id) {
            entity_id player_entity = mgr::world::get_id("player");
            //  Set the shield's location to match the player.
            mgr::world::set_component<cmp::location>(shd_id)->set_x(
                mgr::world::get_component<cmp::location>(player_entity)->get_x() - 28.0f
            );
            mgr::world::set_component<cmp::location>(shd_id)->set_y(
                mgr::world::get_component<cmp::location>(player_entity)->get_y() - 16.0f
            );

            //  Drain the shield.
            if(mgr::world::set_component<energy>(shd_id)->amt > 0)
                mgr::world::set_component<energy>(shd_id)->amt -= 1;

            if(mgr::world::get_component<energy>(shd_id)->amt <= 0) {
                //  Disable shield.
                mgr::world::set_component<cmp::visible>(shd_id)->hide();
                mgr::world::set_component<cmp::enabled>(shd_id)->disable();
                mgr::world::set_component<cmp::hitbox>(player_entity)->make_solid();
                //  Stop sound effect.
                mgr::audio::sample_stop("shield_sound");
            }
        },
        //  Disabeled AI.
        [](const entity_id& shd_id) {
            //  Recharge the shield.
            if(mgr::world::set_component<energy>(shd_id)->amt < mgr::world::set_component<energy>(shd_id)->amt_max)
                mgr::world::set_component<energy>(shd_id)->amt += 1;
        }
    );  //  End shield logic.

    //  Shield message processing.
    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& shd_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(),
                    "damage", std::to_string(mgr::world::get_component<damage>(shd_id)->dmg)));
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
    //mgr::assets::load<al_bitmap>("asteroid", "asteroid.bmp");
    mgr::assets::load<al_bitmap>("asteroid");
    mgr::assets::get<al_bitmap>("asteroid")->load("asteroid.bmp");
    mgr::spawner::add_spawn("asteroid", 5,
        [](const entity_id& e_id, const msg_args& args) {
            int temp_size = std::stoi(args[5]);
            if(temp_size < 1) temp_size = 1;
            if(temp_size > 8) temp_size = 8;

            mgr::world::set_name(e_id, "asteroid" + std::to_string(e_id));
            mgr::world::add_component<cmp::team>(e_id, 1);
            mgr::world::add_component<cmp::location>(e_id, std::stof(args[1]), std::stof(args[2]));
            mgr::world::add_component<cmp::hitbox>(e_id, (float)(temp_size * 16), (float)(temp_size * 16));
            mgr::world::add_component<health>(e_id, temp_size * 10, temp_size * 10);
            mgr::world::add_component<damage>(e_id, 10);
            mgr::world::add_component<size>(e_id, temp_size);
            mgr::world::add_component<cmp::direction>(e_id);
            mgr::world::set_component<cmp::direction>(e_id)->set_degrees(std::stof(args[3]));
            mgr::world::add_component<cmp::velocity>(e_id, std::stof(args[4]));
            mgr::world::add_component<cmp::visible>(e_id);
            mgr::world::add_component<cmp::enabled>(e_id);

            mgr::world::add_component<cmp::sprite>(e_id, mgr::assets::get<al_bitmap>("asteroid"),
                16.0f, 16.0f, 0.0f, 0.0f, (int)(30 / std::stof(args[4])), 0);
            mgr::world::set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 5);
            mgr::world::set_component<cmp::sprite>(e_id)->set_cycle("main");
            mgr::world::set_component<cmp::sprite>(e_id)->set_scale_factor_x((float)temp_size);
            mgr::world::set_component<cmp::sprite>(e_id)->set_scale_factor_y((float)temp_size);

            //  Asteroid logic.
            mgr::world::add_component<cmp::ai>(e_id,
                [](const entity_id& ast_id) {
                    //  AI for asteroids defined here.
                    //  Perform OOB check.
                    if(mgr::world::get_component<cmp::location>(ast_id)->get_y() > (float)(mgr::renderer::get_arena_height() + 100)) {
                        mgr::messages::add_message(message("spawner", "delete", mgr::world::get_name(ast_id)));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(mgr::world::get_component<health>(ast_id)->hp <= 0) {
                        mgr::messages::add_message(message("spawner", "delete", mgr::world::get_name(ast_id)));
                        mgr::audio::sample_play("megumin", "once", 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.8f);

                        mgr::variables::set("score",
                            (mgr::variables::get<int>("score") +
                            (10 * mgr::world::get_component<size>(ast_id)->the_size)));

                        //  If the asteroid was size >= 4, split into two.
                        if(mgr::world::get_component<size>(ast_id)->the_size >= 4) {
                            const int new_size = mgr::world::get_component<size>(ast_id)->the_size / 2;
                            float dir_a = mgr::world::get_component<cmp::direction>(ast_id)->get_degrees() - 90.0f;
                            if(dir_a < 0.0f) dir_a = 0.0f;
                            float dir_b = mgr::world::get_component<cmp::direction>(ast_id)->get_degrees() + 90.0f;
                            if(dir_b > 360.0f) dir_b = 360.0f;
                            const float new_x = mgr::world::get_component<cmp::location>(ast_id)->get_x();
                            const float new_y = mgr::world::get_component<cmp::location>(ast_id)->get_y();
                            const float new_vel = mgr::world::get_component<cmp::velocity>(ast_id)->get_x_vel() / 2;
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
            mgr::world::add_component<cmp::dispatcher>(e_id,
                [](const entity_id& ast_id, const message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        mgr::messages::add_message(message("entities", msg.get_from(), msg.get_to(),
                            "damage", std::to_string(mgr::world::get_component<damage>(ast_id)->dmg)));
                    }

                    if(msg.get_cmd() == "damage") {
                        mgr::world::set_component<health>(ast_id)->hp -= std::stoi(msg.get_arg(0));
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
    mgr::audio::sample_load("sfx/laser.wav", "laser");
    mgr::audio::sample_load("sfx/shield.wav", "shield");
    mgr::audio::sample_load("sfx/megumin.wav", "megumin");
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
