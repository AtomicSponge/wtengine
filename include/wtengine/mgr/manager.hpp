/*!
 * WTEngine | File:  manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_MANAGER_HPP
#define WTE_MGR_MANAGER_HPP

#include <stdexcept>

namespace wte
{

namespace mgr
{

/*!
 * \class manager
 * \brief Extend this to create a new engine manager.
 * 
 * These are restricted to a single instance.
 */
template <class derived> class manager {
    public:
        virtual ~manager() { initialized = false; };

        //!  Remove copy constructor
        manager(const manager&) = delete;
        //!  Remove assignment operator
        void operator=(manager const&) = delete;

    private:
        static bool initialized;  //  Force single instance.

    protected:
        /*!
         * \brief Create a new engine manager.
         * \exception runtime_error Manager is already running.
         */
        manager() {
            if(initialized == true) throw std::runtime_error("An instance of this manager is already running!");
            initialized = true;
        };
};

}  // end namespace mgr

}  // end namespace wte

#endif
