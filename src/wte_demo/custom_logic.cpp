/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_logic.cpp

  See LICENSE.txt for copyright information

  Custom logic code
  This overrides the logic system to define custom behavior
*/

#define _USE_MATH_DEFINES

#include <cmath>

#include "include/custom_logic.hpp"

/*
  Custom logic run member
*/
void custom_logic::custom_run(wte::mgr::entity_manager& world, wte::mgr::message_manager& messages, int64_t current_time) {
    //  Move all entities tagged with an AI component
    for(wte::component_iterator it = ai_components.begin(); it != ai_components.end(); it++) {
        world.set_component<wte::cmp::location>(it->first)->pos_x +=
            world.get_component<wte::cmp::velocity>(it->first)->speed *
            cos(world.get_component<wte::cmp::direction>(it->first)->angle * (M_PI / 180));

        world.set_component<wte::cmp::location>(it->first)->pos_y +=
            world.get_component<wte::cmp::velocity>(it->first)->speed *
            sin(world.get_component<wte::cmp::direction>(it->first)->angle * (M_PI / 180));

        //  OOB check
        if(world.get_component<wte::cmp::location>(it->first)->pos_y > wte::screen_height + 100) {
            messages.add_message(wte::msg::message("spawner", "delete",
                                 world.get_component<wte::cmp::name>(it->first)->name_str));
        }
    }

    //  Perform health check
    wte::component_container health_components = world.get_components<wte::cmp::health>();
    for(wte::component_iterator it = health_components.begin(); it != health_components.end(); it++) {
        if(dynamic_cast<wte::cmp::health*>(it->second.get())->hp < 1) {
            if(world.get_component<wte::cmp::name>(it->first)->name_str == "player") {
                //  Handle player death
            } else {
                //  Everything else
                messages.add_message(wte::msg::message("spawner", "delete",
                                     world.get_component<wte::cmp::name>(it->first)->name_str));
            }
        }
    }
}

/*
  Custom logic process_messages member
*/
void custom_logic::process_message(wte::mgr::entity_manager& world, wte::msg::message message) {
    wte::component_container named_components;
    named_components = world.get_components<wte::cmp::name>();

    //  Process colision messages
    if(message.get_cmd() == "colision") {

        //  Main cannon is hitting something
        if(message.get_from() == "main_cannon") {
            //  Get the entity ID of the object being hit
            for(wte::component_iterator c_it = named_components.begin(); c_it != named_components.end(); c_it++) {
                if(message.get_to() == dynamic_cast<wte::cmp::name*>(c_it->second.get())->name_str) {
                    //  If to:entity has a health componenet, take damage
                    if(world.has_component<wte::cmp::health>(c_it->first)) {
                        world.set_component<wte::cmp::health>(c_it->first)->hp -= 1;
                    }
                }
            }
        }

        //  Shield is hitting something
        if(message.get_from() == "shield") {
            //  Get the entity ID of the object being hit
            for(wte::component_iterator c_it = named_components.begin(); c_it != named_components.end(); c_it++) {
                if(message.get_to() == dynamic_cast<wte::cmp::name*>(c_it->second.get())->name_str) {
                    //  If to:entity has a health componenet, take damage
                    if(world.has_component<wte::cmp::health>(c_it->first)) {
                        world.set_component<wte::cmp::health>(c_it->first)->hp -= 10;
                    }
                }
            }
        }

        //  Player got hit, take damage
        if(message.get_to() == "player") {
            //
        }
    } //  end colision messages
}
