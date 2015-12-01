//
//  Sci32ScriptLoader.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32ScriptLoader_hpp
#define Sci32ScriptLoader_hpp

#include <memory>

namespace scissum {
    
    class Sci32Script;
    class Reader;
    
    class Sci32ScriptLoader
    {
    public:
        
        virtual std::shared_ptr<Sci32Script> load(std::shared_ptr<Reader> const &reader, size_t length);
        
    private:
        
        struct ScriptHeader;
        
    };
    
}
#endif /* Sci32ScriptLoader_hpp */
