/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  wte_demo.cpp
 *
 * See LICENSE.txt for copyright information
 *
 * Remaining members for wte_demo
 * Allows us to set custom behaviour for our game
 */

#include "include/wte_demo.hpp"

#include "include/custom_input.hpp"
#include "include/custom_spawner.hpp"
#include "include/stars.hpp"

using namespace wte;

/*
 * wte_demo
 */
wte_demo::wte_demo(int argc, char **argv) : wte_main(argc, argv, "WTE Demo") {
    game_cfg::reg("score=0");
    game_cfg::reg("hiscore=0");
    game_cfg::reg("lives=3");
    game_cfg_map::load();
}

/*
 * ~wte_demo
 */
wte_demo::~wte_demo() {
    game_cfg_map::clear_save();
    game_cfg_map::save("hiscore");
}

/*
 * Load menus.
 * Items will appear in the order they were added.
 */
void wte_demo::load_menus(void) {
    //  First configure the root main menu and game menu.
    {
        //  Configure main menu.
        menus.set_menu("main_menu")->set_title("WTE Demo");
        menus.set_menu("main_menu")->add_item(std::make_shared<mnu::menu_item_action>("New Game", "new_game"));
        menus.set_menu("main_menu")->add_item(std::make_shared<mnu::menu_item_action>("Settings", "open_menu", "settings"));
        menus.set_menu("main_menu")->add_item(std::make_shared<mnu::menu_item_action>("Exit Game", "exit"));
    }

    {
        //  Configure in-game menu.
        menus.set_menu("game_menu")->set_title("WTE Demo - Game Paused");
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("Resume Game", "close_menu", "all"));
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("Settings", "open_menu", "settings"));
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("End Game", "end_game"));
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("Exit Game", "exit"));
    }

    //  Then define other custom menus.
    {
        //  Create the settings menu.
        mnu::menu temp_menu = mnu::menu("settings", "Settings");
        temp_menu.add_item(std::make_shared<mnu::menu_item_action>("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    systems.add(std::make_unique<custom_input>());
    systems.add(std::make_unique<custom_spawner>());
    systems.add(std::make_unique<sys::colision>());
    systems.add(std::make_unique<sys::logic>());
    systems.add(std::make_unique<sys::animate>());
}

/*
 * Pre-load starting entities.
 */
void wte_demo::new_game(void) {
    entity e_id;

    /*
     * Background entity.
     */
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("starfield"));
    world.add_component(e_id, std::make_shared<cmp::visible>());
    world.add_component(e_id, std::make_shared<stars>());
    world.add_component(e_id, std::make_shared<cmp::background>(0,
        [](entity bkg_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define the animation process for the starfield.
            al_set_target_bitmap(world.get_component<cmp::background>(bkg_id)->background_bitmap);
            al_clear_to_color(WTE_COLOR_BLACK);

            //  Move the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                world.set_component<stars>(bkg_id)->y[i] +=
                    world.get_component<stars>(bkg_id)->speed[i] * world.get_component<stars>(bkg_id)->speed_mult;
                if(world.get_component<stars>(bkg_id)->y[i] > engine_cfg::get<int>("screen_height")) { //  Make a new star
                    world.set_component<stars>(bkg_id)->x[i] = std::rand() % engine_cfg::get<int>("screen_width") + 1;
                    world.set_component<stars>(bkg_id)->y[i] = 0;
                    world.set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                    world.set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                }
            }

            //  Draw the stars.
            for(std::size_t i = 0; i < MAX_STARS; i++) {
                if(world.get_component<stars>(bkg_id)->color[i] == 1 || 4)
                    al_draw_pixel(world.get_component<stars>(bkg_id)->x[i], world.get_component<stars>(bkg_id)->y[i], WTE_COLOR_WHITE);
                if(world.get_component<stars>(bkg_id)->color[i] == 2)
                    al_draw_pixel(world.get_component<stars>(bkg_id)->x[i], world.get_component<stars>(bkg_id)->y[i], WTE_COLOR_YELLOW);
                if(world.get_component<stars>(bkg_id)->color[i] == 3)
                    al_draw_pixel(world.get_component<stars>(bkg_id)->x[i], world.get_component<stars>(bkg_id)->y[i], WTE_COLOR_RED);
            }
        }
    ));
    world.add_component(e_id, std::make_shared<cmp::dispatcher>(
        [](entity bkg_id, mgr::entity_manager& world, message msg) {
            //  Define message processing for the starfield.
            if(msg.get_cmd() == "default") world.set_component<stars>(bkg_id)->speed_mult = 1;
            if(msg.get_cmd() == "up") world.set_component<stars>(bkg_id)->speed_mult *= 2;
            if(msg.get_cmd() == "down") world.set_component<stars>(bkg_id)->speed_mult /= 2;
            if(msg.get_cmd() == "reset") {
                world.set_component<stars>(bkg_id)->speed_mult = 1;

                for(std::size_t i = 0; i < MAX_STARS; i++) {
                    world.set_component<stars>(bkg_id)->x[i] = std::rand() % engine_cfg::get<int>("screen_width") + 1;
                    world.set_component<stars>(bkg_id)->y[i] = std::rand() % engine_cfg::get<int>("screen_height") + 1;
                    world.set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                    world.set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                }
            }
        }
    ));

    /*
     * Overlay entity.
     */
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("overlay"));
    world.add_component(e_id, std::make_shared<cmp::visible>());
    world.add_component(e_id, std::make_shared<cmp::overlay>(300, 20, 0, engine_cfg::get<int>("screen_height") - 20, 0,
        [](entity ovr_id, mgr::entity_manager& world, int64_t engine_time) {
            //  Define what gets displayed on the overlay.
            al_set_target_bitmap(world.get_component<cmp::overlay>(ovr_id)->overlay_bitmap);
            al_clear_to_color(WTE_COLOR_TRANSPARENT);
            world.set_component<cmp::overlay>(ovr_id)->set_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
            world.set_component<cmp::overlay>(ovr_id)->set_text(game_cfg::get("score"), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
            world.set_component<cmp::overlay>(ovr_id)->set_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
            world.set_component<cmp::overlay>(ovr_id)->set_text(game_cfg::get("hiscore"), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
        }
    ));
    world.set_component<cmp::overlay>(e_id)->set_font(al_create_builtin_font());

    /*
     * Player entity.
     */
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("player"));
    world.add_component(e_id, std::make_shared<cmp::team>(0));
    world.add_component(e_id, std::make_shared<cmp::location>((engine_cfg::get<int>("screen_width") / 2) - 5,
                                                               engine_cfg::get<int>("screen_height") - 40));
    world.add_component(e_id, std::make_shared<cmp::hitbox>(10, 10));
    world.add_component(e_id, std::make_shared<cmp::health>(1));
    world.add_component(e_id, std::make_shared<cmp::render_order>(1));
    world.add_component(e_id, std::make_shared<cmp::input_handler>());
    world.add_component(e_id, std::make_shared<cmp::visible>());
    world.add_component(e_id, std::make_shared<cmp::enabled>());
    world.add_component(e_id, std::make_shared<cmp::dispatcher>(
        [](entity plr_id, mgr::entity_manager& world, message msg) {
            //  Define message processing for the player.
            if(msg.get_cmd() == "colision") game_cfg_map::subtract<int>("lives", 1);
        }
    ));
    world.add_component(e_id, std::make_shared<cmp::sprite>(32, 32, -11.0, 0.0, 1, 1));
    world.set_component<cmp::sprite>(e_id)->load_sprite("ship.bmp", ".bmp");
    world.set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 3);
    world.set_component<cmp::sprite>(e_id)->set_cycle("main");

    /*
     * Main cannon entity.
     */
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("main_cannon"));
    world.add_component(e_id, std::make_shared<cmp::team>(0));
    world.add_component(e_id, std::make_shared<cmp::location>(0, 0));
    world.add_component(e_id, std::make_shared<cmp::hitbox>(10, 200, false));
    world.add_component(e_id, std::make_shared<cmp::damage>(5));
    world.add_component(e_id, std::make_shared<cmp::render_order>(2));
    world.add_component(e_id, std::make_shared<cmp::input_handler>());
    world.add_component(e_id, std::make_shared<cmp::visible>(false));
    world.add_component(e_id, std::make_shared<cmp::enabled>(false));
    world.add_component(e_id, std::make_shared<cmp::sprite>(10, 200, 0.0, 0.0, 1, 2));
    world.set_component<cmp::sprite>(e_id)->load_sprite("cannon.bmp", ".bmp");
    world.set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 3);
    world.set_component<cmp::sprite>(e_id)->set_cycle("main");

    /*
     * Shield entity.
     */
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("shield"));
    world.add_component(e_id, std::make_shared<cmp::team>(0));
    world.add_component(e_id, std::make_shared<cmp::location>(0, 0));
    world.add_component(e_id, std::make_shared<cmp::hitbox>(64, 64, false));
    world.add_component(e_id, std::make_shared<cmp::damage>(100));
    world.add_component(e_id, std::make_shared<cmp::render_order>(2));
    world.add_component(e_id, std::make_shared<cmp::input_handler>());
    world.add_component(e_id, std::make_shared<cmp::visible>(false));
    world.add_component(e_id, std::make_shared<cmp::enabled>(false));
    world.add_component(e_id, std::make_shared<cmp::sprite>(64, 64, 0.0, 0.0, 1, 2));
    world.set_component<cmp::sprite>(e_id)->load_sprite("shield.bmp", ".bmp");
    world.set_component<cmp::sprite>(e_id)->add_cycle("main", 0, 3);
    world.set_component<cmp::sprite>(e_id)->set_cycle("main");

    game_cfg::set("score=0");
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
    messages.add_message(message("audio", "null", "null"));
}

/*
 * On menu close.
 */
void wte_demo::on_menu_close(void) {
    messages.add_message(message("audio", "null", "null"));
}
