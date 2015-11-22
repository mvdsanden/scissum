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
    
    class DecompressorSourceBasic
    : public DecompressorSource
    {
    public:
        
        DecompressorSourceBasic();

        ~DecompressorSourceBasic();
        
        virtual bool getBit();
        
        virtual uint8_t getByte();
        
        virtual size_t getVariableBits(size_t bits);
        
    protected:
        
        virtual bool fillWord() = 0;

        size_t d_word;
        
        size_t d_wordFill;
        
    };

    class DecompressorMemorySource
    : public DecompressorSourceBasic
    {
    public:
        
        DecompressorMemorySource(uint8_t const *buffer, size_t length);
        
        ~DecompressorMemorySource();
        
    protected:
        
        virtual bool fillWord();
        
    private:
        
        uint8_t const *d_buffer;
        
        size_t d_length;
        
    };

    
    class DecompressorFileSource
    : public DecompressorSourceBasic
    {
    public:
        
        DecompressorFileSource(int fileDescriptor,
                               size_t offset = 0,
                               size_t length = 0);
        
        ~DecompressorFileSource();
        
    protected:
        
        virtual bool fillWord();
        
    private:

        int d_fd;
        
        bool d_pipe;
        
        size_t d_offset;
        
        size_t d_length;
        
    };
    
}

#endif /* DecompressorSource_hpp */
