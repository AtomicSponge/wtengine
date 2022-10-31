/*!
 * wtengine | File:  wrappers.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_WRAPPERS_HPP)
#define WTE_WRAPPERS_HPP

#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/mgr/assets.hpp"

namespace wte {

/*!
 * \class al_bitmap
 * \brief Provides a wrapper to Allegro bitmaps.
 */
class al_bitmap final {
    public:
        /*!
         * \brief Creates a null bitmap that preserves its data.
         */
        al_bitmap();

        /*!
         * \brief Creates a bitmap of w x h that preserves its data.
         * \param w Width in pixels.
         * \param h Hieght in pixels.
         */
        al_bitmap(
            const int& w,
            const int& h
        );

        ~al_bitmap() = default;  //!<  Default destructor.

        /*!
         * \brief Return a pointer to the internal Allegro bitmap.
         * \return Allegro bitmap pointer.
         */
        ALLEGRO_BITMAP* operator*() { return _al_bitmap; };

        /*!
         * \brief Load a bitmap from file.
         * \param fname Filename to load.
         * \return True if loaded, false on error.
         */
        bool load(const std::string& fname);

        /*!
         * \brief Copy a bitmap from an existing one.
         * \param temp_bmp Bitmap to copy.
         */
        void clone(ALLEGRO_BITMAP* temp_bmp);

        /*!
         * \brief Destroy the internal bitmap.
         */
        void destroy(void);

        /*!
         * \brief Get bitmap width.
         * \return Bitmap width.
         */
        int get_width(void) const;

        /*!
         * \brief Get bitmap height.
         * \return Bitmap height.
         */
        int get_height(void) const;

    private:
        ALLEGRO_BITMAP* _al_bitmap;  //  Internal Allegro bitmap.
};

/*!
 * \class al_font
 * \brief Provides a wrapper to Allegro fonts.
 */
class al_font final {
    public:
        /*!
         * \brief Create a font asset using Allegro's default font.
         */
        al_font();

        /*!
         * \brief Create a font asset from file.
         * \param fname Font file name.
         * \param size Font size.  See Allegro docs on fonts.
         * \param flags Font flags.  See Allegro docs on fonts.
         */
        al_font(
            const std::string& fname,
            const int& size,
            const int& flags
        );

        ~al_font() = default;  //!<  Default destructor.

        /*!
         * \brief Get font asset.
         * \return Pointer to font asset.
         */
        ALLEGRO_FONT* operator*() { return _al_font; };

    private:
        ALLEGRO_FONT* _al_font;
};

/*!
 * \class al_sample
 * \brief Provides a wrapper to Allegro samples.
 */
class al_sample final {
    public:
        /*!
         * \brief Load a sample asset.
         * \param fname Sample file name.
         */
        al_sample(const std::string& fname);

        al_sample() = delete;    //!<  Delete default constructor.
        ~al_sample() = default;  //!<  Default destructor.

        /*!
         * \brief Get sample asset.
         * \return Pointer to sample asset.
         */
        ALLEGRO_SAMPLE* operator*() { return _al_sample; };

    private:
        ALLEGRO_SAMPLE* _al_sample;
};

/*!
 * \class al_audio
 * \brief Provides a wrapper to Allegro audio streams.
 */
class al_audio final {
    public:
        /*!
         * \brief Load an audio asset.
         * \param fname Audio file name.
         */
        al_audio(const std::string& fname);

        al_audio() = delete;    //!<  Delete default constructor.
        ~al_audio() = default;  //!<  Default destructor.

        /*!
         * \brief Get audio asset.
         * \return Pointer to audio asset.
         */
        ALLEGRO_AUDIO_STREAM* operator*() { return _al_audio; };

        /*!
         * \brief Set the play speed of the audio asset.
         * \param speed New speed value.
         */
        void set_speed(const float& speed);

        /*!
         * \brief Get the play speed of the audio asset.
         * \return Current play speed value.
         */
        float get_speed(void) const;
        
        /*!
         * \brief Get the frequency.
         * \return Frequency of the audio asset.
         */
        unsigned int get_frequency(void) const;
        
        /*!
         * \brief Get the length of the audio asset.
         * \return The length in seconds.
         */
        double get_length(void) const;
        
        /*!
         * \brief Check if the audio asset is being played.
         * \return True if being played, else false.
         */
        bool is_playing(void) const;

    private:
        ALLEGRO_AUDIO_STREAM* _al_audio;
};

}  //  end namespace wte

#endif
