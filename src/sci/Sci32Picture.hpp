//
//  Sci32Picture.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32Picture_hpp
#define Sci32Picture_hpp

#include <memory>
#include <vector>

namespace scissum {
    
    class Sci32PictureCel;
    
    class Sci32Picture
    // : public SciPicture
    {
    public:
        
        Sci32Picture(size_t width, size_t height);
        
        ~Sci32Picture();
        
        void appendCel(std::unique_ptr<Sci32PictureCel> &&cel);
        
        size_t celCount() const;
        
        Sci32PictureCel const &getCel(size_t index) const;
        
        Sci32PictureCel &getCel(size_t index);
        
        std::vector<std::unique_ptr<Sci32PictureCel>> const &cels() const;
        
        std::vector<std::unique_ptr<Sci32PictureCel>> &cels();
        
    private:
        
        size_t d_width;
        
        size_t d_height;
        
        std::vector<std::unique_ptr<Sci32PictureCel>> d_cels;
        
    };
    
}

#endif /* Sci32Picture_hpp */
