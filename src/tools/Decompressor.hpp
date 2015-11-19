//
//  Decompressor.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Decompressor_hpp
#define Decompressor_hpp

#include <memory>

namespace scissum {
    
    class Decompressor
    {
    public:
        
        virtual size_t read(uint8_t *buffer, size_t length) = 0;
        
    };
    
}

#endif /* Decompressor_hpp */
