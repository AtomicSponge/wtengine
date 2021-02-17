/*!
 * WTEngine | File:  render_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_RENDER_MANAGER_HPP
#define WTE_MGR_RENDER_MANAGER_HPP

#include <string>
#include <utility>
#include <set>
#include <iterator>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wtenginenager.hpp"
#include "wtenginegine_time.hpp"

#include "wtengine/wte_global_defines.hpp"
#include "wtengine/_globals/engine_flags.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/cmp/components.hpp"
#include "wtenginenu_manager.hpp"
#include "wtenginetity_manager.hpp"

namespace wte
{

namespace mgr
{

/*!
 * Container for an entity and component pair.  Used for sorting.
 * \tparam Component type
 */
template <typename T>
using entity_component_pair = std::pair<const entity_id, std::shared_ptr<const T>>;

/*!
 * \class render_manager
 * \brief An object that handles drawing the world to the screen.
 */
class render_manager final : private manager<render_manager>, private engine_time {
    public:
        /*!
         * \brief render_manager constructor.
         * 
         * Generates the render_manager object.
         */
        render_manager() :
        fps_counter(0), fps(0) {};

        /*!
         * \brief render_manager destructor.
         */
        ~render_manager() {};

        /*!
         * \brief Initialize the render manager.
         * 
         * Configures the internal objects for the render manager to use.
         */
        void initialize(void);

        /*!
         * \brief De-initialize the render manager.
         * 
         * Destories the internal objects.
         */
        void de_init(void);

        /*!
         * \brief Inform the renderer of the screen resolution.
         * 
         * Gets called by the engine when the screen resolution is updated.
         * 
         * \param w Resolution width in pixels.
         * \param h Resolution height in pixels.
         */
        void update_resolution(const int& w, const int& h);

        /*!
         * \brief Get screen width.
         * 
         * \return Screen width in pixels.
         */
        static const int get_screen_width(void);

        /*!
         * \brief Get screen height.
         * 
         * \return Screen height in pixels.
         */
        static const int get_screen_height(void);

        /*!
         * \brief Set scale factor.
         * 
         * This is used to scale the game arena and menus to a larger size.
         * For support of higher resolution screens.
         * 
         * \param f New scale factor value.
         */
        void set_scale_factor(const float& f);

        /*!
         * \brief Get scale factor.
         * 
         * \return Scale factor multiplier.
         */
        static const int get_scale_factor();

        /*!
         * \brief Reload the arena bitmap.
         * 
         * Called when the screen is updated.
         */
        void reload_arena_bitmap(void);

        /*!
         * \brief Set the arena size.
         * 
         * Once the renderer has been initialized it can not be changed.
         * This should be called during engine initialization before the main object is created.
         * 
         * \param w Arena width in pixels.
         * \param h Arena height in pixels.
         */
        static void set_arena_size(const int& w, const int& h);

        /*!
         * \brief Get the arnea width.
         * 
         * \return The arena width in pixels.
         */
        static const int get_arena_width(void);

        /*!
         * \brief Get the arena height.
         * 
         * \return The arena height in pixels.
         */
        static const int get_arena_height(void);

        /*!
         * \brief Set the title screen.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param fname Filename of the title screen.
         */
        static void set_title_screen(const std::string& fname);

        /*!
         * \brief Set the title screen.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param fname Filename of the title screen.
         */
        static void set_background_screen(const std::string& fname);

        /*!
         * \brief Set the font to be used by the renderer.
         * 
         * If not called, Allegro's default font will be used.
         * This should be called during engine initialization before the main object is created.
         * See the Allegro docs for more information on al_load_font and the parameters used.
         * 
         * \param fname Filename to use.
         * \param size Font size.
         * \param flags Font flags.
         */
        static void set_font_file(const std::string& fname, const int& size, const int& flags);

        /*!
         * \brief Render method - Draw the game screen.
         * Gets passed the entity manager and menu manager by
         * reference then draws everything to screen.
         * \param menus Reference to menu manager.
         * \param world Reference to entity manager.
         */
        void render(const menu_manager&, const entity_manager&);

    private:
        template <typename T> struct comparator {
            const bool operator() (const T& a, const T& b) const {
                return *a.second < *b.second;
            }
        };

        ALLEGRO_BITMAP* title_bmp;
        ALLEGRO_BITMAP* background_bmp;
        ALLEGRO_BITMAP* arena_bmp;
        ALLEGRO_BITMAP* render_tmp_bmp;
        ALLEGRO_FONT* overlay_font;

        ALLEGRO_TIMER* fps_timer;
        ALLEGRO_EVENT_QUEUE* fps_event_queue;
        ALLEGRO_EVENT fps_event;

        std::size_t fps_counter, fps;

        inline static int screen_w = 0, screen_h = 0;
        inline static float scale_factor = 1.0;

        inline static int arena_w = 0, arena_h = 0;
        inline static bool arena_created = false;

        inline static std::string title_screen_file = "";
        inline static std::string background_file = "";
        inline static std::string render_font_file = "";
        inline static int render_font_size = 0;
        inline static int render_font_flags = 0;
};

template <> bool render_manager::manager<render_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
