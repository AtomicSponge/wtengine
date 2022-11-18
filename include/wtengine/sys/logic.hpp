/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SYS_LOGIC_HPP)
#define WTE_SYS_LOGIC_HPP

#include "wtengine/sys/system.hpp"

namespace wte::sys {

/*!
 * \class logic
 * \brief Processes entities that have ai components.
 * 
 * Also sends messages to entities with dispatch components.
 */
class logic final : public system {
    public:
        logic();
        ~logic() = default;

        /*!
         * \brief Finds all entities with an ai component and processes their logic.
         */
        void run(void) override;
};

}  //  end namespace wte::sys

#endif
