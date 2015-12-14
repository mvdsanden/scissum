//
//  HeapPointerHelper.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/9/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef HeapPointerHelper_hpp
#define HeapPointerHelper_hpp

#include "HeapPointer.hpp"
#include "HeapAllocator.hpp"

namespace scissum {
    
    class HeapPointerHelper
    {
    public:
        
        template <class ... Args>
        HeapPointerBase allocateTuple(Args ... args)
        {
            
        }
        
    };
    
}

#endif /* HeapPointerHelper_hpp */
