//
//  Binary.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 12/1/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Binary_hpp
#define Binary_hpp

#include <cassert>

namespace scissum {
    
    template <class T>
    size_t countLeadingZeros(T v)
    {
        assert(0 && "this method is not implemented for this data type");
    }
    
    template <>
    size_t countLeadingZeros(uint32_t v)
    {
        return __builtin_clz(v);
    }

    template <>
    size_t countLeadingZeros(uint64_t v)
    {
        return __builtin_clzll(v);
    }
    
}

#endif /* Binary_hpp */
