//
//  Sci32ScriptWriter.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/25/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32ScriptWriter_hpp
#define Sci32ScriptWriter_hpp

#include <memory>
#include <iosfwd>

namespace scissum {
    
    class Sci32Script;
    
    class Sci32ScriptWriter
    {
    public:
    
        void writeAssembly(std::ostream &stream, std::shared_ptr<Sci32Script> const &script);
        
    };
    
}

#endif /* Sci32ScriptWriter_hpp */
