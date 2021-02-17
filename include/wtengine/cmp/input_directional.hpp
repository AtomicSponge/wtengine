/*!
 * WTEngine | File:  input_directional.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_INPUT_DIRECTIONAL_HPP
#define WTE_CMP_INPUT_DIRECTIONAL_HPP

#include <functional>
#include <cassert>

#include "wtengine/component.hpp"
#include "wtengine/../mgr/entity_manager.hpp"
#include "wtengine/../mgr/message_manager.hpp"
#include "wtengine/../_globals/input_flags.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class input_directional
 * \brief Creates an input bind to a directional joystick.
 * 
 * Tag an entity to be processed by the Input system.
 */
class input_directional final : public component {
    public:
        /*!
         * \brief Input Directional constructor.
         */
        input_directional(
            const std::size_t ib,
            void func_a(const entity_id&,
                        const float&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&),
            void func_b(const entity_id&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&)
        ) : input_bind(ib), direction_set(func_a), direction_unset(func_b) {
            assert(input_bind < WTE_MAX_JOYSTICK_FLAGS);
        };

        /*!
         * \brief Input Directional destructor.
         */
        ~input_directional() {};

        /*!
         * \brief Get the joystick bounded to this component.
         * 
         * \return The joystick enum value.
         */
        const std::size_t get_bind(void) const {
            return input_bind;
        };

        /*!
         * \brief Run the joystick direction set function.
         * 
         * \param e_id Entity ID
         * \param rad Direction of input in radians.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void on_set(const entity_id& e_id,
                           const float& rad,
                           mgr::entity_manager& world,
                           mgr::message_manager& messages,
                           const int64_t& current_time) {
            direction_set(e_id, rad, world, messages, current_time);
        };

        /*!
         * \brief Run the joystick direction unset function.
         * 
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void on_unset(const entity_id& e_id,
                             mgr::entity_manager& world,
                             mgr::message_manager& messages,
                             const int64_t& current_time) {
            direction_unset(e_id, world, messages, current_time);
        };

    private:
        std::size_t input_bind;

        std::function<void(const entity_id&,
                           const float&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> direction_set;

        std::function<void(const entity_id&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> direction_unset;
};

} //  namespace cmp

} //  namespace wte

#endif
