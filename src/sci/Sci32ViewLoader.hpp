//
//  Sci32ViewLoader.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/24/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32ViewLoader_hpp
#define Sci32ViewLoader_hpp

#include <memory>

namespace scissum {
    
    class Palette;
    class Sci32View;
    class Reader;
    
    class Sci32ViewLoader
    {
    public:
    
        virtual std::shared_ptr<Sci32View> load(std::shared_ptr<Reader> const &reader, Palette &palette);
        
    private:
        
        struct ResourceHeader;
        struct GroupHeader;
        struct CelHeader;
        
        void readResourceHeader(std::shared_ptr<Reader> const &reader, ResourceHeader &header);
        void readGroupHeader(std::shared_ptr<Reader> const &reader, GroupHeader &header, size_t length);
        void readCelHeader(std::shared_ptr<Reader> const &reader, CelHeader &header, size_t length);
        
    };
    
}

#endif /* Sci32ViewLoader_hpp */
