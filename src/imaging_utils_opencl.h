#pragma once

#include <cstdint>
#include <memory>

#include "opencl_buffer.h"
#include "opencl_command_queue.h"

namespace imaging
{
    class opencl_texture_storage
    {
        public:

        class storage_proxy
        {
            public:
            storage_proxy( std::shared_ptr< uint8_t > pixels ) : m_pixels(pixels)
            {

            }

            uint8_t* get_pixels_cpu() const
            {
                return m_pixels.get();
            }

            private:

            std::shared_ptr< uint8_t > m_pixels;
        };

        opencl_texture_storage( std::shared_ptr< opencl::buffer > buffer) :
        m_pixels( buffer )
        {

        }

        storage_proxy  get_pixels( const opencl::command_queue* queue ) const
        {
            auto size = m_pixels->get_size();
            std::unique_ptr<uint8_t[]> pixels(new uint8_t[size]);

            opencl::scoped_buffer_reader r( queue, m_pixels.get() );

            std::memcpy(pixels.get(), r.get_data(), size );

            return storage_proxy(std::shared_ptr<uint8_t>( pixels.release() , std::default_delete< uint8_t[] >()));
        }

        std::shared_ptr< opencl::buffer >   get_gpu_pixels() const
        {
            return m_pixels;
        }

        private:
        std::shared_ptr< opencl::buffer > m_pixels;    //points to device memory
    };

}
