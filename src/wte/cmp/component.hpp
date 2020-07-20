/*!
 * \brief WTEngine | File:  component.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Component interface class.  Extend this to create a new component.
 */

#ifndef WTE_CMP_COMPONENT_HPP
#define WTE_CMP_COMPONENT_HPP

#include <memory>

namespace wte
{

namespace cmp
{

/*!
 * \class component
 * \brief Interface class for creating a component
 */
class component {
    public:
        /*!
         * Component destructor.
         * \param void
         * \return void
         */
        inline virtual ~component() {};

        //!  Remove copy constructor
        component(const component&) = delete;
        //!  Remove assignment operator
        void operator=(component const&) = delete;

    protected:
        /*!
         * Component constructor.
         * \param void
         * \return void
         */
        inline component() {};
};

//! \typedef Component shared pointer.
typedef std::shared_ptr<cmp::component> component_sptr;
//! \typedef Constant component shared pointer.
typedef std::shared_ptr<const cmp::component> component_csptr;

} //  namespace cmp

} //  namespace wte

#endif
