//
//  Sci32SelectorVocab.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/14/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32SelectorVocab_hpp
#define Sci32SelectorVocab_hpp

#include <vector>
#include <string>

namespace scissum {
    
    class Sci32SelectorVocab
    {
    public:
        
        /**
         *  Creates a selector vocabulary with the specified number of entries.
         */
        Sci32SelectorVocab(size_t count);
        
        /**
         *  @return the value of the specified entry.
         */
        std::string &get(size_t index);

        /**
         *  @return the value of the specified entry.
         */
        std::string const &get(size_t index) const;
        
        /**
         *  @return the number of vocabulary entries.
         */
        size_t size() const;
        
    private:
        
        std::vector<std::string> d_vocab;
        
    };
    
}

#endif /* Sci32SelectorVocab_hpp */
