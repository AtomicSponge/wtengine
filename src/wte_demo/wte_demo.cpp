/*
  WTEngine Demo
  By:  Matthew Evans
  File:  wte_demo.cpp

  See LICENSE.txt for copyright information

  Remaining members for wte_demo
  Allows us to set custom behaviour for our game
*/

#include "include/wte_demo.hpp"

#include "include/custom_input.hpp"
#include "include/custom_spawner.hpp"
#include "include/custom_logic.hpp"
#include "include/starfield.hpp"

using namespace wte;

/*!
 * wte_demo
 */
wte_demo::wte_demo() {
    game_cfg::reg("score=0");
    game_cfg::reg("hiscore=0");
    game_cfg::reg("lives=3");
    game_cfg_map::load();
}

/*!
 * ~wte_demo
 */
wte_demo::~wte_demo() {
    game_cfg_map::clear_save();
    game_cfg_map::save("hiscore");
}

/*
  Load menus
*/
void wte_demo::load_menus(void) {
    {
        //  Configure main menu
        menus.set_menu("main_menu")->set_title("WTE Demo");
        menus.set_menu("main_menu")->add_item(std::make_shared<mnu::menu_item_action>("New Game", "new_game"));
        menus.set_menu("main_menu")->add_item(std::make_shared<mnu::menu_item_action>("Exit Game", "exit"));
    }

    {
        //  Configure in-game menu
        menus.set_menu("game_menu")->set_title("WTE Demo - Game Paused");
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("Resume Game", "close_menu", "all"));
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("End Game", "end_game"));
        menus.set_menu("game_menu")->add_item(std::make_shared<mnu::menu_item_action>("Exit Game", "exit"));
    }

    {
        //  Create the settings menu
        mnu::menu temp_menu = mnu::menu("settings", "Settings");
        temp_menu.add_item(std::make_shared<mnu::menu_item_action>("Return", "close_menu"));
        if(!menus.new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }
}

/*
  Load the systems in order
*/
void wte_demo::load_systems(void) {
    systems.add(std::make_unique<custom_input>());
    systems.add(std::make_unique<custom_spawner>());
    systems.add(std::make_unique<custom_logic>());
    systems.add(std::make_unique<sys::colision>());
    systems.add(std::make_unique<sys::animate>());
    systems.add(std::make_unique<starfield>());
}

/*
  Pre-load starting entities
*/
void wte_demo::load_game(void) {
    entity e_id;

    //  Background entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("starfield"));
    world.add_component(e_id, std::make_shared<cmp::background>());
    world.add_component(e_id, std::make_shared<cmp::background_layer>(0));
    world.add_component(e_id, std::make_shared<cmp::visible>());

    //  Player entity
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

    //  Main cannon entity
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

    //  Shield entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("shield"));
    world.add_component(e_id, std::make_shared<cmp::team>(0));
    world.add_component(e_id, std::make_shared<cmp::location>(0, 0));
    world.add_component(e_id, std::make_shared<cmp::hitbox>(40, 40, false));
    world.add_component(e_id, std::make_shared<cmp::damage>(100));
    world.add_component(e_id, std::make_shared<cmp::render_order>(2));
    world.add_component(e_id, std::make_shared<cmp::input_handler>());
    world.add_component(e_id, std::make_shared<cmp::visible>(false));
    world.add_component(e_id, std::make_shared<cmp::enabled>(false));

    game_cfg::set("score=0");
}

/*
  Stop the game
*/
void wte_demo::end_game(void) {
    if(game_cfg::get<int>("score") > game_cfg::get<int>("hiscore"))
        game_cfg::set("hiscore", game_cfg::get("score"));
    //  test code
    game_cfg::set("hiscore=0");
    game_cfg_map::add<int>("hiscore", 20);
    game_cfg_map::divide<int>("hiscore", 2);
    game_cfg_map::multiply<int>("hiscore", 4);
    game_cfg_map::subtract<int>("hiscore", 3);
    //  end test code
}
