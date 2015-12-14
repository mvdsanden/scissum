//
//  Sci32SelectorVocabWriter.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32SelectorVocabWriter_hpp
#define Sci32SelectorVocabWriter_hpp

#include <memory>

namespace scissum {
    
    class Sci32SelectorVocab;
    
    class Sci32SelectorVocabWriter
    {
    public:
      
        void write(std::ostream &stream, std::shared_ptr<Sci32SelectorVocab> const &vocab);
        
    };
    
}

#endif /* Sci32SelectorVocabWriter_hpp */
