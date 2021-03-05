/*!
 * WTEngine | File:  component.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
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
 * \brief Interface class for creating a component.
 * 
 * Extend this to creata a component that can be loaded into the Entity Manager.
 */
class component {
    public:
        /*!
         * \brief Component destructor.
         */
        inline virtual ~component() {};

        //!  Remove copy constructor
        component(const component&) = delete;
        //!  Remove assignment operator
        void operator=(component const&) = delete;

    protected:
        /*!
         * \brief Component constructor.
         */
        inline component() {};
};

/*!
 * \typedef std::shared_ptr<cmp::component> component_sptr
 * Component shared pointer.
 */
typedef std::shared_ptr<cmp::component> component_sptr;
//extern template <typename C>
//using component_sptr = std::shared_ptr<C>;

/*!
 * \typedef std::shared_ptr<const cmp::component> component_csptr
 * Constant component shared pointer.
 */
typedef std::shared_ptr<const cmp::component> component_csptr;
//extern template <typename C>
//using component_csptr = std::shared_ptr<const C>;

} //  namespace cmp

} //  namespace wte

#endif
