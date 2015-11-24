//
//  Endians.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/23/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Endians_hpp
#define Endians_hpp

#include <cassert>

//#include <endians.h>

namespace scissum {

    template <class T>
    T htole(T v)
    {
        assert(0 && "Conversion not implemented");
    }
    
    /*
    template <> uint16_t htole(uint16_t v) { htole16(v); }
    template <> uint32_t htole(uint32_t v) { htole32(v); }
    template <> uint64_t htole(uint64_t v) { htole64(v); }
    template <> int16_t htole(int16_t v) { htole16(v); }
    template <> int32_t htole(int32_t v) { htole32(v); }
    template <> int64_t htole(int64_t v) { htole64(v); }
     */
    
    template <class T>
    T letoh(T v)
    {
        return v;
    }

    /*
    template <> uint16_t letoh(uint16_t v) { le16toh(v); }
    template <> uint32_t letoh(uint32_t v) { le32toh(v); }
    template <> uint64_t letoh(uint64_t v) { le64toh(v); }
    template <> int16_t letoh(int16_t v) { le16toh(v); }
    template <> int32_t letoh(int32_t v) { le32toh(v); }
    template <> int64_t letoh(int64_t v) { le64toh(v); }
     */
    
}

#endif /* Endians_hpp */
