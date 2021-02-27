/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  damage.hpp
 *
 * See LICENSE.txt for copyright information
 *
 * Size component.
 */

#ifndef WTEDEMO_CMP_SIZE_HPP
#define WTEDEMO_CMP_SIZE_HPP

#include "wtengine/wtengine.hpp"

/*
 * Save the asteroid size
 */
class size final : public wte::cmp::component {
    public:
        /*!
         * Size constructor.
         * \param s Size.
         * \return void
         */
        inline size(int s) : the_size(s) {};

        int the_size;
};

#endif
