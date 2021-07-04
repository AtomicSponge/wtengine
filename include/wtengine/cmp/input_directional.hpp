/*!
 * WTEngine | File:  input_directional.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_INPUT_DIRECTIONAL_HPP
#define WTE_CMP_INPUT_DIRECTIONAL_HPP

#include <functional>
#include <cassert>

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/entities.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/_globals/input_flags.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"

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
            std::function<void(const entity_id&, const float&)> func_a,
            std::function<void(const entity_id&)> func_b
        );

        /*!
         * \brief Input Directional destructor.
         */
        inline ~input_directional() {};

        /*!
         * \brief Get the joystick bounded to this component.
         * 
         * \return The joystick enum value.
         */
        const std::size_t get_bind(void) const;

        /*!
         * \brief Run the joystick direction set function.
         * 
         * \param e_id Entity ID
         * \param rad Direction of input in radians.
         */
        void on_set(const entity_id& e_id,
                    const float& rad);

        /*!
         * \brief Run the joystick direction unset function.
         * 
         * \param e_id Entity ID
         */
        void on_unset(const entity_id& e_id);

    private:
        std::size_t input_bind;

        std::function<void(const entity_id&, const float&)> direction_set;

        std::function<void(const entity_id&)> direction_unset;
};

} //  namespace cmp

} //  namespace wte

#endif
