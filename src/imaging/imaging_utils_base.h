#pragma once

#include <cstdint>
#include <memory>


namespace imaging
{
    enum image_type : int32_t
    {
        rgb = 0,
        grayscale = 1,
        float32 = 2
    };

    template <image_type> inline uint32_t get_bpp();

    template <> inline uint32_t get_bpp< image_type::rgb> ()
    {
        return 24;
    }

    template <> inline uint32_t get_bpp< image_type::grayscale>()
    {
        return 8;
    }

    template <> inline uint32_t get_bpp< image_type::float32>()
    {
        return 32;
    }


    template <typename pixels_storage>
    class texture 
    {

    public:

        texture( uint32_t width, uint32_t height, size_t bpp, size_t size, uint32_t pitch, image_type type, const pixels_storage& storage ) :
            m_width(width)
            , m_height(height)
            , m_bpp(bpp)
            , m_size(size)
            , m_row_pitch(pitch)
            , m_image_type(type)
            , m_storage(storage)
        {

        }

        uint32_t get_width() const
        {
            return m_width;
        }

        uint32_t get_height() const
        {
            return m_height;
        }

        size_t   get_bpp() const
        {
            return m_bpp;
        }

        uint32_t get_pitch() const
        {
            return m_row_pitch;
        }

        image_type get_image_type() const
        {
            return m_image_type;
        }


        size_t get_size() const
        {
            return m_size;
        }

        template <typename a0>
        typename pixels_storage::storage_proxy get_pixels( a0&& a ) const
        {
            return m_storage.get_pixels( a );
        }


        typename pixels_storage::storage_proxy get_pixels() const
        {
            return m_storage.get_pixels();
        }

        const pixels_storage& get_storage() const
        {
            return m_storage;
        }

    private:

        image_type      m_image_type;
        size_t          m_bpp;
        uint32_t        m_row_pitch;
        size_t          m_size;

        uint32_t        m_width;
        uint32_t        m_height;
        pixels_storage  m_storage;
    };
}
