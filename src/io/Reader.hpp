//
//  Reader.h
//  Scissum
//
//  Created by Marten van de Sanden on 11/20/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Reader_hpp
#define Reader_hpp

namespace scissum {
    
    class Reader
    {
    public:
        
        /**
         *  Reads up to length bytes into buffer.
         *
         *  @return the number of bytes read.
         */
        virtual size_t read(uint8_t *buffer, size_t length) = 0;
        
        /**
         *  Skips up to length bytes of input data.
         *
         *  @return the number of bytes skipped.
         */
        virtual size_t skip(size_t length) = 0;
        
        /**
         *  @return the current read pointer.
         */
        virtual size_t readOffset() const = 0;
        
    };
    
}

#endif /* Reader_hpp */
