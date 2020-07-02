/*!
 * \brief WTEngine | File:  input_directional.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details ...
 */

#ifndef WTE_CMP_INPUT_DIRECTIONAL_HPP
#define WTE_CMP_INPUT_DIRECTIONAL_HPP

#include <functional>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class ...
 * ...
 */
class input_directional final : public component {
    public:
        /*!
         * Input Directional constructor.
         * \param void
         * \return void
         */
        inline input_directional(
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
        ) : input_bind(ib), direction_set(func_a), direction_unset(func_b) {};

        /*!
         * Input Directional destructor.
         * \param void
         * \return void
         */
        inline ~input_directional() {};

        /*!
         *
         */
        inline const std::size_t get_bind(void) const {
            return input_bind;
        };

        /*!
         *
         */
        inline void on_set(const entity_id& e_id,
                           const float& rad,
                           mgr::entity_manager& world,
                           mgr::message_manager& messages,
                           const int64_t& current_time) {
            direction_set(e_id, rad, world, messages, current_time);
        };

        /*!
         *
         */
        inline void on_unset(const entity_id& e_id,
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
