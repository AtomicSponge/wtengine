/*!
 * WTEngine | File:  handler.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_HANDLER_HPP
#define WTE_HANDLER_HPP

#include <variant>
#include <functional>

#include <allegro5/allegro.h>

namespace wte {

enum handler_scopes { GLOBAL_HANDLES, NONGAME_HANDLES, GAME_HANDLES };

using key_handler = std::function<void(int, ALLEGRO_DISPLAY*)>;
using mouse_axes_event = std::function<void(int, int, int, int, int, int, int, float, ALLEGRO_DISPLAY*)>;
using mouse_button_event = std::function<void(int, int, int, int, unsigned int, float, ALLEGRO_DISPLAY*)>;
using joy_axis_handler = std::function<void(int, int, float, ALLEGRO_JOYSTICK*)>;
using joy_button_handler = std::function<void(int, ALLEGRO_JOYSTICK*)>;  //!<  ...

using handler_types = std::variant<key_handler, mouse_axes_event, mouse_button_event, joy_axis_handler, joy_button_handler>;

/*!
 * \class handler
 * \brief Input handler.
 * \tparam S
 */
template <size_t S>
class handler {
    friend class handlers;

    public:
        /*!
         * \brief Create a new handler.
         * \param h New handle function.
         */
        handler(const handler_types& h) : handle(h) {};

        handler() = delete;                       //!<  Delete default constructor.
        handler(const handler&) = delete;         //!<  Delete copy constructor.
        void operator=(handler const&) = delete;  //!<  Delete assignment operator.
        ~handler() = default;                     //!<  Default destructor.

    private:
        handler_types handle;
};

/*!
 * \brief Add handler
 * \return false
 */
template <size_t S>
inline static void add_input_handler(const handler<S>& handle) {
    //
};

}  //  end namespace wte

#endif
