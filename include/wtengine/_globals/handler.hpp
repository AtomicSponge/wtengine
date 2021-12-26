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

#endif

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
