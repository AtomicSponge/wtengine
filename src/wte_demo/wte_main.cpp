/*
  WTEngine Demo
  By:  Matthew Evans
  File:  wte_main.cpp

  See LICENSE.txt for copyright information

  Remaining members for wte_main
  Allows us to set custom behaviour for our game
*/

#include "include/wte/wte.hpp"

#include "include/custom_input.hpp"
#include "include/custom_spawner.hpp"
#include "include/custom_logic.hpp"
#include "include/starfield.hpp"

using namespace wte;

/*
  Load menus
*/
void wte_main::load_menus(void) {
    menus.set_menu("main_menu")->set_title("WTE Demo");
    menus.set_menu("game_menu")->set_title("WTE Demo - Game Paused");
    //mnu::menu_item_action item;
    //menus.set_menu("main_menu")->add_item(item);
}

/*
  Load the systems in order
*/
void wte_main::load_systems(void) {
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
void wte_main::load_game(void) {
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
}

/*
  Stop the game
*/
void wte_main::end_game(void) {
    //
}

/*
  Process custom system messages
*/
void wte_main::handle_custom_sys_msg(message message) {
    //
}
