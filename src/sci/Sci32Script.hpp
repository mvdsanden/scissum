//
//  Sci32Script.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32Script_hpp
#define Sci32Script_hpp

#include <cstdint>

namespace scissum {
    
    class Sci32Script
    {
    public:
        
        Sci32Script(uint8_t *data, size_t length);
        
        ~Sci32Script();
        
        uint8_t const *data() const;
        
        uint8_t *data();
        
        size_t length() const;
        
        uint8_t const *bytecodeArea() const;
        
        size_t bytecodeLength() const;
        
    private:
        
        struct Header;
        
        uint8_t *d_data;
        
        size_t d_length;
        
        uint8_t *d_bytecodeData;
        
        size_t d_bytecodeLength;
        
    };
    
}

#endif /* Sci32Script_hpp */
