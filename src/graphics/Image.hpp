//
//  Image.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <memory>

namespace scissum {
    
    class PixelBuffer;
    
    class Image
    {
    public:
    
        Image(size_t width, size_t height, std::unique_ptr<PixelBuffer> &&pixelBuffer);
        
        Image(size_t width, size_t height);
        
        ~Image();
        
        void setPixelBuffer(std::unique_ptr<PixelBuffer> &&pixelBuffer);
        
        PixelBuffer &pixelBuffer();
        
        PixelBuffer const &pixelBuffer() const;
        
        size_t width() const;
        
        size_t height() const;
        
    private:
        
        size_t d_width;
        
        size_t d_height;
        
        std::unique_ptr<PixelBuffer> d_pixelBuffer;
        
    };
    
}

#endif /* Image_hpp */
