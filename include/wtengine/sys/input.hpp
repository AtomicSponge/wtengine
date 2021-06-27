/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_INPUT_HPP
#define WTE_SYS_INPUT_HPP

#include "wtengine/sys/system.hpp"
#include "wtengine/_globals/input_flags.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class input
 * \brief Processes keyboard input
 */
class input final : public system {
    public:
        input();
        ~input();

        void disable(void) override;

        /*!
         * \brief Input system run method
         * 
         * Get all entities tagged with an input handler component and run.
         */
        void run(void) override;
};

}  //  namespace sys

}  //  namespace wte

#endif
