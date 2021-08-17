/*!
 * WTEngine | File:  component.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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
        virtual ~component() = default;

        //!  Remove copy constructor
        component(const component&) = delete;
        //!  Remove assignment operator
        void operator=(component const&) = delete;

    protected:
        /*!
         * \brief Component constructor.
         */
        component() = default;
};

/*!
 * \typedef std::shared_ptr<cmp::component> component_sptr
 * Component shared pointer.
 */

typedef std::shared_ptr<cmp::component> component_sptr;
/*!
 * \typedef std::shared_ptr<const cmp::component> component_csptr
 * Constant component shared pointer.
 */
typedef std::shared_ptr<const cmp::component> component_csptr;

/*!
 * \typedef
 */
template <typename T>
using comp_ptr = std::shared_ptr<T>;
/*!
 * \typedef
 */
template <typename T>
using const_comp_ptr = std::shared_ptr<const T>;

} //  namespace cmp

} //  namespace wte

#endif
