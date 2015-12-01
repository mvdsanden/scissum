//
//  Sci32PaletteLoader.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32PaletteLoader_hpp
#define Sci32PaletteLoader_hpp

#include <memory>

namespace scissum {
    
    class Palette;
    class Reader;
    
    class Sci32PaletteLoader
    {
    public:
        
        virtual void load(std::shared_ptr<Reader> const &reader, Palette &palette);
        
    private:
        
        struct PaletteHeader;

        void readPalette(std::shared_ptr<Reader> const &reader, Palette &palette);
        
    };
    
}

#endif /* Sci32PaletteLoader_hpp */
