//
//  DecompressorSource.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef DecompressorSource_hpp
#define DecompressorSource_hpp

#include <cstdint>

namespace scissum {
    
    class DecompressorSource
    {
    public:
    
        virtual bool getBit() = 0;
        
        virtual uint8_t getByte() = 0;
        
        virtual size_t getVariableBits(size_t bits) = 0;
        
    };
    
    class DecompressorMemorySource
    : public DecompressorSource
    {
    public:
        
        DecompressorMemorySource(uint8_t const *buffer, size_t length);
        
        ~DecompressorMemorySource();
        
        virtual bool getBit();
        
        virtual uint8_t getByte();
        
        virtual size_t getVariableBits(size_t bits);
        
    private:
        
        bool fillWord();

        uint8_t const *d_buffer;
        
        size_t d_length;
        
        size_t d_word;
        
        size_t d_wordFill;
        
    };
    
}

#endif /* DecompressorSource_hpp */
