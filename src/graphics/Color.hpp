//
//  Color.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <cstdint>

namespace scissum {
    
    struct Color
    {
        union
        {
            struct
            {
                uint8_t a;
                uint8_t r;
                uint8_t g;
                uint8_t b;
            };
            uint32_t color;
            uint8_t v[4];
        };
    };
    
}

#endif /* Color_hpp */
