/*!
 * WTEngine | File:  entity_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>

#include "wtengine/mgr/entity_manager.hpp"

namespace wte
{

namespace mgr
{

template <> bool manager<entity_manager>::initialized = false;

entity_manager::entity_manager() : entity_counter(WTE_ENTITY_START) {
    entity_vec.clear();
    world.clear();
}

entity_manager::~entity_manager() {
    entity_vec.clear();
    world.clear();
}

void entity_manager::clear(void) {
    entity_counter = WTE_ENTITY_START;
    entity_vec.clear();
    world.clear();
}

const entity_id entity_manager::new_entity(void) {
    entity_id next_id;

    if(entity_counter == WTE_ENTITY_MAX) {  //  Counter hit max.
        bool test = false;
        //  Look for the first available ID.
        for(next_id = WTE_ENTITY_START; !test; next_id++) {
            if(next_id == WTE_ENTITY_MAX) return WTE_ENTITY_ERROR;  //  No available ID, error.
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
    for(entity_id temp_id = WTE_ENTITY_START; !test; temp_id++) {
        if(temp_id == WTE_ENTITY_MAX) return WTE_ENTITY_ERROR;  //  Couldn't name entity, error.
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

const bool entity_manager::delete_entity(const entity_id& e_id) {
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it != entity_vec.end()) {
        world.erase(e_id);     //  Remove all associated componenets.
        entity_vec.erase(e_it);  //  Delete the entity.
        return true;
    }
    return false;
}

const bool entity_manager::entity_exists(const entity_id& e_id) const {
    return (std::find_if(entity_vec.begin(), entity_vec.end(),
                            [&e_id](const entity& e){ return e.first == e_id; })
            != entity_vec.end());
}

const std::string entity_manager::get_name(const entity_id& e_id) const {
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it != entity_vec.end()) return e_it->second;
    return "";
}

const bool entity_manager::set_name(const entity_id& e_id, const std::string& name) {
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                [&name](const entity& e){ return e.second == name; });
    if(n_it != entity_vec.end())
        return false;  //  Entity with the new name exists, error.
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it != entity_vec.end()) {
        e_it->second = name;
        return true;  //  New name set.
    }
    return false;  //  Didn't find entity_id, error.
}

const entity_id entity_manager::get_id(const std::string& name) const {
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                                [&name](const entity& e){ return e.second == name; });
    if(n_it != entity_vec.end())
        return n_it->first;
    return WTE_ENTITY_ERROR;
}

const world_container entity_manager::get_entities(void) const {
    return entity_vec;
}

const entity_container entity_manager::set_entity(const entity_id& e_id) {
    if(!entity_exists(e_id)) return {};

    entity_container temp_container;
    const auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_sptr((*it).second));
    }
    return temp_container;
}

const const_entity_container entity_manager::get_entity(const entity_id& e_id) const {
    if(!entity_exists(e_id)) return {};

    const_entity_container temp_container;
    const auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_csptr((*it).second));
    }
    return temp_container;
}

const bool entity_manager::add_component(const entity_id& e_id, const cmp::component_sptr& comp) {
    if(!entity_exists(e_id)) return false;

    //  Check derived types of existing components, make sure one does not already exist.
    const const_entity_container check_entity = get_entity(e_id);
    for(auto & it : check_entity) {
        if(typeid(*it).name() == typeid(*comp).name()) return false;
    }

    world.insert(std::make_pair(e_id, comp));
    return true;
}

template <typename T> const bool entity_manager::delete_component(const entity_id& e_id) {
    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second)) {
            it = world.erase(it);
            return true;
        }
    }
    return false;
}

template <typename T> const bool entity_manager::has_component(const entity_id& e_id) const {
    const auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second)) return true;
    }
    return false;
}

template <typename T> const std::shared_ptr<T> entity_manager::set_component(const entity_id& e_id) {
    const auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second))
            return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template <typename T> const std::shared_ptr<const T> entity_manager::get_component(const entity_id& e_id) const {
    const auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second))
            return std::static_pointer_cast<const T>(it->second);
    }
    return nullptr;
}

template <typename T> const component_container<T> entity_manager::set_components(void) {
    component_container<T> temp_components;

    for(auto & it : world) {
        if(std::dynamic_pointer_cast<T>(it.second))
            temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
    }
    return temp_components;
}

template <typename T> const const_component_container<T> entity_manager::get_components(void) const {
    const_component_container<T> temp_components;

    for(auto & it : world) {
        if(std::dynamic_pointer_cast<T>(it.second))
            temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
    }
    return temp_components;
}

} //  namespace mgr

} //  namespace wte
