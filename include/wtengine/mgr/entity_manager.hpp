/*!
 * WTEngine | File:  entity_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_ENTITY_MANAGER_HPP
#define WTE_MGR_ENTITY_MANAGER_HPP

#define WTE_ENTITY_ERROR (0)
#define WTE_ENTITY_START (1)
#define WTE_ENTITY_MAX (std::numeric_limits<entity_id>::max())

#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/cmp/component.hpp"

namespace wte
{

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
 * \class entity_manager
 * \brief Store a collection of entities and their corresponding components in memory.
 */
class entity_manager final : private manager<entity_manager> {
    public:
        /*!
         * \brief Entity manager constructor.
         * 
         * Start entity counter at 1
         */
        entity_manager();

        /*!
         * \brief Entity manager destructor.
         */
        ~entity_manager();

        /*!
         * \brief Clear the entity manager.
         * 
         * Set the entity counter to zero and clear the entities and componenets.
         */
        void clear(void);

        /*!
         * \brief Create a new entity by name, using the next available ID.
         * 
         * Throw error if there is no room for entities.
         * 
         * \return The newly created entity ID.  WTE_ENTITY_ERROR on fail.
         */
        const entity_id new_entity(void);

        /*!
         * \brief Delete entity by ID.
         * 
         * \param e_id The entity ID to delete.
         * \return Return true on success, false if entity does not exist.
         */
        const bool delete_entity(const entity_id& e_id);

        /*!
         * \brief Check if an entity exists.
         * 
         * Check the entity vector by ID and return result.
         * 
         * \param e_id The entity ID to check.
         * \return Return true if found, return false if not found.
         */
        const bool entity_exists(const entity_id& e_id) const;

        /*!
         * \brief Get entity name.
         * 
         * \param e_id Entity ID to get name for.
         * \return Entity name string.  Empty string if not found.
         */
        const std::string get_name(const entity_id& e_id) const;

        /*!
         * \brief Set the entity name.
         * 
         * \param e_id Entity ID to set.
         * \param name Entity name to set.
         * \return True if set, false on error.
         */
        const bool set_name(const entity_id& e_id, const std::string& name);

        /*!
         * \brief Get entity ID by name.
         * 
         * \param name Name to search.
         * \return Entity ID, WTE_ENTITY_ERROR if not found.
         */
        const entity_id get_id(const std::string& name) const;

        /*!
         * \brief Get the entity reference vector.
         * 
         * \return Returns a vector of all entity IDs and names.
         */
        const world_container get_entities(void) const;

        /*!
         * \brief Set all components related to an entity.
         * 
         * \param e_id Entity ID to set components for.
         * \return Returns a container of components based by entity ID.
         */
        const entity_container set_entity(const entity_id& e_id);

        /*!
         * \brief Get all components related to an entity.
         * 
         * \param e_id Entity ID to get components for.
         * \return Returns a constant container of components based by entity ID.
         */
        const const_entity_container get_entity(const entity_id& e_id) const;

        /*!
         * \brief Add a new component to an entity.
         * 
         * Entities can only have a single compoenent of each type.
         * 
         * \param e_id Entity ID to add a component to.
         * \param comp Componenet to add.
         * \return Return false if the entity does not exist.
         * \return Return false if the entity already has a component of the same type.
         * \return Return true on success.
         */
        const bool add_component(const entity_id& e_id, const cmp::component_sptr& comp);

        /*!
         * \brief Delete a component by type for an entity.
         * 
         * \tparam T Component type.
         * \param e_id Entity ID to delete component from.
         * \return Return true if a component was deleted.
         * \return Return false if no components were deleted.
         */
        template <typename T> const bool delete_component(const entity_id& e_id);

        /*!
         * \brief Check if an entity has a component by type.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to check.
         * \return Return true if the entity has the component.
         * \return Return false if it does not.
         */
        template <typename T> const bool has_component(const entity_id& e_id) const;

        /*!
         * \brief Set the value of a component by type for an entity.
         * 
         * Note that calling this for a component that does not exist will cause the
         * program to fail.  If there is a chance it does not exist, call has_component() first.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to search.
         * \return Return the component or nullptr if not found.
         */
        template <typename T> const std::shared_ptr<T> set_component(const entity_id& e_id);

        /*!
         * \brief Read the value of a component by type for an entity.
         * 
         * Note that calling this for a component that does not exist will cause the
         * program to fail.  If there is a chance it does not exist, call has_component() first.
         * 
         * \tparam T Component type.
         * \param e_id The entity ID to search.
         * \return Return the component or nullptr if not found.
         */
        template <typename T> const std::shared_ptr<const T> get_component(const entity_id& e_id) const;

        /*!
         * \brief Set all components for a particulair type.
         * 
         * This will return a container of modifiable components casted to their type.
         * 
         * \tparam T Component type.
         * \return Returns a container of components of all the same type.
         */
        template <typename T> const component_container<T> set_components(void);

        /*!
         * \brief Get all components for a particulair type.
         * 
         * This will return a container of non-modifiable components casted to their type.
         * 
         * \tparam T Component type.
         * \return Returns a constant container of components of all the same type.
         */
        template <typename T> const const_component_container<T> get_components(void) const;

    private:
        entity_id entity_counter;
        world_container entity_vec;
        world_map world;
};

template <> bool manager<entity_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
