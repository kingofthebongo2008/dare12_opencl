#pragma once

#include <cstdint>
#include <memory>


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

        opencl_texture_storage(uint8_t pixels[], size_t size) 
        {

        }

        storage_proxy  get_pixels( ) const
        {

            std::unique_ptr<uint8_t[]> pixels(new uint8_t[m_size]);

            /*
            cuda::throw_if_failed(cudaMemcpy(pixels.get(), m_pixels.get(), m_size, cudaMemcpyDeviceToHost));
            */

            return storage_proxy(std::shared_ptr<uint8_t>( pixels.release() , std::default_delete< uint8_t[] >()));
        }

        uint8_t*    get_gpu_pixels() const
        {
            return m_pixels.get();
        }

        private:

        std::shared_ptr< uint8_t > m_pixels;    //points to device memory
        size_t                     m_size;
    };

}
