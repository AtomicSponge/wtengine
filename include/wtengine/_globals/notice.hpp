/*!
 * WTEngine | File:  notice.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_NOTICE_HPP
#define WTE_NOTICE_HPP

#include <string>

namespace wte {

/*!
 * \class notice
 * \brief Allows the engine to set a notice to be displayed to the screen.
 * 
 * Notices are cleared after the player presses a button.
 */
class notice final {
    public:
        notice() = delete;
        ~notice() = delete;

        /*!
         * \brief Set the notice and flag that its set.
         * \param n Notice to display.
         */
        static void set(
            const std::string& n
        );

        /*!
         * \brief Return the notice.
         * \return The notice message.
         */
        static const std::string get(void);

        /*!
         * \brief Check to see if a notice is set.
         * \return True if set, false if not set.
         */
        static const bool is_set(void);

        /*!
         * \brief Clear the notice.
         */
        static void clear(void);

    private:
        static std::string notice_text;     //  The notice display text.
        static bool notice_set;             //  Flag to check if a notice is set.
};

} //  end namespace wte

#endif
