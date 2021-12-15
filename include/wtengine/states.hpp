/*!
 * WTEngine | File:  states.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_STATES_HPP
#define WTE_STATES_HPP

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/config.hpp"
#include "wtengine/handlers.hpp"

namespace wte {

/*!
 * \class states
 * \brief Input states.
 */
class states {
    friend class handlers;

    public:
        virtual ~states() = default;             //!<  Default virtual destructor.
        states(const states&) = delete;          //!<  Delete copy constructor.
        void operator=(states const&) = delete;  //!<  Delete assignment operator.

    protected:
        states();  //!<  Constructor

    private:
        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
