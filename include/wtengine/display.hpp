/*!
 * WTEngine | File:  display.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_DISPLAY_HPP
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>

#include <cmath>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <type_traits>
#include <stdexcept>

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/mgr/renderer.hpp"

namespace wte {

/*!
 * \class display
 * \brief Handles control of the display.
 */
class display {
    friend class engine;

    public:
        virtual ~display() = default;             //!<  Default virtual destructor.
        display(const display&) = delete;         //!<  Delete copy constructor.
        void operator=(display const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Get the window title.
         * \return Window title as string.
         */
        const std::string get_window_title(void) const;

    protected:
        /*!
         * \brief Display constructor.
         * \param title Window title.
         */
        display(const std::string& title);

    private:
        /*
         * Configure the display.
         */
        void create_display(void);

        /*
         * Destroy the display.
         */
        void destroy_display(void);

        /*
         * Reconfigure the display.
         */
        void reconf_display(void);

        static ALLEGRO_DISPLAY* _display;  //  Allegro object for the display.

        std::string window_title;  //  Title for application window.
        static bool initialized;   //  Restrict to one instance.

        class al_bitmap_converter final {
            friend class display;

            public:
                al_bitmap_converter() = delete;
                ~al_bitmap_converter() = delete;
                al_bitmap_converter(const al_bitmap_converter&) = delete;
                void operator=(al_bitmap_converter const&) = delete;

            private:
                /*
                *
                */
                inline static void backup_bitmaps(void) {
                    _bitmaps_backup.clear();
                    /*al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
                    for (auto& it: std::get<1>(mgr::assets<>::_assets)) {
                        if(it.second->isconverted()) {
                            //  Make a conversion safe copy in the backup map.
                            _bitmaps_backup.insert(std::make_pair(it.first, al_clone_bitmap(**it.second)));
                            //  Now delete the old item.
                            al_destroy_bitmap(**it.second);
                        }
                    }*/
                };

                /*
                *
                */
                inline static void reload_bitmaps(void) {
                    /*al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
                    for (auto& it: _bitmaps_backup) {
                        //  Restore bitmap.
                        try {
                            std::get<idx>(mgr::assets<>::_assets).get(it.first)->set(it.second);
                        } catch(...) {}
                        //  Now delete the old backup bitmap.
                        al_destroy_bitmap(it.second);
                    }*/
                    _bitmaps_backup.clear();
                };

                inline static std::map<
                    std::string,
                    ALLEGRO_BITMAP*
                > _bitmaps_backup;

                /*template<
                    typename Tuple,
                    typename Indices = std::make_index_sequence<std::tuple_size<Tuple>::value>
                >
                struct runtime_get_func_table;

                template<typename Tuple, std::size_t ...Indices>
                struct runtime_get_func_table<Tuple,std::index_sequence<Indices...>>{
                    using return_type = typename std::tuple_element<0,Tuple>::type&;
                    using get_func_ptr = return_type (*)(Tuple&) noexcept;
                    inline static constexpr get_func_ptr table[std::tuple_size<Tuple>::value] = {
                        &std::get<Indices>...
                    };
                };

                inline static constexpr std::size_t idx = Indices<al_bitmap, mgr::assets<>::_assets>;

                template<typename Tuple, std::size_t ...Indices>
                constexpr typename runtime_get_func_table<Tuple, std::index_sequence<Indices...>>::get_func_ptr
                runtime_get_func_table<Tuple, std::index_sequence<Indices...>>::table[std::tuple_size<Tuple>::value];

                template<typename Tuple>
                constexpr typename std::tuple_element<0, typename std::remove_reference<Tuple>::type>::type& runtime_get(Tuple&& t, std::size_t index) {
                    using tuple_type = typename std::remove_reference<Tuple>::type;
                    if(index >= std::tuple_size<tuple_type>::value)
                        throw std::runtime_error("Index not found.");
                    return runtime_get_func_table<tuple_type>::table[index](t);
                };*/
        };
};

} //  end namespace wte

#endif
