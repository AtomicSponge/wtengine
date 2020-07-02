/*!
 * \brief WTEngine | File:  input_button.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input button component.  Tag to be processed by input system.
 */

#ifndef WTE_CMP_INPUT_BUTTON_HPP
#define WTE_CMP_INPUT_BUTTON_HPP

#include <map>
#include <utility>
#include <functional>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Input button component
 * Tag an entity to be processed by the Input system
 */
class input_button final : public component {
    //! \typedef Container for the action map.
    typedef std::map<const std::size_t,
                     std::function<void(const entity_id&,
                                        mgr::entity_manager&,
                                        mgr::message_manager&,
                                        const int64_t&)>
            > action_map;

    public:
        /*!
         * Input button constructor.
         * \param void
         * \return void
         */
        inline input_button() {
            input_action_map.clear();
        };

        /*!
         * Input button destructor.
         * \param void
         * \return void
         */
        inline ~input_button() {
            input_action_map.clear();
        };

        /*!
         * Get the input map.
         * \param void
         * \return Map of input events.
         */
        inline const action_map get_map(void) const {
            return input_action_map;
        };

        /*!
         * Add event button.
         * \param button Button event.
         * \param func Action to take on event.
         * \return True if created, false if not.
         */
        inline const bool add_event(
            const std::size_t& button,
            void func(const entity_id&,
                      mgr::entity_manager&,
                      mgr::message_manager&,
                      const int64_t&)
        ) {
            auto ret = input_action_map.insert(std::make_pair(button, func));
            return ret.second;
        };

    private:
        action_map input_action_map;
};

} //  namespace cmp

} //  namespace wte

#endif
