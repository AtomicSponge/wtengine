/*!
 * wtengine | File:  world.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/world.hpp"

namespace wte::mgr {

template <> bool manager<world>::initialized = false;

entity_id world::entity_counter = ENTITY_START;
entities world::entity_vec;
world_map world::_world;

/*
 *
 */
void world::clear(void) {
    entity_counter = ENTITY_START;
    entity_vec.clear();     //  Clear entities vector
    _world.clear();         //  Clear the world block
}

/*
 *
 */
entity_id world::new_entity(void) {
    entity_id next_id;

    if(entity_counter == ENTITY_MAX) {  //  Counter hit max.
        bool test = false;
        //  Look for the first available ID.
        for(next_id = ENTITY_START; !test; next_id++) {
            if(next_id == ENTITY_MAX) return ENTITY_ERROR;  //  No available ID, error.
            //  See if the new ID does not exist.
            test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                                 [&next_id](const entity& e){ return e.first == next_id; })
                    == entity_vec.end());
        }
    } else {  //  Counter not max, use the counter for entity ID.
        next_id = entity_counter;
        entity_counter++;
    }

    //  Set a new name.  Make sure name doesn't exist.
    std::string entity_name = "Entity" + std::to_string(next_id);
    bool test = false;
    for(entity_id temp_id = ENTITY_START; !test; temp_id++) {
        if(temp_id == ENTITY_MAX) return ENTITY_ERROR;  //  Couldn't name entity, error.
        //  See if the new name does not exist.
        test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&entity_name](const entity& e){ return e.second == entity_name; })
                == entity_vec.end());
        //  If it does, append the temp number and try that.
        if(!test) entity_name = "Entity" + std::to_string(next_id) + std::to_string(temp_id);
    }

    //  Tests complete, insert new entity.
    entity_vec.push_back(std::make_pair(next_id, entity_name));
    return next_id;  //  Return new entity ID.
}

/*
 *
 */
bool world::delete_entity(const entity_id& e_id) {
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it == entity_vec.end()) return false;

    _world.erase(e_id);      //  Remove all associated componenets.
    entity_vec.erase(e_it);  //  Delete the entity.

    return true;
}

/*
 *
 */
bool world::entity_exists(const entity_id& e_id) {
    const bool result = (std::find_if(entity_vec.begin(), entity_vec.end(),
        [&e_id](const entity& e){ return e.first == e_id; })
        != entity_vec.end());
    return result;
}

/*
 *
 */
const std::string world::get_name(const entity_id& e_id) {
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it == entity_vec.end()) {
        //  Not found, throw error.
        throw engine_exception(
            "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
    }
    return e_it->second;
}

/*
 *
 */
bool world::set_name(const entity_id& e_id, const std::string& name) {
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&name](const entity& e){ return e.second == name; });
    if(n_it != entity_vec.end()) return false;  //  Entity with the new name exists, error.

    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it == entity_vec.end()) return false;  //  Didn't find entity_id, error.

    e_it->second = name;
    return true;
}

/*
 *
 */
entity_id world::get_id(const std::string& name) {
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&name](const entity& e){ return e.second == name; });
    if(n_it == entity_vec.end()) return ENTITY_ERROR;
    return n_it->first;
}

/*
 *
 */
const entities world::get_entities(void) {
    entities temp_vec = entity_vec;
    return temp_vec;
}

/*
 *
 */
const entity_container world::set_entity(const entity_id& e_id) {
    if(!entity_exists(e_id)) {
        throw engine_exception(
            "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
    }

    entity_container temp_container;
    const auto results = _world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_sptr((*it).second));
    }
    return temp_container;
}

/*
 *
 */
const const_entity_container world::get_entity(const entity_id& e_id) {
    if(!entity_exists(e_id)) {
        throw engine_exception(
            "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
    }

    const_entity_container temp_container;
    const auto results = _world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_csptr((*it).second));
    }
    return temp_container;
}

}  //  end namespace wte::mgr
