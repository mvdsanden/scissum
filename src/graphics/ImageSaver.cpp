//
//  ImageSaver.cpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/21/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#include "ImageSaver.hpp"

#include "Image.hpp"
#include "PixelBuffer.hpp"

#include <fstream>
#include <stdexcept>

namespace scissum {
    
    class ImageSaverImpl
    : public ImageSaver
    {
    public:
        
    };
    
    class PnmImageSaver
    : public ImageSaverImpl
    {
    public:
        
        static PnmImageSaver &instance()
        {
            static PnmImageSaver s_instance;
            return s_instance;
        }
        
        virtual void saveImage(Image const &image, std::string const &path)
        {
            std::ofstream stream(path);
            
            if (!stream) {
                throw std::runtime_error("error opening stream for writing");
            }
            
            saveImage(image, stream);
        }
        
    private:
        
        void saveImage(Image const &image, std::ostream &stream)
        {
            stream
            << "P6\n"
            << image.width() << " " << image.height() << "\n"
            << "255\n";

            ScopedMapReadOnly<PixelBuffer> map(image.pixelBuffer());
  
            Color const *colors = map.ptr();
            size_t length = image.pixelBuffer().size();
            
            while (length != 0) {
                uint8_t buffer[64*3];
                
                size_t count = std::min<size_t>(length, 64);
                
                for (size_t i = 0; i < count; ++i) {
                    buffer[i*3+0] = colors->r;
                    buffer[i*3+1] = colors->g;
                    buffer[i*3+2] = colors->b;
                    ++colors;
                }
              
                stream.write(reinterpret_cast<char const*>(buffer), count*3);
                
                length -= count;
            }
        }
        
    };
    
    ImageSaver &ImageSaver::getPnmSaver()
    {
        return PnmImageSaver::instance();
    }
    
}