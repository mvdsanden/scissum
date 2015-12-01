//
//  Palette.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Palette_hpp
#define Palette_hpp

#include "Color.hpp"

namespace scissum {
    
    class Palette
    {
    public:
        
        Palette();
        
        ~Palette();
        
        Color &operator[](uint8_t index);
        Color const &operator[](uint8_t index) const;
        
        Color *ptr();
        Color const *ptr() const;
        
        void setSafetyPalette();
        
    private:
        
        __declspec( align( 16 ) ) Color d_table[256];
        
    };
    
}

#endif /* Palette_hpp */
