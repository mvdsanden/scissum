//
//  LzsDecompressor.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef LzsDecompressor_hpp
#define LzsDecompressor_hpp

#include "Decompressor.hpp"

#include <memory>

namespace scissum {
    
    class DecompressorSource;
    
    class LzsDecompressor
    : public Decompressor
    {
    public:
        
        LzsDecompressor(std::unique_ptr<DecompressorSource> &&source);
        
        ~LzsDecompressor();
        
        virtual size_t read(uint8_t *buffer, size_t length);
        
        virtual size_t skip(size_t length);
        
    private:
        
        bool decompressByte(uint8_t &dest);
        
        size_t parseOffset();
        
        size_t parseLength();
        
        void pushHistory(uint8_t value);

        size_t skipFromHistory(size_t length);
        
        size_t copyFromHistory(uint8_t *buffer, size_t length);
        
        std::unique_ptr<DecompressorSource> d_source;
        
        uint8_t d_history[2048];
        size_t d_historyPos;
        size_t d_historyFill;
        
        size_t d_historyMatchPos;
        size_t d_historyMatchLength;
        
        bool d_endOfData;
        
    };
    
}

#endif /* LzsDecompressor_hpp */
