/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  wte_demo.cpp
 *
 * See LICENSE.md for copyright information
 *
 * WTE Demo game code.
 * This extends the engine object to define a custom game.
 */

#include <numeric>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>

#include <wte_demo.hpp>
#include <my_components.hpp>

using namespace wte;

namespace layer {
    const std::size_t background = 0;
    const std::size_t enemy = 1;
    const std::size_t player = 2;
    const std::size_t overlay = 0;
}

/*
 * Game's constructor.
 * Initialize game specific variables here.
 */
wte_demo::wte_demo(int argc, char **argv) : engine(argc, argv) {
    //  Init Allegro addons that the engine does not use.
    al_init_primitives_addon();
    al_init_acodec_addon();

    mgr::variables::reg<int>("score", 0);
    mgr::variables::reg<int>("hiscore", 0);
    mgr::variables::reg<int>("max_lives", 3);
    mgr::variables::reg<int>("lives", 3);

    mgr::variables::set_data_file("game.cfg");
    mgr::variables::load<int>("max_lives");
    mgr::variables::load<int>("hiscore");

    /*
     * Set up input handling - WIP
     */
    add_handler<WTE_NONGAME_HANDLES, WTE_EVENT_KEY_DOWN, handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        mgr::messages::add(message("system", "new-game", "game.sdf"));
    });

    add_handler<WTE_GAME_HANDLES, WTE_EVENT_KEY_DOWN, handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == config::controls::p1_key_up) {
            entity_id player_id = mgr::world::get_id("player");
            const float rad = std::atan2(player_pols::y, player_pols::x);
            mgr::world::set_component<cmp::motion>(player_id)->direction = rad;
            mgr::world::set_component<cmp::motion>(player_id)->x_vel = 5.0f;
            mgr::world::set_component<cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_down) {
            entity_id player_id = mgr::world::get_id("player");
            const float rad = std::atan2(player_pols::y, player_pols::x);
            mgr::world::set_component<cmp::motion>(player_id)->direction = rad;
            mgr::world::set_component<cmp::motion>(player_id)->x_vel = 5.0f;
            mgr::world::set_component<cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_left) {
            entity_id player_id = mgr::world::get_id("player");
            const float rad = std::atan2(player_pols::y, player_pols::x);
            mgr::world::set_component<cmp::motion>(player_id)->direction = rad;
            mgr::world::set_component<cmp::motion>(player_id)->x_vel = 5.0f;
            mgr::world::set_component<cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_right) {
            entity_id player_id = mgr::world::get_id("player");
            const float rad = std::atan2(player_pols::y, player_pols::x);
            mgr::world::set_component<cmp::motion>(player_id)->direction = rad;
            mgr::world::set_component<cmp::motion>(player_id)->x_vel = 5.0f;
            mgr::world::set_component<cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_action1) {
            entity_id player_id = mgr::world::get_id("player");
            entity_id can_id = mgr::world::get_id("main_cannon");
            //  Set the cannon's location to match the player.
            mgr::world::set_component<cmp::location>(can_id)->pos_x = 
                mgr::world::get_component<cmp::location>(player_id)->pos_x;
            mgr::world::set_component<cmp::location>(can_id)->pos_y = 
                mgr::world::get_component<cmp::location>(player_id)->pos_y -
                mgr::world::get_component<cmp::hitbox>(can_id)->height;

            //  Turn the cannon on.
            mgr::world::set_component<cmp::gfx::sprite>(can_id)->visible = true;
            mgr::world::set_component<cmp::ai>(can_id)->enabled = true;
            mgr::world::set_component<cmp::hitbox>(can_id)->solid = true;
            //  Play sound effect.
            mgr::audio::sample::play(mgr::assets<al_sample>::get<al_sample>("laser"), "cannon_fire");
        }
        if(key == config::controls::p1_key_action2) {
            entity_id player_id = mgr::world::get_id("player");
            entity_id shd_id = mgr::world::get_id("shield");
            //  Set the shield's location to match the player
            mgr::world::set_component<cmp::location>(shd_id)->pos_x =
                mgr::world::get_component<cmp::location>(player_id)->pos_x - 28.0f;
            mgr::world::set_component<cmp::location>(shd_id)->pos_y =
                mgr::world::get_component<cmp::location>(player_id)->pos_y - 16.0f;

            if(mgr::world::set_component<energy>(shd_id)->amt > 0) {
                //  Enable the shield.
                mgr::world::set_component<cmp::gfx::sprite>(shd_id)->visible = true;
                mgr::world::set_component<cmp::ai>(shd_id)->enabled = true;
                mgr::world::set_component<cmp::hitbox>(shd_id)->solid = true;
                mgr::world::set_component<cmp::hitbox>(player_id)->solid = false;
                //  Play sound effect.
                mgr::audio::sample::play(mgr::assets<al_sample>::get<al_sample>("shield"), "shield_sound");
            }
        }
    });

    add_handler<WTE_GAME_HANDLES, WTE_EVENT_KEY_UP, handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == config::controls::p1_key_up) {
            entity_id player_id = mgr::world::get_id("player");
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                mgr::world::set_component<cmp::motion>(player_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_down) {
            entity_id player_id = mgr::world::get_id("player");
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                mgr::world::set_component<cmp::motion>(player_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_left) {
            entity_id player_id = mgr::world::get_id("player");
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                mgr::world::set_component<cmp::motion>(player_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_right) {
            entity_id player_id = mgr::world::get_id("player");
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                mgr::world::set_component<cmp::motion>(player_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_action1) {
            //  Turn the cannon off.
            entity_id can_id = mgr::world::get_id("main_cannon");
            mgr::world::set_component<cmp::gfx::sprite>(can_id)->visible = false;
            mgr::world::set_component<cmp::ai>(can_id)->enabled = false;
            mgr::world::set_component<cmp::hitbox>(can_id)->solid = false;
            //  Stop sound effect.
            mgr::audio::sample::stop("cannon_fire");
        }
        if(key == config::controls::p1_key_action2) {
            entity_id player_id = mgr::world::get_id("player");
            entity_id shd_id = mgr::world::get_id("shield");
            //  Disable shield.
            mgr::world::set_component<cmp::gfx::sprite>(shd_id)->visible = false;
            mgr::world::set_component<cmp::ai>(shd_id)->enabled = false;
            mgr::world::set_component<cmp::hitbox>(shd_id)->solid = false;
            mgr::world::set_component<cmp::hitbox>(player_id)->solid = true;
            //  Stop sound effect.
            mgr::audio::sample::stop("shield_sound");
        }
    });
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
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    mgr::systems::add(std::make_unique<sys::movement>());
    mgr::systems::add(std::make_unique<sys::colision>());
    mgr::systems::add(std::make_unique<sys::logic>());
    mgr::systems::add(std::make_unique<sys::gfx::animate>());
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
    mgr::world::add_component<stars>(e_id);
    mgr::assets<al_bitmap>::load<al_bitmap>(
        "starfield",
        al_bitmap(
            config::gfx::arena_w,
            config::gfx::arena_h,
            true
        )
    );
    mgr::world::add_component<cmp::gfx::background>(e_id,
        mgr::assets<al_bitmap>::get<al_bitmap>("starfield"), layer::background, 0, 0,
        [](const entity_id& bkg_id) {
            //  Define the animation process for the starfield.
            mgr::world::set_component<cmp::gfx::background>(bkg_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_BLACK);

            //  Move the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                mgr::world::set_component<stars>(bkg_id)->y[i] +=
                    mgr::world::get_component<stars>(bkg_id)->speed[i] * mgr::world::get_component<stars>(bkg_id)->speed_mult;
                if(mgr::world::get_component<stars>(bkg_id)->y[i] > config::gfx::arena_h) {
                    //  Make a new star.
                    mgr::world::set_component<stars>(bkg_id)->x[i] =
                        std::rand() % config::gfx::arena_w + 1;
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
                        std::rand() % config::gfx::arena_w + 1;
                    mgr::world::set_component<stars>(bkg_id)->y[i] =
                        std::rand() % config::gfx::arena_h + 1;
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
    mgr::assets<al_bitmap>::load<al_bitmap>("score_overlay", al_bitmap(200, 20, true));
    mgr::world::add_component<cmp::gfx::overlay>(e_id,
        mgr::assets<al_bitmap>::get<al_bitmap>("score_overlay"),
        mgr::assets<al_font>::get<al_font>("wte_default_font"),
        layer::overlay, config::gfx::arena_h - 20, 0,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text(std::to_string(mgr::variables::get<int>("score")), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text(std::to_string(mgr::variables::get<int>("hiscore")), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    );  //  End score overlay drawing.

    /* ********************************* */
    /* *** Player Info overlay entity ** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "player_info_overlay");
    mgr::assets<al_bitmap>::load<al_bitmap>("player_info_overlay", al_bitmap(200, 20, true));
    mgr::world::add_component<cmp::gfx::overlay>(e_id,
        mgr::assets<al_bitmap>::get<al_bitmap>("player_info_overlay"),
        mgr::assets<al_font>::get<al_font>("wte_default_font"), layer::overlay,
        config::gfx::arena_w - 200, config::gfx::arena_h - 20,
        [](const entity_id& ovr_id) {
            //  Define what gets displayed on the overlay.
            entity_id shd_id = mgr::world::get_id("shield");
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->set_drawing();
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            al_draw_filled_rectangle((float)(120 - mgr::world::get_component<energy>(shd_id)->amt), 0, 120, 10, WTE_COLOR_YELLOW);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
            mgr::world::set_component<cmp::gfx::overlay>(ovr_id)->draw_text("Lives:  " + std::to_string(mgr::variables::get<int>("lives")), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
        }
    );  //  End info overlay drawing.

    /* ********************************* */
    /* *** Game Over overlay entity **** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "game_over_overlay");
    mgr::assets<al_bitmap>::load<al_bitmap>("game_over_overlay", al_bitmap());
    mgr::assets<al_bitmap>::get<al_bitmap>("game_over_overlay")->load("game_over.bmp");
    mgr::world::add_component<cmp::gfx::overlay>(e_id,
        mgr::assets<al_bitmap>::get<al_bitmap>("game_over_overlay"),
        mgr::assets<al_font>::get<al_font>("wte_default_font"),
        layer::overlay, (config::gfx::arena_w / 2) - 240, (config::gfx::arena_h / 2) - 66,
        [](const entity_id& ovr_id) {}
    );
    mgr::world::set_component<cmp::gfx::overlay>(e_id)->visible = false;

    /* ********************************* */
    /* *** Player entity *************** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "player");
    mgr::world::add_component<cmp::location>(e_id,
        (config::gfx::arena_w / 2) - 5,
         config::gfx::arena_h - 40);
    mgr::world::add_component<cmp::hitbox>(e_id, 10, 10, 0);
    mgr::world::add_component<cmp::bounding_box>(e_id, 12.0f, 0.0f,
        (float)(config::gfx::arena_w - 21),
        (float)(config::gfx::arena_h - 32));
    mgr::world::add_component<health>(e_id, 1, 1);
    mgr::world::add_component<cmp::motion>(e_id, 0.0f, 0.0f, 0.0f);

    mgr::assets<al_bitmap>::load<al_bitmap>("ship", al_bitmap());
    mgr::assets<al_bitmap>::get<al_bitmap>("ship")->load("ship.bmp");
    mgr::world::add_component<cmp::gfx::sprite>(e_id, mgr::assets<al_bitmap>::get<al_bitmap>("ship"),
                                           layer::player, 32.0f, 32.0f, -11.0f, 0.0f, 1);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 3);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->add_cycle("death", 4, 7);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->set_cycle("main");

    //  Player logic.
    mgr::world::add_component<cmp::ai>(e_id,
        [](const entity_id& plr_id) {
            if(mgr::world::get_component<health>(plr_id)->hp <= 0) {  //  Check player health.
                mgr::world::set_component<cmp::ai>(plr_id)->enabled = false;
                mgr::world::set_component<health>(plr_id)->hp = mgr::world::get_component<health>(plr_id)->hp_max;
                std::string player_name = mgr::world::get_name(plr_id);
                mgr::messages::add(message("entities", player_name, player_name, "death", ""));
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
                mgr::world::set_component<cmp::gfx::sprite>(cannon_id)->visible = false;
                mgr::world::set_component<cmp::ai>(cannon_id)->enabled = false;
                mgr::world::set_component<cmp::hitbox>(cannon_id)->solid = false;
                mgr::audio::sample::stop("cannon_fire");

                //  Just to make sure... turn shield off
                entity_id shield_id = mgr::world::get_id("shield");
                mgr::world::set_component<cmp::gfx::sprite>(shield_id)->visible = false;
                mgr::world::set_component<cmp::ai>(shield_id)->enabled = false;
                mgr::world::set_component<cmp::hitbox>(shield_id)->solid = false;
                mgr::audio::sample::stop("shield_sound");

                mgr::world::set_component<cmp::hitbox>(plr_id)->solid = false;

                mgr::audio::sample::play(mgr::assets<al_sample>::get<al_sample>("megumin"), "once");
                mgr::variables::set<int>("lives", mgr::variables::get<int>("lives") - 1);
                mgr::world::set_component<cmp::motion>(plr_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(plr_id)->y_vel = 0.0f;
                mgr::world::set_component<cmp::gfx::sprite>(plr_id)->set_cycle("death");
                if(mgr::variables::get<int>("lives") == 0) {
                    //  Game over!
                    mgr::messages::add(message(engine_time::check() + 180, "system", "end-game", ""));
                    entity_id go_id = mgr::world::get_id("game_over_overlay");
                    mgr::world::set_component<cmp::gfx::overlay>(go_id)->visible = true;
                } else {
                    std::string player_name = mgr::world::get_name(plr_id);
                    mgr::messages::add(
                        message(engine_time::check() + 120, "entities", player_name, player_name, "reset", "")
                    );
                }
            }

            //  Reset player.
            if(msg.get_cmd() == "reset") {
                config::flags::input_enabled = true;
                player_pols::reset();
                mgr::world::set_component<cmp::motion>(plr_id)->x_vel = 0.0f;
                mgr::world::set_component<cmp::motion>(plr_id)->y_vel = 0.0f;
                mgr::world::set_component<cmp::location>(plr_id)->pos_x = (float)((config::gfx::arena_w / 2) - 5);
                mgr::world::set_component<cmp::location>(plr_id)->pos_y = (float)(config::gfx::arena_h - 40);
                mgr::world::set_component<health>(plr_id)->hp = mgr::world::get_component<health>(plr_id)->hp_max;
                mgr::world::set_component<cmp::ai>(plr_id)->enabled = true;
                mgr::world::set_component<cmp::gfx::sprite>(plr_id)->set_cycle("main");
                mgr::world::set_component<cmp::hitbox>(plr_id)->solid = true;
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
    mgr::world::add_component<cmp::location>(e_id, 0, 0);
    mgr::world::add_component<cmp::hitbox>(e_id, 10, 200, 0, false);
    mgr::world::set_component<cmp::hitbox>(e_id)->solid = false;
    mgr::world::add_component<damage>(e_id, 3);

    mgr::assets<al_bitmap>::load<al_bitmap>("cannon", al_bitmap());
    mgr::assets<al_bitmap>::get<al_bitmap>("cannon")->load("cannon.bmp");
    mgr::world::add_component<cmp::gfx::sprite>(e_id, mgr::assets<al_bitmap>::get<al_bitmap>("cannon"),
                                           layer::player, 10.0f, 200.0f, 0.0f, 0.0f, 2);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 3);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->set_cycle("main");
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->visible = false;

    //  Cannon logic.
    mgr::world::add_component<cmp::ai>(e_id,
        [](const entity_id& can_id) {
            entity_id player_entity = mgr::world::get_id("player");
            //  Set the cannon's location to match the player.
            mgr::world::set_component<cmp::location>(can_id)->pos_x =
                mgr::world::get_component<cmp::location>(player_entity)->pos_x;
            mgr::world::set_component<cmp::location>(can_id)->pos_y =
                mgr::world::get_component<cmp::location>(player_entity)->pos_y -
                mgr::world::get_component<cmp::hitbox>(can_id)->height;
        }
    );  //  End cannon logic.
    mgr::world::set_component<cmp::ai>(e_id)->enabled = false;

    //  Cannon message processing.
    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& can_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add(message("entities", msg.get_from(), msg.get_to(),
                    "damage", std::to_string(mgr::world::get_component<damage>(can_id)->dmg)));
            }
        }
    );  //  End cannon message processing.

    /* ********************************* */
    /* *** Shield entity *************** */
    /* ********************************* */
    e_id = mgr::world::new_entity();
    mgr::world::set_name(e_id, "shield");
    mgr::world::add_component<cmp::location>(e_id, 0, 0);
    mgr::world::add_component<cmp::hitbox>(e_id, 64, 64, 0, false);
    mgr::world::set_component<cmp::hitbox>(e_id)->solid = false;
    mgr::world::add_component<energy>(e_id, 50, 100);
    mgr::world::add_component<damage>(e_id, 100);

    mgr::assets<al_bitmap>::load<al_bitmap>("shield", al_bitmap());
    mgr::assets<al_bitmap>::get<al_bitmap>("shield")->load("shield.bmp");
    mgr::world::add_component<cmp::gfx::sprite>(e_id, mgr::assets<al_bitmap>::get<al_bitmap>("shield"),
                                           layer::player, 64.0f, 64.0f, 0.0f, 0.0f, 6);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 5);
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->set_cycle("main");
    mgr::world::set_component<cmp::gfx::sprite>(e_id)->visible = false;

    //  Shield logic.
    mgr::world::add_component<cmp::ai>(e_id,
        //  Enabeled AI.
        [](const entity_id& shd_id) {
            entity_id player_entity = mgr::world::get_id("player");
            //  Set the shield's location to match the player.
            mgr::world::set_component<cmp::location>(shd_id)->pos_x =
                mgr::world::get_component<cmp::location>(player_entity)->pos_x - 28.0f;
            mgr::world::set_component<cmp::location>(shd_id)->pos_y =
                mgr::world::get_component<cmp::location>(player_entity)->pos_y - 16.0f;

            //  Drain the shield.
            if(mgr::world::set_component<energy>(shd_id)->amt > 0)
                mgr::world::set_component<energy>(shd_id)->amt -= 1;

            if(mgr::world::get_component<energy>(shd_id)->amt <= 0) {
                //  Disable shield.
                mgr::world::set_component<cmp::gfx::sprite>(shd_id)->visible = false;
                mgr::world::set_component<cmp::ai>(shd_id)->enabled = false;
                mgr::world::set_component<cmp::hitbox>(player_entity)->solid = true;
                //  Stop sound effect.
                mgr::audio::sample::stop("shield_sound");
            }
        },
        //  Disabeled AI.
        [](const entity_id& shd_id) {
            //  Recharge the shield.
            if(mgr::world::set_component<energy>(shd_id)->amt < mgr::world::set_component<energy>(shd_id)->amt_max)
                mgr::world::set_component<energy>(shd_id)->amt += 1;
        }
    );  //  End shield logic.
    mgr::world::set_component<cmp::ai>(e_id)->enabled = false;

    //  Shield message processing.
    mgr::world::add_component<cmp::dispatcher>(e_id,
        [](const entity_id& shd_id, const message& msg) {
            if(msg.get_cmd() == "colision") {
                //  Deal damage
                mgr::messages::add(message("entities", msg.get_from(), msg.get_to(),
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
    mgr::assets<al_bitmap>::load<al_bitmap>("asteroid", al_bitmap());
    mgr::assets<al_bitmap>::get<al_bitmap>("asteroid")->load("asteroid.bmp");
    mgr::spawner::add("asteroid", 5,
        [](const entity_id& e_id, const msg_args& args) {
            int temp_size = std::stoi(args[5]);
            if(temp_size < 1) temp_size = 1;
            if(temp_size > 8) temp_size = 8;

            mgr::world::set_name(e_id, "asteroid" + std::to_string(e_id));
            mgr::world::add_component<cmp::location>(e_id, std::stof(args[1]), std::stof(args[2]));
            mgr::world::add_component<cmp::hitbox>(e_id, (float)(temp_size * 16), (float)(temp_size * 16), 1);
            mgr::world::add_component<health>(e_id, temp_size * 10, temp_size * 10);
            mgr::world::add_component<damage>(e_id, 10);
            mgr::world::add_component<size>(e_id, temp_size);
            mgr::world::add_component<cmp::motion>(e_id, 0.0f, 0.0f, 0.0f);
            mgr::world::set_component<cmp::motion>(e_id)->direction = std::stof(args[3]) * (M_PI / 180);
            mgr::world::set_component<cmp::motion>(e_id)->x_vel = std::stof(args[4]);
            mgr::world::set_component<cmp::motion>(e_id)->y_vel = std::stof(args[4]);

            mgr::world::add_component<cmp::gfx::sprite>(e_id, mgr::assets<al_bitmap>::get<al_bitmap>("asteroid"),
                layer::enemy, 16.0f, 16.0f, 0.0f, 0.0f, (int)(30 / std::stof(args[4])));
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 5);
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->set_cycle("main");
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->scale_factor_x = (float)temp_size;
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->scale_factor_y = (float)temp_size;
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->rotated = true;
            mgr::world::set_component<cmp::gfx::sprite>(e_id)->direction = std::stof(args[3]) * (M_PI / 180);

            //  Asteroid logic.
            mgr::world::add_component<cmp::ai>(e_id,
                [](const entity_id& ast_id) {
                    //  AI for asteroids defined here.
                    //  Perform OOB check.
                    if(mgr::world::get_component<cmp::location>(ast_id)->pos_y > (float)(config::gfx::arena_h + 100)) {
                        mgr::messages::add(message("spawner", "delete", mgr::world::get_name(ast_id)));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(mgr::world::get_component<health>(ast_id)->hp <= 0) {
                        mgr::messages::add(message("spawner", "delete", mgr::world::get_name(ast_id)));
                        mgr::audio::sample::play(mgr::assets<al_sample>::get<al_sample>("megumin"), "once", 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.8f);

                        mgr::variables::set("score",
                            (mgr::variables::get<int>("score") +
                            (10 * mgr::world::get_component<size>(ast_id)->the_size)));

                        //  If the asteroid was size >= 4, split into two.
                        if(mgr::world::get_component<size>(ast_id)->the_size >= 4) {
                            const int new_size = mgr::world::get_component<size>(ast_id)->the_size / 2;
                            float dir_a = mgr::world::get_component<cmp::motion>(ast_id)->direction - 90.0f;
                            if(dir_a < 0.0f) dir_a = 0.0f;
                            float dir_b = mgr::world::get_component<cmp::motion>(ast_id)->direction + 90.0f;
                            if(dir_b > 360.0f) dir_b = 360.0f;
                            const float new_x = mgr::world::get_component<cmp::location>(ast_id)->pos_x;
                            const float new_y = mgr::world::get_component<cmp::location>(ast_id)->pos_y;
                            const float new_vel = mgr::world::get_component<cmp::motion>(ast_id)->x_vel / 2;
                            std::string new_spawner_a = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_a) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            std::string new_spawner_b = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_b) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            mgr::messages::add(message("spawner", "new", new_spawner_a));
                            mgr::messages::add(message("spawner", "new", new_spawner_b));
                        }
                    }
                }
            );  //  End asteroid AI

            //  Asteroid message processing.
            mgr::world::add_component<cmp::dispatcher>(e_id,
                [](const entity_id& ast_id, const message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        mgr::messages::add(message("entities", msg.get_from(), msg.get_to(),
                            "damage", std::to_string(mgr::world::get_component<damage>(ast_id)->dmg)));
                    }

                    if(msg.get_cmd() == "damage") {
                        mgr::world::set_component<health>(ast_id)->hp -= std::stoi(msg.get_arg(0));
                    }
                }
            );  //  End asteroid message dispatching.
        }  //  End asteroid spawner function.
    );

    //  Load music for the demo into the asset manager.
    mgr::assets<al_audio>::load<al_audio>("music", al_audio("music.ogg"));
    //  Load some samples in the asset manager.
    mgr::assets<al_sample>::load<al_sample>("laser", al_sample("sfx/laser.wav"));
    mgr::assets<al_sample>::load<al_sample>("shield", al_sample("sfx/shield.wav"));
    mgr::assets<al_sample>::load<al_sample>("megumin", al_sample("sfx/megumin.wav"));

    //  Reset score.
    mgr::variables::set("score", 0);

    //  Set number of lives.
    if(mgr::variables::get<int>("max_lives") > 5 || mgr::variables::get<int>("max_lives") < 3)
        mgr::variables::set("max_lives", 3);
    mgr::variables::set("lives", mgr::variables::get<int>("max_lives"));

    mgr::audio::music::a::play(mgr::assets<al_audio>::get<al_audio>("music"));
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
    mgr::audio::music::a::pause();
    mgr::audio::ambiance::pause();
}

/*
 * On menu close.
 */
void wte_demo::on_menu_close(void) {
    mgr::audio::music::a::unpause();
    mgr::audio::ambiance::unpause();
}
