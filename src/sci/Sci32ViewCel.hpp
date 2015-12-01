//
//  Sci32ViewCel.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32ViewCel_hpp
#define Sci32ViewCel_hpp

#include "Sci32Cel.hpp"

namespace scissum {
    
    class Sci32ViewCel
    : public Sci32Cel
    {
    public:
        
        Sci32ViewCel(size_t width,
                     size_t height,
                     size_t horzDisplacement,
                     size_t vertDisplacement);
        
        virtual ~Sci32ViewCel();
        
    };
    
}

#endif /* Sci32ViewCel_hpp */
