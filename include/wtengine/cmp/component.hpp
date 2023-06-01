/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_COMPONENT_HPP)
#define WTE_CMP_COMPONENT_HPP

#include <memory>

namespace wte::cmp {

/*!
 * \class component
 * \brief Interface class for creating a component.
 * 
 * Extend this to creata a component that can be loaded into the Entity Manager.
 */
class component {
    public:
        virtual ~component() = default;             //  Default virtual destructor.
        component(const component&) = delete;       //  Delete copy constructor.
        void operator=(component const&) = delete;  //  Delete assignment operator.

    protected:
        component() = default;                      //  Default constructor.
};

/*!
 * \typedef std::shared_ptr<cmp::component> component_sptr
 * Component shared pointer.
 */
using component_sptr = std::shared_ptr<cmp::component>;

/*!
 * \typedef std::shared_ptr<const cmp::component> component_csptr
 * Constant component shared pointer.
 */
using component_csptr = std::shared_ptr<const cmp::component>;

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

}  //  end namespace wte::cmp

#endif
