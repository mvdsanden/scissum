//
//  Sci32PictureCel.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32PictureCel_hpp
#define Sci32PictureCel_hpp

#include "Sci32Cel.hpp"

namespace scissum {
    
    class Sci32PictureCel
    : public Sci32Cel
    {
    public:
        
        Sci32PictureCel(size_t width, size_t height,
                        size_t horzDisplacement, size_t vertDisplacement,
                        size_t priority,
                        size_t relativeXPos,
                        size_t relativeYPos);
        
        virtual ~Sci32PictureCel();
        
        size_t priority() const;
        size_t relativeXPos() const;
        size_t relativeYPos() const;        
        
    private:
        
        size_t d_priority;
        size_t d_relativeXPos;
        size_t d_relativeYPos;
        
    };
    
}

#endif /* Sci32PictureCel_hpp */
