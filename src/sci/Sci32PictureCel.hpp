//
//  Sci32PictureCel.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32PictureCel_hpp
#define Sci32PictureCel_hpp

#include <memory>

namespace scissum {
    
    class Image;
    
    class Sci32PictureCel
    {
    public:
        
        Sci32PictureCel(size_t width, size_t height,
                        size_t horzDisplacement, size_t vertDisplacement,
                        size_t priority,
                        size_t relativeXPos,
                        size_t relativeYPos);
        
        ~Sci32PictureCel();
        
        void setImage(std::unique_ptr<Image> &&image);
        
        size_t width() const;
        size_t height() const;
        size_t horzDisplacement() const;
        size_t vertDisplacement() const;
        size_t priority() const;
        size_t relativeXPos() const;
        size_t relativeYPos() const;
        
        Image const &image() const;
        
        Image &image();
        
    private:
        
        size_t d_width;
        size_t d_height;
        size_t d_horzDisplacement;
        size_t d_vertDisplacement;
        size_t d_priority;
        size_t d_relativeXPos;
        size_t d_relativeYPos;
        
        std::unique_ptr<Image> d_image;
        
    };
    
}

#endif /* Sci32PictureCel_hpp */
