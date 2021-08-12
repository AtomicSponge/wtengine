/*!
 * WTEngine | File:  world.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_WORLD_HPP
#define WTE_MGR_WORLD_HPP

#define WTE_ENTITY_ERROR (0)
#define WTE_ENTITY_START (1)
#define WTE_ENTITY_MAX (std::numeric_limits<entity_id>::max())

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <memory>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/cmp/component.hpp"

namespace wte
{

class engine;

/*
 * Define containers for entity/component/world storage.
 */
/*!
 * \typedef std::size_t entity_id
 * Container to store an entity id.
 */
typedef std::size_t entity_id;

/*!
 * \typedef std::pair<entity_id, std::string> entity
 * Container to store an entity reference.
 */
typedef std::pair<entity_id, std::string> entity;

/*!
 * \typedef std::vector<entity> world_container
 * Container for storing a group of entity references.
 */
typedef std::vector<entity> world_container;

/*!
 * \typedef std::vector<cmp::component_sptr> entity_container
 * Container to store a group of components related to an entity.
 */
typedef std::vector<cmp::component_sptr> entity_container;

/*!
 * \typedef std::vector<cmp::component_csptr> const_entity_container
 * Constant container to store a group of components related to an entity.
 */
typedef std::vector<cmp::component_csptr> const_entity_container;

/*!
 * Container for storing components of similar type.
 * \tparam Component type
 */
template <typename T>
using component_container = std::map<const entity_id, std::shared_ptr<T>>;

/*!
 * Constant container for storing components of similar type.
 * \tparam Component type
 */
template <typename T>
using const_component_container = std::map<const entity_id, std::shared_ptr<const T>>;

/*!
 * \typedef std::unordered_multimap<entity_id, cmp::component_sptr> world_map
 * Container to store the entire game world.
 */
typedef std::unordered_multimap<entity_id, cmp::component_sptr> world_map;

namespace mgr
{

/*!
 * \class world
 * \brief Store a collection of entities and their corresponding components in memory.
 */
class world final : private manager<world> {
    friend class wte::engine;

    public:
        /*!
         * \brief Create a new entity by name, using the next available ID.
         * 
         * Throw error if there is no room for entities.
         * 
         * \return The newly created entity ID.  WTE_ENTITY_ERROR on fail.
         */
        static const entity_id new_entity(void);

        /*!
         * \brief Delete entity by ID.
         * 
         * \param e_id The entity ID to delete.
         * \return Return true on success, false if entity does not exist.
         */
        static const bool delete_entity(const entity_id& e_id);

        /*!
         * \brief Check if an entity exists.
         * 
         * Check the entity vector by ID and return result.
         * 
         * \param e_id The entity ID to check.
         * \return Return true if found, return false if not found.
         */
        static const bool entity_exists(const entity_id& e_id);

        /*!
         * \brief Get entity name.
         * 
         * \param e_id Entity ID to get name for.
         * \return Entity name string.
         * \exception wte_exception Entity does not exist.
         */
        static const std::string get_name(const entity_id& e_id);

        /*!
         * \brief Set the entity name.
         * 
         * \param e_id Entity ID to set.
         * \param name Entity name to set.
         * \return True if set, false on error.
         */
        static const bool set_name(
            const entity_id& e_id,
            const std::string& name
        );

        /*!
         * \brief Get entity ID by name.
         * 
         * \param name Name to search.
         * \return Entity ID, WTE_ENTITY_ERROR if not found.
         */
        static const entity_id get_id(const std::string& name);

        /*!
         * \brief Get the entity reference vector.
         * 
         * \return Returns a vector of all entity IDs and names.
         */
        static const world_container get_entities(void);

        /*!
         * \brief Set all components related to an entity.
         * 
         * \param e_id Entity ID to set components for.
         * \return Returns a container of components based by entity ID.
         * \exception wte_exception Entity does not exist.
         */
        static const entity_container set_entity(const entity_id& e_id);

        /*!
         * \brief Get all components related to an entity.
         * 
         * \param e_id Entity ID to get components for.
         * \return Returns a constant container of components based by entity ID.
         * \exception wte_exception Entity does not exist.
         */
        static const const_entity_container get_entity(const entity_id& e_id);

        /*!
         * \brief Add a new component to an entity.
         * 
         * Entities can only have a single compoenent of each type.
         * 
         * \tparam T Component type to add.
         * \param e_id Entity ID to add a component to.
         * \param args List of parameters to pass to component constructor.
         * \return Return false if the entity does not exist.
         * \return Return false if the entity already has a component of the same type.
         * \return Return true on success.
         */
        template <typename T, typename... Args>
        inline static const bool add_component(
            const entity_id& e_id,
            Args... args
        ) {
            if(!entity_exists(e_id)) return false;

            //  Check derived types of existing components, make sure one does not already exist.
            const auto check_entity = get_entity(e_id);
            for(auto& it: check_entity) {
                if(typeid(*it).name() == typeid(T).name()) return false;
            }

            _world.insert(std::make_pair(e_id, std::make_shared<T>(args...)));
            return true;
        };

        /*!
         * \brief Delete a component by type for an entity.
         * 
         * \tparam T Component type to delete.
         * \param e_id Entity ID to delete component from.
         * \return Return true if a component was deleted.
         * \return Return false if no components were deleted.
         */
        template <typename T>
        inline static const bool delete_component(
            const entity_id& e_id
        ) {
            auto results = _world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second)) {
                    it = _world.erase(it);
                    return true;
                }
            }
            return false;
        };

        /*!
         * \brief Check if an entity has a component by type.
         * 
         * \tparam T Component type to check.
         * \param e_id The entity ID to check.
         * \return Return true if the entity has the component.
         * \return Return false if it does not.
         */
        template <typename T>
        inline static const bool has_component(
            const entity_id& e_id
        ) {
            const auto results = _world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second)) return true;
            }
            return false;
        };

        /*!
         * \brief Set the value of a component by type for an entity.
         * 
         * \tparam T Component type to search.
         * \param e_id The entity ID to search.
         * \return Return the component.
         * \exception wte_exception Component not found.
         */
        template <typename T>
        inline static const std::shared_ptr<T> set_component(
            const entity_id& e_id
        ) {
            const auto results = _world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<T>(it->second);
            }
            std::string err_str = "Entity: " + std::to_string(e_id) + " - Component not found";
            throw wte_exception(err_str.c_str(), "world", engine_time::check_time());
        };

        /*!
         * \brief Read the value of a component by type for an entity.
         * 
         * \tparam T Component type to search.
         * \param e_id The entity ID to search.
         * \return Return the component.
         * \exception wte_exception Component not found.
         */
        template <typename T>
        inline static const std::shared_ptr<const T> get_component(
            const entity_id& e_id
        ) {
            const auto results = _world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<const T>(it->second);
            }
            std::string err_str = "Entity: " + std::to_string(e_id) + " - Component not found";
            throw wte_exception(err_str.c_str(), "world", engine_time::check_time());
        };

        /*!
         * \brief Set all components for a particulair type.
         * 
         * This will return a container of modifiable components casted to their type.
         * 
         * \tparam T Component type to search.
         * \return Returns a container of components of all the same type.
         */
        template <typename T>
        inline static const component_container<T> set_components(void) {
            component_container<T> temp_components;

            for(auto& it: _world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }
            return temp_components;
        };

        /*!
         * \brief Get all components for a particulair type.
         * 
         * This will return a container of non-modifiable components casted to their type.
         * 
         * \tparam T Component type to search.
         * \return Returns a constant container of components of all the same type.
         */
        template <typename T>
        inline static const const_component_container<T> get_components(void) {
            const_component_container<T> temp_components;

            for(auto& it: _world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }
            return temp_components;
        };

    private:
        world() = default;
        ~world() = default;

        /*
         * Clear the entity manager.
         */
        static void clear(void);

        static entity_id entity_counter;    //  Last Entity ID used.
        static world_container entity_vec;  //  Container for all entities.
        static world_map _world;            //  Container for all components.
};

} //  namespace mgr

} //  namespace wte

#endif
