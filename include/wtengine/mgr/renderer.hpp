/*!
 * WTEngine | File:  renderer.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_RENDERER_HPP
#define WTE_MGR_RENDERER_HPP

#include <string>
#include <utility>
#include <set>
#include <iterator>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/wte_global_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/engine_flags.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/cmp/components.hpp"
#include "wtengine/mgr/menu.hpp"
#include "wtengine/mgr/entities.hpp"

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
 * \class renderer
 * \brief An object that handles drawing the world to the screen.
 */
class renderer final : private manager<renderer> {
    friend class interface;

    public:
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
        static void set_scale_factor(const float& f);

        /*!
         * \brief Get scale factor.
         * 
         * \return Scale factor multiplier.
         */
        static const int get_scale_factor();

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
         * \brief Inform the renderer of the screen resolution.
         * 
         * Gets called by the engine when the screen resolution is updated.
         * 
         * \param w Resolution width in pixels.
         * \param h Resolution height in pixels.
         */
        static void update_resolution(const int& w, const int& h);

        /*!
         * \brief Reload the arena bitmap.
         * 
         * Called when the screen is updated.
         */
        static void reload_arena_bitmap(void);

        /*!
         * \brief Render method - Draw the game screen.
         */
        static void render(void);

    private:
        /*!
         * \brief renderer constructor.
         * 
         * Generates the renderer object.
         */
        inline renderer() {};

        /*!
         * \brief renderer destructor.
         */
        inline ~renderer() {};

        /*!
         * \brief Initialize the render manager.
         * 
         * Configures the internal objects for the render manager to use.
         */
        static void initialize(void);

        /*!
         * \brief De-initialize the render manager.
         * 
         * Destories the internal objects.
         */
        static void de_init(void);

        template <typename T> struct comparator {
            const bool operator() (const T& a, const T& b) const {
                return *a.second < *b.second;
            }
        };

        inline static ALLEGRO_BITMAP* title_bmp = NULL;
        inline static ALLEGRO_BITMAP* background_bmp = NULL;
        inline static ALLEGRO_BITMAP* arena_bmp = NULL;
        inline static ALLEGRO_BITMAP* render_tmp_bmp = NULL;
        inline static ALLEGRO_FONT* overlay_font = NULL;

        inline static ALLEGRO_TIMER* fps_timer = NULL;
        inline static ALLEGRO_EVENT_QUEUE* fps_event_queue = NULL;
        inline static ALLEGRO_EVENT fps_event;

        inline static std::size_t fps_counter = 0, fps = 0;

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

} //  namespace mgr

} //  namespace wte

#endif
