//
//  Sci32SelectorVocabLoader.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32SelectorVocabLoader_hpp
#define Sci32SelectorVocabLoader_hpp

#include <memory>

namespace scissum {
    
    class Sci32SelectorVocab;
    class Reader;
    
    class Sci32SelectorVocabLoader
    {
    public:
        
        virtual std::shared_ptr<Sci32SelectorVocab> load(std::shared_ptr<Reader> const &reader, size_t length);
        
    };
    
}

#endif /* Sci32SelectorVocabLoader_hpp */
