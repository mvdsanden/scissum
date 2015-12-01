//
//  Sci32Cel.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32Cel_hpp
#define Sci32Cel_hpp

#include <memory>

namespace scissum {
    
    class Image;
    
    class Sci32Cel
    {
    public:
        
        Sci32Cel(size_t width,
                 size_t height,
                 ssize_t horzDisplacement,
                 ssize_t vertDisplacement);
        
        virtual ~Sci32Cel();
        
        void setImage(std::unique_ptr<Image> &&image);
        
        size_t width() const;
        size_t height() const;
        ssize_t horzDisplacement() const;
        ssize_t vertDisplacement() const;
        
        Image const &image() const;
        
        Image &image();
        
    private:
        
        size_t d_width;
        size_t d_height;
        ssize_t d_horzDisplacement;
        ssize_t d_vertDisplacement;
        
        std::unique_ptr<Image> d_image;
        
    };
    
}

#endif /* Sci32Cel_hpp */
