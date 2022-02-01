/*!
 * WTEngine | File:  manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_MANAGER_HPP
#define WTE_MGR_MANAGER_HPP

#include <stdexcept>

namespace wte::mgr {

/*!
 * \class manager
 * \brief Manager base class, restricted to one instance.
 * \tparam D Manager that is deriving the class.
 */
template <class D>
class manager {
    public:
        virtual ~manager() = default;             //!<  Default virtual destructor.
        manager(const manager&) = delete;         //!<  Delete copy constructor.
        void operator=(manager const&) = delete;  //!<  Delete assignment operator.

    protected:
        /*!
         * \brief Create a new engine manager.
         * \exception runtime_error Manager is already running.
         */
        manager() {
            if(initialized == true) throw std::runtime_error("An instance of this manager is already running!");
            initialized = true;
        };

    private:
        static bool initialized;  //  Force single instance.
};

}  //  end namespace wte::mgr

#endif
