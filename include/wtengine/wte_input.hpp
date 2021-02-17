/*!
 * WTEngine | File:  wte_input.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_INPUT_HPP
#define WTE_INPUT_HPP

#include "wtenginee_input_map.hpp"
#include "wtenginee_global_defines.hpp"
#include "wtenginelobals/engine_flags.hpp"
#include "wtenginelobals/input_flags.hpp"
#include "wtenginelobals/alert.hpp"

namespace wte
{

/*!
 * \class wte_input
 * \brief Manage the input queue and process events.
 * 
 * The engine has two input modes that can be set with the WTE_INPUT_MODE build flag.
 * (0) Digital mode:
 *      - D-pad and sticks record 8-way directions.
 *      - D-pad and left stick are the same.
 *      - Analogue triggers are read as button presses.
 * (1) Analogue mode:
 *      - D-pad only registers button presses, read full data from sticks.
 *      - Analogue triggers have their data stored.
 */
class wte_input : private wte_input_map {
    public:
        /*!
         * \brief WTE Input destructor.
         */
        virtual ~wte_input();

    protected:
        /*!
         * \brief WTE Input constructor.
         */
        wte_input();

        /*!
         * \brief Create the input queue for capturing events.
         * 
         * Called during engine initialization.
         */
        void create_input_event_queue(void);

        /*!
         * \brief Destroy the input queue.
         * 
         * Called during engine de-initialization.
         */
        void destroy_input_event_queue(void);

        /*!
         * \brief Check the input queue for events.
         * 
         * Called during the main engine loop.  Pass any events to the input handler.
         */
        void check_input_events(void);

    private:
        void handle_input_event(const ALLEGRO_EVENT&);

        ALLEGRO_EVENT_QUEUE* input_event_queue;

        float x_axis[WTE_MAX_JOYSTICK_FLAGS];
        float y_axis[WTE_MAX_JOYSTICK_FLAGS];

        float last_x_axis[WTE_MAX_JOYSTICK_FLAGS];
        float last_y_axis[WTE_MAX_JOYSTICK_FLAGS];
};

} //  end namespace wte

#endif
