/*!
 * WTEngine | File:  wte_main.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MAIN_HPP
#define WTE_MAIN_HPP

#include "wte_global_defines.hpp"
#include "wte_display.hpp"
#include "wte_input.hpp"
#include "_globals/engine_cfg.hpp"
#include "_globals/game_cfg.hpp"
#include "_globals/engine_flags.hpp"
#include "_globals/alert.hpp"
#include "mgr/engine_time.hpp"
#include "mgr/managers.hpp"

namespace wte
{

/*!
 * \class wte_main
 * \brief The main engine object.
 * 
 * Sets up various system objects used by the engine.
 * Contains the main game loop and members for managing the game and engine.
 */
class wte_main : private wte_display, private wte_input {
    public:
        /*!
         * \brief Engine destructor.
         * 
         * Frees up instance, set initialized flag to false.
         * Also makes sure to unload the engine.
         */
        inline ~wte_main();

        //  Remove copy constructor.
        wte_main(const wte_main&) = delete;
        //  Remove assignment operator.
        void operator=(wte_main const&) = delete;

        /*!
         * \brief Add file path to provide to PhysFS.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param flocation File location to add to PhysFS.
         */
        inline static void add_file_location(const std::string& flocation);

        void do_game(void);

    protected:
        /*!
         * \brief Engine constructor.
         * 
         * Force single instance, set initialized flag to true.
         * Throws a runtime error if another instance is called.
         * 
         * \param argc Command line arguments.
         * \param argv Command line arguments count.
         * \param title Window title.
         */
        inline wte_main(const int argc, char **argv, const std::string& title) :
            wte_display(title);

        /* These function members are overridden in the derived class */
        //!  Define this to implement custom menu layout.
        virtual void load_menus(void) = 0;
        //!  Define this to load all systems to be used by the game.
        virtual void load_systems(void) = 0;
        //!  Define what gets loaded when a game starts.
        virtual void new_game(void) = 0;
        //!  Define what happens at the end of a game.
        virtual void end_game(void) = 0;
        //!  Optional:  On menu open.
        virtual void on_menu_open(void) {};
        //!  Optional:  On menu close.
        virtual void on_menu_close(void) {};
        //!  Optional:  Out of focus handler
        virtual void out_of_focus(void) {};
        //!  Optional:  In focus handler
        virtual void back_in_focus(void) {};
        //!  Optional:  Define custom system message handling.
        virtual void handle_custom_sys_msg(const message&) {};
        /* *** End overridden function members *** */

        //  Managers available to game object declared here.
        //!  World manager
        mgr::entity_manager world;
        //!  Menu manager
        mgr::menu_manager menus;
        //!  Message manager
        mgr::message_manager messages;
        //!  Spawn manager
        mgr::spawn_manager spawner;
        //!  System manager
        mgr::system_manager systems;

    private:
        /*!
         * \brief Load the engine's managers.
         * 
         * Called before the main loop starts.
         */
        inline void wte_load(void);

        /*!
         * \brief Unload the engine's managers.
         * 
         * Called after the main loop ends running.
         */
        inline void wte_unload(void);

        void process_new_game(const std::string&);
        void process_end_game(void);
        void handle_sys_msg(message_container&);

        //  Managers only available internally declared here.
        mgr::audio_manager audio_th;

        //  Used for mapping commands and switching on system messages:
        enum CMD_STR_VALUE {
            CMD_STR_EXIT,               CMD_STR_ALERT,
            CMD_STR_NEW_GAME,           CMD_STR_END_GAME,
            CMD_STR_OPEN_MENU,          CMD_STR_CLOSE_MENU,
            CMD_STR_ENABLE_SYSTEM,      CMD_STR_DISABLE_SYSTEM,
            CMD_STR_SET_ENGCFG,         CMD_STR_SET_GAMECFG,
            CMD_STR_RECONF_DISPLAY,     CMD_STR_FPS_COUNTER,
            CMD_STR_LOAD_SCRIPT
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        //  Allegro objects used by the engine.
        ALLEGRO_TIMER* main_timer;
        ALLEGRO_EVENT_QUEUE* main_event_queue;

        //  Vector of file paths to provide to PhysFS.
        inline static std::vector<std::string> file_locations = {};
        //  Restrict to one instance of the engine running.
        inline static bool initialized = false;
};

} //  end namespace wte

#endif
