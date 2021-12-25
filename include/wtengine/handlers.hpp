/*!
 * WTEngine | File:  handlers.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_HANDLERS_HPP
#define WTE_HANDLERS_HPP

#include <variant>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/config.hpp"

namespace wte {

enum handler_scope { GLOBAL, NONGAME, GAME };

using key_handler = std::function<void(int)>;     //!<  ...
using button_handler = std::function<void(int)>;  //!<  ...

using handler_types = std::variant<key_handler, button_handler>;

/*!
 * \class handler
 * \brief Input handler.
 * \tparam S
 */
template <size_t S>
class handler {
    friend class handlers;

    public:
        handler(const handler_types& h) : handle(h) {};
        handler(const handler&) = delete;         //!<  Delete copy constructor.
        void operator=(handler const&) = delete;  //!<  Delete assignment operator.
        ~handler() = default;

    private:
        handler_types handle;
};

/*!
 * \class handlers
 * \brief Input handlers.
 */
class handlers {
    friend class input;

    public:
        virtual ~handlers() = default;             //!<  Default virtual destructor.
        handlers(const handlers&) = delete;        //!<  Delete copy constructor.
        void operator=(handlers const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add handler
         * \return false
         */
        template <size_t S>
        inline static const bool add_handler(
            const handler<S>& handle
        ) {
            return false;
        };

    protected:
        inline handlers() {
            if(initialized == true) throw std::runtime_error("Handlers instance already running!");
            initialized = true;
        };

    private:
        inline static void run_handlers(const ALLEGRO_EVENT& event) {
            //
        };

        inline static bool initialized = false;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
