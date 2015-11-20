//
//  Sci32ResourceFileScanner.hpp
//  Scissum
//
//  Created by Marten van de Sanden on 11/19/15.
//  Copyright © 2015 MVDS. All rights reserved.
//

#ifndef Sci32ResourceFileScanner_hpp
#define Sci32ResourceFileScanner_hpp

#include <istream>

namespace scissum {
    
    class Sci32ResourceFileScanner
    {
    public:
        
        
        enum Types {
            TYPE_VIEW = 0,
            TYPE_PIC = 1,
            TYPE_SCRIPT = 2,
            TYPE_TEXT = 3,
            TYPE_SOUND = 4,
            TYPE_ETC = 5,
            TYPE_VOCAB = 6,
            TYPE_FONT = 7,
            TYPE_CURSOR = 8,
            TYPE_PATCH = 9,
            TYPE_BITMAP = 10,
            TYPE_PALETTE = 11,
            TYPE_AUDIO1 = 12,
            TYPE_AUDIO2 = 13,
            TYPE_SYNC = 14,
            TYPE_MESSAGE = 15,
            TYPE_MAP = 16,
            TYPE_CHUNK = 18,
            TYPE_AUDIO36 = 19,
            TYPE_SYNC36 = 20,
            TYPE_TRANSLATION = 21,
            TYPE_ROBOT = 22,
            TYPE_VMD = 23,
            TYPE_DUCK = 24,
            TYPE_CLUT = 25,
            TYPE_TARGA = 26,
            TYPE_ZZZ = 27,
            
            // Negative values???
            TYPE_VIEW2 = 128,
            TYPE_PIC2 = 129,
            TYPE_SCRIPT2 = 130,
            TYPE_TEXT2 = 131,
            TYPE_SOUND2 = 132,
            TYPE_MEMORY = 133,
            TYPE_VOCAB2 = 134,
            TYPE_FONT2 = 135,
            TYPE_CURSOR2 = 136,
            TYPE_PATCH2 = 137,
            TYPE_BITMAP2 = 138,
            TYPE_PALETTE2 = 139,
            TYPE_WAVE2 = 140,
            TYPE_AUDIO2_2 = 141,
            TYPE_SYNC2 = 142,
            TYPE_MESSAGE2 = 143,
            TYPE_MAP2 = 144,
            TYPE_HEAP2 = 145,
            TYPE_AUDIO36_2 = 146,
            TYPE_SYNC36_2 = 147,
            TYPE_XLATE = 148,
        };
        
        
        Sci32ResourceFileScanner(std::istream &stream);
        
        ~Sci32ResourceFileScanner();
        
        bool next();
        
        size_t resourceOffset() const;
        
        uint8_t resourceType() const;
        
        uint16_t resourceId() const;
        
        uint32_t resourceCompressedSize() const;
        
        uint32_t resourceUncompressedSize() const;
        
        uint16_t resourceCompressionMethod() const;
        
    private:
        
        struct ResourceEntryHeader {
            uint8_t resourceType;
            uint16_t resourceId;
            uint32_t resourceCompressedSize;
            uint32_t resourceUncompressedSize;
            uint16_t resourceCompressionMethod;
        } __attribute__ ((packed));
        
        std::istream &d_stream;
        
        size_t d_resourceOffset;
        
        ResourceEntryHeader d_entryHeader;
        
    };
    
}

#endif /* Sci32ResourceFileScanner_hpp */
